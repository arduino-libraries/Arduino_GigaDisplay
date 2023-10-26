# Arduino_GigaDisplayRGB Library

Minimal library for controlling the built-in RGB on the GIGA Display Shield via the IS31FL3197 driver (I2C).

To use this library:

```
#include <Arduino_GigaDisplay.h>
```


Minimal example:

```
#include <Arduino_GigaDisplay.h>

GigaDisplayRGB rgb;

void setup() {
  rgb.begin();
}

void loop() {
  rgb.on(255, 0, 0);
  delay(100);
  rgb.off();
  delay(100);
}
```