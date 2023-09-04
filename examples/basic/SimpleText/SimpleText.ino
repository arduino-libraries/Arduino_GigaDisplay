/*
  Simple Text

  This example initializes the 800x480 display on the 
  GIGA Display Shield, and writes "Hello World" at
  specific coordinates.

  The circuit:
  - GIGA R1 WiFi
  - GIGA Display Shield

  Created 4 sept 2023
  by Karl Söderby

  This example code is in the public domain.
*/

#include "Arduino_H7_Video.h"
#include "ArduinoGraphics.h"

Arduino_H7_Video Display(800, 480, GigaDisplayShield);

void setup() {
  Display.begin();
  Display.clear();
  Display.beginDraw();
  Display.textFont(Font_5x7);
  Display.stroke(255, 255, 255);
  Display.text("Hello world!", 50, 50);
  Display.endDraw();
}

void loop() {}