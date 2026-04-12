// Copyright 2021 Nicolas Druoton (druotoni)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "transactions.h"

// global
#include "gui_state.h"
#include "boot.h"
#include "navi_logo.h"

#include "draw_helper.h"
#include "fast_random.h"

// left side
#include "layer_frame.h"
#include "burst.h"

// right side
#include "ring.h"


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT_split_3x6_3(   //Base layer
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                      KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ESC,
                                   KC_LGUI, MO(1), KC_ENT,        KC_SPC,  MO(2), KC_RALT
),

[1] = LAYOUT_split_3x6_3(   //Number layer
    KC_TAB,  KC_NO,   KC_MUTE, KC_VOLD, KC_VOLU, KC_NO,                     KC_NO,   KC_MPRV, KC_MPLY, KC_MNXT, KC_NO,   KC_BSPC,
    KC_LCTL, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_NO,
    KC_LSFT, KC_NO,   KC_BRID, KC_NO,   KC_BRIU, KC_NO,                     KC_NO,   KC_NO,   KC_COMM, KC_DOT,  KC_SLSH, KC_ESC,
                                   KC_LGUI, KC_TRNS, KC_ENT,      KC_SPC,  MO(3), KC_RALT
),

[2] = LAYOUT_split_3x6_3(   //Symbol layer
    KC_TAB,  KC_GRV,  KC_LT,   KC_GT,   KC_MINS, KC_PIPE,                   KC_HASH, KC_LCBR, KC_RCBR, KC_DLR,  KC_BSLS, KC_BSPC,
    KC_LCTL, KC_EXLM, KC_PAST, KC_SLSH, KC_EQL,  KC_AMPR,                   KC_UNDS, KC_LPRN, KC_RPRN, KC_SCLN, KC_DQUO, KC_CIRC,
    KC_LSFT, KC_TILD, KC_PPLS, KC_LBRC, KC_RBRC, KC_PERC,                   KC_AT,   KC_COLN, KC_COMM, KC_DOT,  KC_QUOT, KC_ESC,
                                   KC_LGUI, MO(3), KC_ENT,        KC_SPC,  KC_TRNS, KC_RALT
),

[3] = LAYOUT_split_3x6_3(   // Function layer
    KC_TAB,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_NO,                     KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_BSPC,
    KC_LCTL, KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_NO,                     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_NO,   KC_NO,
    KC_LSFT, KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NO,                     KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_ESC,
                                   KC_LGUI, KC_TRNS, KC_ENT,      KC_SPC,  KC_TRNS, KC_RALT
)

};
// sync transport
typedef struct _sync_keycode_t {
    uint16_t keycode;
} sync_keycode_t;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    // vertical orientation
    return OLED_ROTATION_270;
}

void render(gui_state_t t) {
    // logo
    render_logo(t);

#if IS_LEFT
    // left side
    render_layer_frame(t);
    render_gears();

    decay_scope();
    render_scope(t);
#endif

#if IS_RIGHT
    // right side
    render_circle(t);
#endif
}

void update(uint16_t keycode) {
#if IS_LEFT
    update_scope();
#endif

#if IS_RIGHT
    update_circle(keycode);
#endif
}

void reset(void) {
#if IS_LEFT
    reset_scope();
#endif

#if IS_RIGHT
    reset_ring();
#endif
}

void set_wackingup_mode_clean(void) {
    oled_clear();
    reset();
}

bool oled_task_user(void) {
    gui_state_t t = get_gui_state();

    // in sleep mode => turn display off
    if (t == _SLEEP) {
        oled_off();
        return false;
    }

    // not in sleep mode => screen is on
    oled_on();

#ifdef WITH_BOOT
    // in booting mode => display booting animation
    if (t == _BOOTING) {
        bool boot_finished = render_boot();
        if (boot_finished) {
            // end of the boot : wacking up
            set_wackingup_mode_clean();
            update_gui_state();
        }
        return false;
    }
#endif

    // in halting mode => display booting animation
    if (t == _HALTING) {
        render_halt();
        return false;
    }

    render(t);
    return false;
}

void process_key(uint16_t keycode) {
    // update screen with the new key
    update(keycode);

    gui_state_t t = get_gui_state();

    if (t == _IDLE) {
        // wake up animation
        reset();
    }

    if (t == _BOOTING || t == _HALTING) {
        // cancel booting or halting : waking_up
        set_wackingup_mode_clean();
    }

    if (t == _SLEEP) {
        // boot sequence
        set_wackingup_mode_clean();
        reset_boot();
    }

    update_gui_state();
}

bool should_process_keypress(void) {
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (record->event.pressed) {
        switch (keycode) {
            case LGUI_T(KC_A):
                if (record->tap.count && record->event.pressed) {
                    keycode = KC_A;
                } else {
                    keycode = KC_LGUI;
                }
                break;
            case LALT_T(KC_R):
                if (record->tap.count && record->event.pressed) {
                    keycode = KC_R;
                } else {
                    keycode = KC_LALT;
                }
                break;
            case LCTL_T(KC_S):
                if (record->tap.count && record->event.pressed) {
                    keycode = KC_S;
                } else {
                    keycode = KC_LCTL;
                }
                break;
            case LSFT_T(KC_T):
                if (record->tap.count && record->event.pressed) {
                    keycode = KC_T;
                } else {
                    keycode = KC_LSFT;
                }
                break;
            case LGUI_T(KC_O):
                if (record->tap.count && record->event.pressed) {
                    keycode = KC_O;
                } else {
                    keycode = KC_LGUI;
                }
                break;
            case LALT_T(KC_I):
                if (record->tap.count && record->event.pressed) {
                    keycode = KC_I;
                } else {
                    keycode = KC_LALT;
                }
                break;
            case LCTL_T(KC_E):
                if (record->tap.count && record->event.pressed) {
                    keycode = KC_E;
                } else {
                    keycode = KC_LCTL;
                }
                break;
            case LSFT_T(KC_N):
                if (record->tap.count && record->event.pressed) {
                    keycode = KC_N;
                } else {
                    keycode = KC_LSFT;
                }
                break;
            case LT(1, KC_SPC):
                if (record->tap.count && record->event.pressed) {
                    keycode = KC_SPC;
                }
                break;
            case LT(2, KC_TAB):
                if (record->tap.count && record->event.pressed) {
                    keycode = KC_TAB;
                }
                break;
            case LT(3, KC_ESC):
                if (record->tap.count && record->event.pressed) {
                    keycode = KC_ESC;
                }
                break;
            case LT(4, KC_BSPC):
                if (record->tap.count && record->event.pressed) {
                    keycode = KC_BSPC;
                }
                break;
            case LT(5, KC_ENT):
                if (record->tap.count && record->event.pressed) {
                    keycode = KC_ENT;
                }
                break;
            case LT(6, KC_DEL):
                if (record->tap.count && record->event.pressed) {
                    keycode = KC_DEL;
                }
                break;
        }
        // gui process the input
        process_key(keycode);
    }
    return true;
}

#if IS_LEFT
layer_state_t layer_state_set_user(layer_state_t state) {
    // update the frame with the layer name
    update_layer_frame(state);
    return state;
}
#endif


