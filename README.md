big-friendly-button
===================

Just a one button keyboard with customizable keycode.

![big-friendly-button](https://i.imgur.com/LjoTHO0.png)

Build
-----

`git clone --recursive git://github.com/encrypt94/big-friendly-button.git`

### Firmware

```sh
   cd firmware
   make hex
   make program
```

### Big friendly tool

```sh
   cd bfb
   make
```

Usage
-----

   1. Attach the device to pc
   2. Set the keycode with `bfb set <keycode>`
   3. Press the button
   4. goto 3