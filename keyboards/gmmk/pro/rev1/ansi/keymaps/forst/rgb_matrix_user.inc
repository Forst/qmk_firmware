RGB_MATRIX_EFFECT(TYPING_HEATMAP_NOFB)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#    ifndef RGB_MATRIX_CUSTOM_TYPING_HEATMAP_NOFB_DECREASE_DELAY_MS
#        define RGB_MATRIX_CUSTOM_TYPING_HEATMAP_NOFB_DECREASE_DELAY_MS 25
#    endif

#    ifndef RGB_MATRIX_CUSTOM_TYPING_HEATMAP_NOFB_RADIUS
#        define RGB_MATRIX_CUSTOM_TYPING_HEATMAP_NOFB_RADIUS 48
#    endif

static const uint8_t BASE_HUE = 180;
static const uint8_t INCREASE_FACTOR = 32;

// A timer to track the last time we decremented all heatmap values.
static uint16_t heatmap_decrease_timer;
// Whether we should decrement the heatmap values during the next update.
static bool decrease_heatmap_values;
// Intensity state per LED
static uint8_t led_intensity_state[DRIVER_LED_TOTAL] = {0};

static void process_rgb_matrix_custom_typing_heatmap_nofb(uint8_t row, uint8_t col) {
    uint8_t led_index = g_led_config.matrix_co[row][col];
    if (led_index == NO_LED) return;

    led_point_t center = g_led_config.point[led_index];

    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        led_point_t key = g_led_config.point[i];

        uint16_t dx = key.x > center.x ? sub8(key.x, center.x) : sub8(center.x, key.x);
        uint16_t dy = key.y > center.y ? sub8(key.y, center.y) : sub8(center.y, key.y);

        uint16_t radius = sqrt16(dx*dx + dy*dy);
        if (radius >= RGB_MATRIX_CUSTOM_TYPING_HEATMAP_NOFB_RADIUS) continue;

        uint8_t effect = INCREASE_FACTOR * (RGB_MATRIX_CUSTOM_TYPING_HEATMAP_NOFB_RADIUS - radius) / RGB_MATRIX_CUSTOM_TYPING_HEATMAP_NOFB_RADIUS;
        led_intensity_state[i] = qadd8(led_intensity_state[i], effect);
    }
}

static bool TYPING_HEATMAP_NOFB(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (params->init) {
        rgb_matrix_set_color_all(0, 0, 0);
        memset(led_intensity_state, 0, sizeof led_intensity_state);
    }

    // The heatmap animation might run in several iterations depending on
    // `RGB_MATRIX_LED_PROCESS_LIMIT`, therefore we only want to update the
    // timer when the animation starts.
    if (params->iter == 0) {
        decrease_heatmap_values = timer_elapsed(heatmap_decrease_timer) >= RGB_MATRIX_CUSTOM_TYPING_HEATMAP_NOFB_DECREASE_DELAY_MS;

        // Restart the timer if we are going to decrease the heatmap this frame.
        if (decrease_heatmap_values) {
            heatmap_decrease_timer = timer_read();
        }
    }

    for (uint8_t i = led_min; i < led_max; i++) {
        uint8_t val = led_intensity_state[i];

        HSV hsv = {
            BASE_HUE - qsub8(val, 255 - BASE_HUE),
            rgb_matrix_config.hsv.s,
            scale8((qadd8(BASE_HUE, val) - BASE_HUE) * 3, rgb_matrix_config.hsv.v)
        };
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);

        if (decrease_heatmap_values) {
            led_intensity_state[i] = qsub8(val, 1);
        }
    }

    return led_max < DRIVER_LED_TOTAL;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
