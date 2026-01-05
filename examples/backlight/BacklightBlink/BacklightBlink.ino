/*
 * This example shows how to change the backlight intensity of the GIGA Display Shield.
 * 
 * The backlight intensity is set by passing a number between 0 and 100 to the .set() method.
 * Here we set the backlight to 0% and 100% intensity in a loop, changing the value every 1 second.
 * 
 * Initial author: Martino Facchin @facchinm
 * Modified by: Ali Jahangiri @aliphys
 */

#include <Arduino_GigaDisplay.h>

//Create backlight object
GigaDisplayBacklight backlight;

void setup() {
  backlight.begin();
}

void loop() {
  backlight.set(0); // backlight off - 0%
  delay(1000);
  backlight.set(100); // backlight on - 100%
  delay(1000);
}
