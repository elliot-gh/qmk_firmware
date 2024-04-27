/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>
   Copyright 2021 Elliot Yoon

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

static bool gui_locked = false;
static const uint16_t RESET_HOLD_MS = 3000;
static uint16_t reset_timer = 0;
static uint8_t prev_mode_ind = RGB_MATRIX_CUSTOM_indicator_only;

enum custom_keycodes {
    GUI_LOCK = SAFE_RANGE,
    RST_DLY,
    RGB_IND
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,           KC_MPLY,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_HOME,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_PGUP,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGDN,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_END,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [1] = LAYOUT(
        RST_DLY, _______, _______, _______, RGB_TOG, _______, _______, _______, _______, KC_PSCR, KC_SCRL, KC_PAUS, _______, KC_INS,           KC_MUTE,
        RGB_RMOD,_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_MNXT,
        RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, _______, _______, _______, _______, NK_OFF,  _______, _______, _______, _______,          KC_MPRV,
        RGB_IND, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, _______, _______, _______, _______, _______, _______, _______,          _______,          KC_MSTP,
        _______,          _______, _______, _______, _______, _______, NK_ON,   _______, _______, _______, _______,          _______, _______, KC_APP,
        _______, GUI_LOCK,_______,                            _______,                            _______, _______, _______, _______, _______, _______
    )

};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) }
};
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case (GUI_LOCK):
            if (record->event.pressed) {
                gui_locked = !gui_locked;
            }
            return false;
        case KC_LGUI:
        case KC_RGUI:
            if (record->event.pressed) {
                return !gui_locked;
            }
            return true;
        case RGB_IND:
            if (record->event.pressed) {
                uint8_t current_mode = rgb_matrix_get_mode();
                if (current_mode == RGB_MATRIX_CUSTOM_indicator_only) {
                    rgb_matrix_mode(prev_mode_ind);
                } else {
                    prev_mode_ind = current_mode;
                    rgb_matrix_mode(RGB_MATRIX_CUSTOM_indicator_only);
                }
            }
            return false;
        case (RST_DLY):
            if (record->event.pressed) {
                reset_timer = timer_read();
            } else {
                if (timer_elapsed(reset_timer) >= RESET_HOLD_MS) {
                    reset_keyboard();
                }
            }
            return false;
        default:
            return true;
  }
}

// used in rgb_matrix_indicators_advanced_user()
void turn_on_indicators(const uint8_t index_arr[], const uint8_t size) {
    for (uint8_t index = 0; index < size; index++) {
        uint8_t led_index = index_arr[index];

        rgb_matrix_set_color(led_index, 255, 255, 255);
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // caps lock, a, s
    static const uint8_t arr_caps_lock[] = {3, 9, 15};
    if (host_keyboard_led_state().caps_lock) {
        turn_on_indicators(arr_caps_lock, sizeof(arr_caps_lock) / sizeof(arr_caps_lock[0]));
    }

    // F9, F10, F11
    static const uint8_t arr_scroll_lock[] = {50, 56, 61};
    if (host_keyboard_led_state().scroll_lock) {
        turn_on_indicators(arr_scroll_lock, sizeof(arr_scroll_lock) / sizeof(arr_scroll_lock[0]));
    }

    // lctrl, lgui, lalt
    static const uint8_t arr_gui_lock[] = {5, 11, 17};
    if (gui_locked) {
        turn_on_indicators(arr_gui_lock, sizeof(arr_gui_lock) / sizeof(arr_gui_lock[0]));
    }

    return false;
}


// https://www.reddit.com/r/glorious/comments/okrbjg/peeps_qmk_and_via_rgb_guide_only_for_ansi/
// RGB LED layout
// led number, function of the key
//  67, Side led 01    0, ESC      6, F1       12, F2       18, F3       23, F4       28, F5       34, F6       39, F7       44, F8       50, F9       56, F10      61, F11      66, F12      69, Prt       Rotary(Mute)   68, Side led 12
//  70, Side led 02    1, ~        7, 1        13, 2        19, 3        24, 4        29, 5        35, 6        40, 7        45, 8        51, 9        57, 0        62, -_       78, (=+)     85, BackSpc   72, Del        71, Side led 13
//  73, Side led 03    2, Tab      8, Q        14, W        20. E        25, R        30, T        36, Y        41, U        46, I        52, O        58, P        63, [{       89, ]}       93, \|        75, PgUp       74, Side led 14
//  76, Side led 04    3, Caps     9, A        15, S        21, D        26, F        31, G        37, H        42, J        47, K        53, L        59, ;:       64, '"                    96, Enter     86, PgDn       77, Side led 15
//  80, Side led 05    4, Sh_L     10, Z       16, X        22, C        27, V        32, B        38, N        43, M        48, ,<       54, .<       60, /?                    90, Sh_R     94, Up        82, End        81, Side led 16
//  83, Side led 06    5, Ct_L     11,Win_L    17, Alt_L                              33, SPACE                              49, Alt_R    55, FN                    65, Ct_R     95, Left     97, Down      79, Right      84, Side led 17
//  87, Side led 07                                                                                                                                                                                                        88, Side led 18
//  91, Side led 08
