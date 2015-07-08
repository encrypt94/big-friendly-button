big-friendly-button
===================

Just a single button keyboard with customizable keycode.

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
   cd bft
   make
```

Usage
-----

   1. Attach the device to pc
   2. Set the [keycode][0] with `bfb set <keycode>`
   3. Press the button
   4. goto 3

[0]: http://www.usb.org/developers/hidpage/Hut1_12v2.pdf#53 "Complete list of keycodes - page 53"

TODO
----

   * [ ] Add schematics
   * [x] Switch to hex keycodes in bft