#pragma once

#if __has_include("serial_number.h")
    #include "serial_number.h"
#endif

#ifdef RGB_MATRIX_ENABLE
    #define RGB_DISABLE_WHEN_USB_SUSPENDED
    #define RGB_MATRIX_STARTUP_VAL 127
    #define RGB_MATRIX_KEYPRESSES

    #define RGB_NOTIFICATION_DELAY 100
    #define RGB_NOTIFICATION_REPEAT 3
#endif

#ifdef TAP_DANCE_ENABLE
    #define TAPPING_TERM 175
#endif

#define DEBOUNCE 20
