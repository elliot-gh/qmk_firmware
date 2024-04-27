# elliot-gh's GMMK Pro Keymap

## Keymap

Layer 0:

- Print Screen -> Delete
- Encoder click -> Play/Pause
- Delete -> Home

Layer 1:

- F9 -> Print Screen
- F10 -> Scroll Lock
- F11 -> Pause
- Print Screen -> Insert
- Encoder click -> Mute
- Delete -> Media next
- Page Up -> Media Previous
- Page Down -> Media Stop
- End -> App Key
- Esc -> Hold for 3000 ms to enter bootloader
- F4 -> RGB toggle
- Grave/Tab -> RGB previous/next mode
- Caps Lock -> Switch to a blank RGB matrix mode with indicator support
- Q/A -> RGB hue increase/decrease
- W/S -> RGB saturation increase/decrease
- E/D -> RGB value increase/decrease
- R/F -> RGB speed increase/decrease
- Left GUI -> GUI lock
- O -> NKRO Off (default forced on)
- N -> NKRO On (default forced on)

## Features

- Basic GUI (Windows key) lock activated with Fn + Left GUI.
- RGB indicators for Caps Lock, Scroll Lock, and GUI lock that are three white LEDs wide for better visibilty.

## Custom User RGB Matrix Effects

- `RGB_MATRIX_CUSTOM_esc_white_only`: Only Esc key is lit white (I have a custom keycap on that key only).
- `RGB_MATRIX_CUSTOM_all_white`: All keys/glow lit white.
- `RGB_MATRIX_CUSTOM_indicator_only`: No RGB lit, but this allows RGB indicators to work.
