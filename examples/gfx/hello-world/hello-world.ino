/*
  This example uses the Arduino_GigaDisplay_GFX library
  to print "Hello World!" on a 800x480 canvas.

  The circuit:
  - GIGA R1 WiFi
  - GIGA Display Shield

  created 10 sept 2023
  by Karl Söderby

  This example code is in the public domain.
*/

#include "Arduino_GigaDisplay_GFX.h"

GigaDisplay_GFX display; // create the object

#define BLACK 0x0000

void setup() {
  display.begin();
  display.fillScreen(BLACK);
  display.setCursor(10,10); //x,y
  display.setTextSize(5); //adjust text size
  display.print("Hello World!"); //print
}
void loop(){}