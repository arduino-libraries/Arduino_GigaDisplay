#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "Arduino_GigaDisplayTouch.h"

Arduino_H7_Video          Display(800, 480, GigaDisplayShield);
Arduino_GigaDisplayTouch  TouchDetector;

static void btn_event_cb(lv_event_t * e) {
  lv_obj_t * btn = lv_event_get_target(e);
  lv_obj_t * label = lv_obj_get_child(btn, 0);
  lv_label_set_text_fmt(label, "Clicked!");
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

  lv_obj_t * btn = lv_btn_create(obj);
  lv_obj_set_size(btn, 100, 40);
  lv_obj_center(btn);
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);

  label = lv_label_create(btn);
  lv_label_set_text(label, "Click me!");
  lv_obj_center(label);

}

void loop() {
  lv_timer_handler();
}
