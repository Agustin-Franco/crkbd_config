# crkbd_config

My personal **CRKBD (Corne) QMK configuration**.

This repository contains my keymap, layers, and OLED configuration for my Corne keyboard.

## Philosophy

This layout was designed with a few goals in mind:

- **Minimal finger movement**
- **Heavy use of layers instead of stretching**
- **Home-row efficiency**
- **Practicality across multiple computers**

The base layer is **QWERTY** because I frequently switch between different computers, making alternative layouts like Dvorak impractical.

## Layers

The layout currently uses **4 layers**:

| Layer | Description |
|------|-------------|
| Base | Standard QWERTY typing layer |
| Lower | Numbers + media controls |
| Symbols | Programming symbols |
| Navigation | Arrows, navigation keys, function keys |

Layers are accessed using **momentary layer keys** (`MO()`).

## Symbol Layer

The symbol layer is heavily inspired by:

https://getreuer.info/posts/keyboards/symbol-layer/index.html

It follows the same philosophy of grouping symbols in logical positions to reduce cognitive load and finger travel.

## OLED

The OLED screens run a small animation.

Credits to:

**4xy0m**  
https://github.com/4xy0m/qmk_userspace

for the NAVI animations used in this configuration.

## Hardware

- **Keyboard:** Corne (CRKBD)
- **Firmware:** QMK
- **Features used:**
  - OLED display
  - Layer-based layout
  - Custom animations

## Why This Repo Exists

Mainly for:

- Backups
- Version control
- Easier updates
- Sharing the layout with others

## Structure

```
keyboards/crkbd/keymaps/<keymap_name>/
├── keymap.c
├── rules.mk
└── config.h
```

## Future Plans

- Improve symbol layer ergonomics
- Experiment with home-row mods
- More OLED animations
- Possibly add combos

## License

MIT (or same license as QMK if included inside QMK tree)
