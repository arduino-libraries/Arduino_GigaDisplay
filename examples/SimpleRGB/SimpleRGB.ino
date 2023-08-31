#include <Arduino_GigaDisplayRGB.h>

//Create rgb object
GigaDisplayRGB rgb;

void setup() {
  //init library
  rgb.begin();
}

void loop() {
  //all pixels off
  rgb.on(0, 0, 0);
  delay(100);

  //red pixel on
  rgb.on(255, 0, 0);
  delay(100);

  //green pixel on
  rgb.on(0, 255, 0);
  delay(100);

  //blue pixel on
  rgb.on(0, 0, 255);
  delay(100);

  //all pixels on
  rgb.on(255, 255, 255);
  delay(100);
}