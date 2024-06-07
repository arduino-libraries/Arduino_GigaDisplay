/*
  Changes the display backlight from very dim to 100%
  This example code is in the public domain.
*/

#include <Arduino_GigaDisplay.h>

//Create backlight object
GigaDisplayBacklight backlight;

void setup() {
  //init library
  backlight.begin();
}

void loop() {
  backlight.set(0); // backlight off - 0%
  delay(1000);
  backlight.set(100); // backlight on - 100%
  delay(1000);
}
