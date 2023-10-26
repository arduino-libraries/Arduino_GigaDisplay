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

int i = 0;
void loop() {
  backlight.set(i++ % 100);
  delay(100);
}
