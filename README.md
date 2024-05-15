Arduino_GigaDisplay
===================

[![Compile Examples](https://github.com/arduino-libraries/Arduino_GigaDisplay/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_GigaDisplay/actions?workflow=Compile+Examples)
[![Arduino Lint](https://github.com/arduino-libraries/Arduino_GigaDisplay/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_GigaDisplay/actions?workflow=Arduino+Lint)
[![Spell Check](https://github.com/arduino-libraries/Arduino_GigaDisplay/actions/workflows/spell-check.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_GigaDisplay/actions?workflow=Spell+Check)

This library contains a set of examples to use the [GIGA Display Shield](docs.arduino.cc/hardware/giga-display-shield). Upon installing this library from the Arduino IDE, the following libraries will also be installed:
- [ArduinoGraphics](https://github.com/arduino-libraries/ArduinoGraphics)
- [Arduino_GigaDisplayTouch](https://github.com/arduino-libraries/Arduino_GigaDisplayTouch)
- [Arduino_BMI270_BMM150](https://github.com/arduino-libraries/Arduino_BMI270_BMM150)
- [Arduino_GigaDisplayGFX](https://github.com/arduino-libraries/Arduino_GigaDisplay_GFX)
- [Arducam_dvp](https://github.com/ArduCAM/Arducam_dvp)

It will also install the [lvgl](https://github.com/lvgl/lvgl) library, which is required for a large number of examples in this repository.

>For any issues with the examples, please visit the repository of the libraries listed above.

## GigaDisplayRGB

This library has a class called `GigaDisplayRGB`, which is used to control the built-in RGB on the GIGA Display Shield via the IS31FL3197 driver (I2C).
