// Copyright 2022 Bryan Ong
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Physical layout:
//         [ ESC  ]          <- top key
// [ PTT ] [ --- ] [ GUI ]   <- big button (tap=Enter, hold=Ctrl+F18), inactive, GUI

enum custom_keycodes {
    KC_ENT_PTT = SAFE_RANGE,
};

static uint16_t ptt_timer = 0;
static bool ptt_held = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
                 KC_ESC,
        KC_ENT_PTT, KC_NO, KC_LGUI
    ),
};

void matrix_scan_user(void) {
    if (ptt_timer != 0 && !ptt_held && timer_elapsed(ptt_timer) >= TAPPING_TERM) {
        ptt_held = true;
        register_mods(MOD_BIT(KC_LCTL));
        register_code(KC_F18);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_ENT_PTT:
            if (record->event.pressed) {
                ptt_timer = timer_read() | 1; // ensure non-zero; timer_read() can legitimately return 0
                ptt_held = false;
            } else {
                if (ptt_held) {
                    ptt_held = false;
                    unregister_code(KC_F18);
                    unregister_mods(MOD_BIT(KC_LCTL));
                } else {
                    tap_code(KC_ENT);
                }
                ptt_timer = 0;
            }
            return false;
    }
    return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    return false; // encoder intentionally disabled
}
