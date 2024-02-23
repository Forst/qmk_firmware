#include QMK_KEYBOARD_H


// Ctrl-Shift shorthand
#define CS(x) C(S(x))


// Layers
enum {
    L_LINUX,
    L_FUNC,
};


// Tap Dance
#ifdef TAP_DANCE_ENABLE
    enum {
        TD_END_HOME,
    };
    tap_dance_action_t tap_dance_actions[] = {
        [TD_END_HOME] = ACTION_TAP_DANCE_DOUBLE(KC_END, KC_HOME),
    };

    #define TD_EH TD(TD_END_HOME)
#else
    #define TD_EH KC_END
#endif // TAP_DANCE_ENABLE


// Encoder
#ifdef ENCODER_ENABLE
    bool encoder_update_user(uint8_t index, bool clockwise) {
        if (layer_state_is(L_FUNC)) {
            tap_code(clockwise ? KC_BRIU : KC_BRID);
        } else {
            tap_code(clockwise ? KC_VOLU : KC_VOLD);
        }
        return false;
    }
#endif // ENCODER_ENABLE


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12      Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)      BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                 PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter             PgDn
//      Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right

    [L_LINUX] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_HOME,          KC_MUTE,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_PGUP,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGDN,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   TD_EH,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RCTL, MO(L_FUNC), RALT_T(KC_APP),  KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [L_FUNC] = LAYOUT(
        KC_SLEP, CS(KC_F1), CS(KC_F2), CS(KC_F3), CS(KC_F4), CS(KC_F5), CS(KC_F6), CS(KC_F7), CS(KC_F8), CS(KC_F9), CS(KC_F10), CS(KC_F11), CS(KC_F12), KC_PSCR, XXXXXXX,
        XXXXXXX, RGB_TOG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, XXXXXXX, RGB_VAI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,          XXXXXXX,
        XXXXXXX, RGB_HUI, RGB_VAD, RGB_HUD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,          XXXXXXX,
        _______,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, NK_TOGG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          _______, RGB_MOD, XXXXXXX,
        _______, _______, _______,                            XXXXXXX,                            _______, _______, _______, RGB_SPD, RGB_RMOD, RGB_SPI
    )
};
// clang-format on


// RGB
#ifdef RGB_MATRIX_ENABLE
    #include "leds.h"

    void rgb_matrix_set_hsv_obey_brightness(int index, uint8_t h, uint8_t s, uint8_t v) {
        uint16_t v_scaled = (v * rgb_matrix_config.hsv.v) / 255;
        HSV hsv = {h, s, (uint8_t)v_scaled};
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
    }

    #define RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(pin, colour) \
        if ((pin) >= led_min && (pin) < led_max) \
            rgb_matrix_set_hsv_obey_brightness(pin, colour);

    #define RGB_MATRIX_SET_HSV_FULL_OBEY_BRIGHTNESS(pin, h, s, v) \
        if ((pin) >= led_min && (pin) < led_max) \
            rgb_matrix_set_hsv_obey_brightness(pin, h, s, v);

    #ifdef RAW_ENABLE
        const int NOTIFICATION_LEDS[] = {
            // Left side
            PIN_LED_L1,
            PIN_LED_L2,
            PIN_LED_L3,
            PIN_LED_L4,
            PIN_LED_L5,
            PIN_LED_L6,
            PIN_LED_L7,
            PIN_LED_L8,

            // Right side
            PIN_LED_R1,
            PIN_LED_R2,
            PIN_LED_R3,
            PIN_LED_R4,
            PIN_LED_R5,
            PIN_LED_R6,
            PIN_LED_R7,
            PIN_LED_R8,

            // Space key
            PIN_SPACE,
        };

        static HSV notification_colour = {0, 0, 0};
        static int notification_repeats = 0;
        static bool notification_active = false;
        static uint16_t notification_timer = 0;

        // Blink notification LEDs when receiving packet from host
        void raw_hid_receive(uint8_t *data, uint8_t length) {
            if (length < 3)
                return;

            notification_colour.h = data[0];
            notification_colour.s = data[1];
            notification_colour.v = data[2];
            notification_active = false;
            notification_timer = timer_read();
            notification_repeats = RGB_NOTIFICATION_REPEAT;
        }
    #endif // RAW_ENABLE

    bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
        if (layer_state_is(L_FUNC)) {
            // Sleep
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_ESC, HSV_PURPLE);

            // Bootloader
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_BSLASH, HSV_RED);

            // PrintScreen
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_HOME, HSV_BLUE);

            // RGB control
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_1, HSV_WHITE);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_W, HSV_WHITE);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_A, HSV_WHITE);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_S, HSV_WHITE);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_D, HSV_WHITE);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_UP, HSV_WHITE);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_DOWN, HSV_WHITE);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_LEFT, HSV_WHITE);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_RIGHT, HSV_WHITE);

            // Function keys
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_F1, HSV_CYAN);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_F2, HSV_CYAN);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_F3, HSV_CYAN);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_F4, HSV_CYAN);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_F5, HSV_CYAN);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_F6, HSV_CYAN);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_F7, HSV_CYAN);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_F8, HSV_CYAN);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_F9, HSV_CYAN);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_F10, HSV_CYAN);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_F11, HSV_CYAN);
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_F12, HSV_CYAN);

            // NKRO
            #ifdef NKRO_ENABLE
                if (keymap_config.nkro) {
                    RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_N, HSV_GREEN);
                } else {
                    RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_N, HSV_RED);
                }
            #endif // NKRO_ENABLE
        }

        if (host_keyboard_led_state().caps_lock) {
            RGB_MATRIX_SET_HSV_OBEY_BRIGHTNESS(PIN_CAPS, HSV_RED);
        }

        // Notifications from host
        #ifdef RAW_ENABLE
            if (notification_active) {
                for (int i = 0; i < sizeof(NOTIFICATION_LEDS) / sizeof(NOTIFICATION_LEDS[0]); i++) {
                    RGB_MATRIX_SET_HSV_FULL_OBEY_BRIGHTNESS(
                        NOTIFICATION_LEDS[i],
                        notification_colour.h,
                        notification_colour.s,
                        notification_colour.v
                    );
                }
            }

            if (notification_repeats > 0 && timer_elapsed(notification_timer) >= RGB_NOTIFICATION_DELAY) {
                if (notification_active) notification_repeats--;
                notification_active = !notification_active;
                notification_timer = timer_read();
            }
        #endif // RAW_ENABLE

        return false;
    }
#endif // RGB_MATRIX_ENABLE
