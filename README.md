# crkbd_config

My personal QMK configuration for my Corne (crkbd).

## Setup

Install **QMK MSYS**, then run:

```bash
qmk setup
qmk new-keymap -kb crkbd/rev1 -km <name>
```

Copy your QMK config files into the keymap folder you just created.

On Windows, this is usually:

```text
C:\Users\<Username>\qmk_firmware\keyboards\crkbd\keymaps\<name>
```

## Compile

```bash
qmk compile -kb crkbd/rev1 -km <name>
```

## Flashing left/right side

In `config.h`, choose the side before compiling/flashing:

```c
// Choose one side per build
//#define IS_LEFT 1
//#define IS_RIGHT 1
```

1. Uncomment `IS_LEFT` and flash the left half.  
2. Uncomment `IS_RIGHT` and flash the right half.

Done 

---

## Layers

I designed these layers to reduce finger travel.  
I use **QWERTY** instead of the “superior” layouts (DVORAK, etc.) because I switch computers often.

The symbol layer is heavily inspired by:
https://getreuer.info/posts/keyboards/symbol-layer/index.html

![Layer diagram 1](https://github.com/user-attachments/assets/4c6286ed-d721-421f-8bb4-544d15dfe772)
![Layer diagram 2](https://github.com/user-attachments/assets/ac19c508-8d5b-4718-9fae-e163b848c41d)

---

## OLED

![OLED demo](https://github.com/user-attachments/assets/10fbc453-a298-4d61-81d5-b46e23095460)

The OLED display runs the **HELL0 NAVI** interface.

Credit: **4xy0m**  
https://github.com/4xy0m/qmk_userspace/tree/main/keyboards/crkbd/keymaps/axyom
