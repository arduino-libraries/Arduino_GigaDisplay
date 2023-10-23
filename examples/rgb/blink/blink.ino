/*
  Blinks the blue pixel on the GIGA Display Shield,
  every second.

  created 4 sept 2023
  by Karl Söderby

  This example code is in the public domain.
*/

#include <Arduino_GigaDisplay.h>

//Create rgb object
GigaDisplayRGB rgb;

void setup() {
  //init library
  rgb.begin();
}

void loop() {
  rgb.on(0, 0, 255);
  delay(1000);
  rgb.off();
  delay(1000);
}