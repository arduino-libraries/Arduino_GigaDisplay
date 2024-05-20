// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.1
// LVGL version: 9.1.0
// Project name: SquareLine_Project

#include "ui.h"


// COMPONENT Button

lv_obj_t * ui_Button_create(lv_obj_t * comp_parent)
{

    lv_obj_t * cui_Button;
    cui_Button = lv_button_create(comp_parent);
    lv_obj_set_width(cui_Button, 100);
    lv_obj_set_height(cui_Button, 50);
    lv_obj_set_x(cui_Button, 0);
    lv_obj_set_y(cui_Button, -3);
    lv_obj_set_align(cui_Button, LV_ALIGN_CENTER);
    lv_obj_add_flag(cui_Button, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(cui_Button, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_t ** children = lv_malloc(sizeof(lv_obj_t *) * _UI_COMP_BUTTON_NUM);
    children[UI_COMP_BUTTON_BUTTON] = cui_Button;
    lv_obj_add_event_cb(cui_Button, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_Button, del_component_child_event_cb, LV_EVENT_DELETE, children);
    ui_comp_Button_create_hook(cui_Button);
    return cui_Button;
}
