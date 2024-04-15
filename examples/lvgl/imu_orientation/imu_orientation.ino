/*
* Online image converter for LVGL v9: https://lvgl.io/tools/imageconverter_v9
* Online image converter for LVGL v8 or earlier: https://lvgl.io/tools/imageconverter
*/

#include "Arduino_BMI270_BMM150.h"
#include "Arduino_H7_Video.h"
#include "lvgl.h"

Arduino_H7_Video          Display(800, 480, GigaDisplayShield); /* Arduino_H7_Video Display(1024, 768, USBCVideo); */

BoschSensorClass imu(Wire1);

LV_IMG_DECLARE(img_arduinologo);
lv_obj_t * img;

void setup() {
  Serial.begin(115200);
  
  Display.begin();
  imu.begin();

  img = lv_img_create(lv_scr_act());
  lv_img_set_src(img, &img_arduinologo);
  lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
  lv_img_set_pivot(img, (img_arduinologo.header.w)/2, (img_arduinologo.header.h)/2); /* Rotate around the center of the image */
}

uint8_t rotation = 0;

void loop() {
  float x, y, z;
  if (imu.accelerationAvailable()) {
    imu.readAcceleration(x, y, z);
    if ( z < 0.8 && z > -0.8) {
      if (x < -0.8) {
        rotation = 0;
      } else if (x > 0.8) {
        rotation = 2;
      } else if (y < -0.8) {
        rotation = 1;
      } else if (y > 0.8) {
        rotation = 3;
      }
      int16_t rot_angle = 900 - atan(x / y) * 180.0 / M_PI * 10;
      lv_img_set_angle(img, rot_angle);
    }
  }
  lv_timer_handler();
}