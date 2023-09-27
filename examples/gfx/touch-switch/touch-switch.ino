/*
  This example uses the Arduino_GigaDisplayTouch and
  Arduino_GigaDisplay_GFX library to change the
  display whenever it is touched, inverting the colors.

  The circuit:
  - GIGA R1 WiFi
  - GIGA Display Shield

  created 10 sept 2023
  by Karl Söderby

  This example code is in the public domain.
*/

#include "Arduino_GigaDisplay_GFX.h"
#include "Arduino_GigaDisplayTouch.h"

Arduino_GigaDisplayTouch touchDetector;
GigaDisplay_GFX display;

#define WHITE 0xffff
#define BLACK 0x0000

#define screen_size_x 480
#define screen_size_y 800

int touch_x;
int touch_y;

int lastTouch;
int threshold = 250;

bool switch_1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  display.begin();

  if (touchDetector.begin()) {
    Serial.print("Touch controller init - OK");
  } else {
    Serial.print("Touch controller init - FAILED");
    while (1)
      ;
  }
  changeSwitch();
}

void loop() {
  uint8_t contacts;
  GDTpoint_t points[5];
  contacts = touchDetector.getTouchPoints(points);
  
  if (contacts > 0 && (millis() - lastTouch > threshold)) {
    Serial.print("Contacts: ");
    Serial.println(contacts);

    //record the x,y coordinates 
    for (uint8_t i = 0; i < contacts; i++) {
      touch_x = points[i].x;
      touch_y = points[i].y;
    }

    //as the display is 480x800, anywhere you touch the screen it will trigger
    if (touch_x < screen_size_x && touch_y < screen_size_y) {
      switch_1 = !switch_1;
      Serial.println("switched");
      changeSwitch();
    }
    lastTouch = millis();
  }
}

void changeSwitch() {
  if (switch_1) {
    display.fillScreen(BLACK);
    display.setTextColor(WHITE);
  } else {
    display.fillScreen(WHITE);
    display.setTextColor(BLACK);
  }
  display.setCursor(50, screen_size_y/2);
  display.setTextSize(5);
  display.print("Switched");
}