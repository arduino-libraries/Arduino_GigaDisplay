#include "Arduino_H7_Video.h"
#include "Arduino_GigaDisplayTouch.h"

#include "lvgl.h"

Arduino_H7_Video          Display(800, 480, GigaDisplayShield); /* Arduino_H7_Video Display(1024, 768, USBCVideo); */
Arduino_GigaDisplayTouch  TouchDetector;

static void set_slider_val(void * bar, int32_t val) {
  lv_bar_set_value((lv_obj_t *)bar, val, LV_ANIM_ON);
}

void setup() {
  Display.begin();
  TouchDetector.begin();

  lv_obj_t * screen = lv_obj_create(lv_scr_act());
  lv_obj_set_size(screen, Display.width(), Display.height());

  static lv_coord_t col_dsc[] = { 500, LV_GRID_TEMPLATE_LAST};
  static lv_coord_t row_dsc[] = { 400, LV_GRID_TEMPLATE_LAST};

  lv_obj_t * grid = lv_obj_create(lv_scr_act());

  lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);

  lv_obj_set_size(grid, Display.width(), Display.height());

  lv_obj_center(grid);

  lv_obj_t * label;
  lv_obj_t * obj;

  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1,
                        LV_GRID_ALIGN_STRETCH, 0, 1);
  lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);

  lv_obj_t * bar = lv_bar_create(obj);
  lv_obj_set_size(bar, 200, 20);
  lv_obj_center(bar);
  lv_bar_set_value(bar, 70, LV_ANIM_OFF);

  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_exec_cb(&a, set_slider_val);
  lv_anim_set_time(&a, 3000);
  lv_anim_set_playback_time(&a, 3000);
  lv_anim_set_var(&a, bar);
  lv_anim_set_values(&a, 0, 100);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
  lv_anim_start(&a);

}

void loop() {
  lv_timer_handler();
}
