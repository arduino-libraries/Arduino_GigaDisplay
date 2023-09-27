/*
  This example uses the Arduino_GigaDisplay_GFX library
  to draw some geometrical shapes on a 800x480 canvas.

  The circuit:
  - GIGA R1 WiFi
  - GIGA Display Shield

  created 10 sept 2023
  by Karl Söderby

  This example code is in the public domain.
*/

#include "Arduino_GigaDisplay_GFX.h"

GigaDisplay_GFX display;

#define WHITE 0xffff
#define BLACK 0x0000

void setup() {
  display.begin();
  display.fillScreen(WHITE);
  display.drawTriangle(100, 200, 300, 400, 300, 600, BLACK);
  display.drawCircle(100, 100, 50, BLACK);
  display.drawRect(10, 650, 300, 80, BLACK);
  display.drawRoundRect(300, 50, 100, 100, 30, BLACK);
}
void loop() {}