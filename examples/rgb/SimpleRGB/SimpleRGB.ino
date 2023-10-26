/*
  Control the GIGA Display Shield's onboard RGB pixel.
  This example turns on each base color (red, green, blue).

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
  //all pixels off
  rgb.on(0, 0, 0);
  delay(250);

  //red pixel on
  rgb.on(255, 0, 0);
  delay(250);

  //green pixel on
  rgb.on(0, 255, 0);
  delay(250);

  //blue pixel on
  rgb.on(0, 0, 255);
  delay(250);

  //all pixels on
  rgb.on(255, 255, 255);
  delay(100);
}