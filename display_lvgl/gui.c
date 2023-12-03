/*
 * gui.c
 *
 *  Created on: 1 dic 2023
 *      Author: giuli
 */

#include "cmsis_os.h"
#include "stdio.h"
#include "Tasklvgl.h"
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"

void gui_define(void)
{
//	lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x00FF00), LV_PART_MAIN);
//	lv_obj_t * spinner = lv_spinner_create(lv_scr_act(), 1000, 60);
//	lv_obj_set_size(spinner, 64, 64);
//	lv_obj_align(spinner, LV_ALIGN_CENTER, 0, 0);
	lv_example_grid_1();
}


