#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "Arduino_GigaDisplayTouch.h"

Arduino_H7_Video          Display(800, 480, GigaDisplayShield);
Arduino_GigaDisplayTouch  TouchDetector;

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

  lv_obj_t * obj;

  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1,
                        LV_GRID_ALIGN_STRETCH, 0, 1);
  lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);

  lv_obj_t * cb;
  cb = lv_checkbox_create(obj);
  lv_checkbox_set_text(cb, "Apple");

  cb = lv_checkbox_create(obj);
  lv_checkbox_set_text(cb, "Banana");
  lv_obj_add_state(cb, LV_STATE_CHECKED);

  // static lv_style_t style_radio;
  // static lv_style_t style_radio_chk;
  // lv_style_init(&style_radio);
  // lv_style_set_radius(&style_radio, LV_RADIUS_CIRCLE);
  // lv_style_init(&style_radio_chk);
  // lv_style_set_bg_img_src(&style_radio_chk, NULL);
  
  // cb = lv_checkbox_create(obj);
  // lv_checkbox_set_text(cb, "Lemon");
  // lv_obj_add_flag(cb, LV_OBJ_FLAG_EVENT_BUBBLE);
  // lv_obj_add_style(cb, &style_radio, LV_PART_INDICATOR);
  // lv_obj_add_style(cb, &style_radio_chk, LV_PART_INDICATOR | LV_STATE_CHECKED);
  
  // cb = lv_checkbox_create(obj);
  // lv_checkbox_set_text(cb, "Melon");
  // lv_obj_add_flag(cb, LV_OBJ_FLAG_EVENT_BUBBLE);
  // lv_obj_add_style(cb, &style_radio, LV_PART_INDICATOR);
  // lv_obj_add_style(cb, &style_radio_chk, LV_PART_INDICATOR | LV_STATE_CHECKED);
  // lv_obj_add_state(cb, LV_STATE_CHECKED);
}

void loop() {
  lv_timer_handler();
}
