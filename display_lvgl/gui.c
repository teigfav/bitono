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

//void gui_define(void)
//{
//	static lv_style_t style_button_small;
//		lv_style_init(&style_button_small);
//		lv_style_set_text_font(&style_button_small, &lv_font_montserrat_12);
////		lv_style_set_bg_color(&style_button_small, lv_color_black());
//		lv_style_set_radius(&style_button_small,5);
//		lv_style_set_shadow_width(&style_button_small, 0);
//		lv_style_set_bg_color(&style_button_small, lv_color_hex(0xD3D3D3));
//		lv_style_set_text_color(&style_button_small, lv_color_black());
//		//lv_style_set_border_width(&style_button_small, 1);
//
//
//		/*Create a container with COLUMN flex direction*/
//		lv_obj_t* cont_col = lv_obj_create(lv_scr_act());
//		lv_obj_set_size(cont_col, 150, 100);
//		lv_obj_align(cont_col,LV_ALIGN_LEFT_MID, 0, 0);
//		lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);
//		//lv_obj_add_style(cont_col, &style_button_small,0 );
//
//
////		/*primo item del menu*/
//		lv_obj_t* obj_three_tone_indep;
//		lv_obj_t* label_three_tone_indep;
//		obj_three_tone_indep = lv_btn_create(cont_col);
////		lv_obj_set_size(obj_three_tone_indep, LV_PCT(100), LV_SIZE_CONTENT);
////		//lv_obj_set_size(obj_three_tone_indep, LV_PCT(110), LV_SIZE_CONTENT);
//		lv_obj_set_size(obj_three_tone_indep, 110, 30);
//		label_three_tone_indep = lv_label_create(obj_three_tone_indep);
//		lv_label_set_text_fmt(label_three_tone_indep, "Three tones");
//		lv_obj_center(label_three_tone_indep);
//		lv_obj_add_style(obj_three_tone_indep, &style_button_small,0 );
////
////		/*secondo item del menu*/
//		lv_obj_t* obj_set_power;
////		lv_obj_t* label_set_power;
//		obj_set_power = lv_btn_create(cont_col);
//		lv_obj_set_size(obj_set_power, LV_PCT(100), LV_SIZE_CONTENT);
////		lv_obj_set_size(obj_set_power, 120, 30);
////		label_set_power = lv_label_create(obj_set_power);
////		lv_label_set_text_fmt(label_set_power, "Set power");
////		lv_obj_center(label_set_power);
////		lv_obj_add_style(obj_set_power, &style_button_small, 0);
////
////		/*terzo item del menu*/
//		lv_obj_t* obj_one_tone_sweep;
////		lv_obj_t* label_one_tone_sweep;
//		obj_one_tone_sweep = lv_btn_create(cont_col);
//		lv_obj_set_size(obj_one_tone_sweep, LV_PCT(100), LV_SIZE_CONTENT);
////		lv_obj_set_size(obj_one_tone_sweep, 120, 30);
////		label_one_tone_sweep = lv_label_create(obj_one_tone_sweep);
////		lv_label_set_text_fmt(label_one_tone_sweep, "One tone sweep");
////		lv_obj_center(label_one_tone_sweep);
////		lv_obj_add_style(obj_one_tone_sweep, &style_button_small, 0);
//
//	//	lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x00FF00), LV_PART_MAIN);
//	//	lv_obj_t * spinner = lv_spinner_create(lv_scr_act(), 1000, 60);
//	//	lv_obj_set_size(spinner, 64, 64);
//	//	lv_obj_align(spinner, LV_ALIGN_CENTER, 0, 0);
//	//	lv_example_grid_1();
//	//	lv_example_flex_1();
//}

void gui_define(void)
{
	   uint8_t dec_GHz = 7;
	   uint8_t un_GHz = 1;
	   uint64_t f1=71000000000;
	   uint64_t f2 = 72000000000;
	   lv_obj_t* act_scr = lv_scr_act();
	   lv_obj_t* label;
	   lv_obj_t* label_f1;
	   lv_obj_t* label_f2;
	   lv_obj_t* label_f3;
	   lv_obj_t* cont;
	   lv_obj_t* switch_f1;
	   lv_obj_t* switch_f2;
	   lv_obj_t* switch_f3;
	   lv_obj_t* cont_set_power;
	   lv_obj_t* cont_freq_sweep;
	   lv_obj_t* roller_container;
	   lv_obj_t* roller_10G;
	   lv_obj_t* roller_1G;
	   lv_obj_t* roller_100M;
	   lv_obj_t* roller_10M;
	   lv_obj_t* roller_1M;
	   lv_obj_t* roller_100K;
	   lv_obj_t* roller_10K;
	   lv_obj_t* roller_1K;
	   lv_obj_t* roller_100;
	   lv_obj_t* roller_10;
	   lv_obj_t* roller_1;
	   lv_obj_t* but_1;
	   const char* opts = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10";
	   const char* opts_gig = "7\n8";
	   //Write codes menuDemo
	   lv_obj_t* menuDemo = lv_menu_create(act_scr);
	   lv_obj_set_size(menuDemo, 480, 272);
	   lv_obj_set_style_bg_color(menuDemo, lv_color_hex(0xD3D3D3), 0);



	   //Crete threetonepage
	   lv_obj_t* threetone_page = lv_menu_page_create(menuDemo, (char*)"                         Three tones");
	   lv_obj_set_scrollbar_mode(threetone_page, LV_SCROLLBAR_MODE_OFF);
	   lv_obj_clear_flag(threetone_page,LV_OBJ_FLAG_SCROLLABLE);
	   label_f1 = lv_label_create(threetone_page);
	   lv_obj_set_parent(label_f1, threetone_page);
	   lv_label_set_text_fmt(label_f1, "Freq 1: %lld",f1);
	   lv_obj_add_flag(label_f1, LV_OBJ_FLAG_IGNORE_LAYOUT);
	   lv_obj_set_pos(label_f1, 50, 25);
	   switch_f1 = lv_switch_create(threetone_page);
	   lv_obj_add_flag(switch_f1, LV_OBJ_FLAG_IGNORE_LAYOUT);
	   lv_obj_set_size(switch_f1, 30, 15);
	   lv_obj_set_pos(switch_f1, 10, 25);

	   label_f2 = lv_label_create(threetone_page);
	   lv_obj_set_parent(label_f2, threetone_page);
	   lv_label_set_text_fmt(label_f2, "Freq 2: %lld", f2);
	   lv_obj_add_flag(label_f2, LV_OBJ_FLAG_IGNORE_LAYOUT);
	   lv_obj_set_pos(label_f2, 50, 50);
	   switch_f2 = lv_switch_create(threetone_page);
	   lv_obj_add_flag(switch_f2, LV_OBJ_FLAG_IGNORE_LAYOUT);
	   lv_obj_set_size(switch_f2, 30, 15);
	   lv_obj_set_pos(switch_f2, 10, 50);

	   roller_container = lv_menu_cont_create(threetone_page);
	   lv_obj_add_flag(roller_container, LV_OBJ_FLAG_IGNORE_LAYOUT);
	   lv_obj_set_pos(roller_container, 10, 150);

	   roller_10G = lv_roller_create(roller_container);
	   //lv_obj_add_flag(roller, LV_OBJ_FLAG_IGNORE_LAYOUT);
	   lv_roller_set_options(roller_10G, opts_gig, LV_ROLLER_MODE_NORMAL);
	   lv_roller_set_visible_row_count(roller_10G, 2);
	   lv_obj_set_width(roller_10G, 35);
	   //lv_obj_set_pos(roller, 10, 150);
	   //lv_obj_add_flag(but_1, LV_OBJ_FLAG_HIDDEN);

	   roller_1G = lv_roller_create(roller_container);
	   lv_roller_set_options(roller_1G, opts_gig, LV_ROLLER_MODE_NORMAL);
	   lv_roller_set_visible_row_count(roller_1G, 2);
	   lv_obj_set_width(roller_1G, 35);

//	    LV_IMG_DECLARE(wheel_130x130);
//	    lv_obj_t* img1 = lv_img_create(threetone_page);
//	    lv_img_set_src(img1, &wheel_130x130);
//	    lv_obj_align(img1, LV_ALIGN_CENTER, 0, -100);
//	    lv_obj_set_size(img1, 130, 130);

	   //metodo per creare un pulsante attivo ma non visibile
//	   but_1 = lv_button_create(threetone_page);
//	   lv_obj_add_event_cb(but_1, event_handler, LV_EVENT_ALL, NULL);
//	   lv_obj_set_style_opa(but_1, LV_OPA_TRANSP, 0);

	   lv_obj_t* power_page = lv_menu_page_create(menuDemo, (char*)"Set Powers");
	   lv_obj_t* freq_sweep_page = lv_menu_page_create(menuDemo, (char*)"Freq Sweep");
	   lv_obj_t* power_sweep_page = lv_menu_page_create(menuDemo, (char*)"Power Sweep");
	   lv_obj_t* setting_page = lv_menu_page_create(menuDemo, (char*)"Settings");
	   lv_obj_t* info_page = lv_menu_page_create(menuDemo, (char*)"Informations");

	   /*create menu page*/
	   lv_obj_t* menu_page = lv_menu_page_create(menuDemo, (char*)"      Menu");

	   cont = lv_menu_cont_create(menu_page);
	   label = lv_label_create(cont);
	   lv_label_set_text(label, "Three Tones");
	   lv_menu_set_load_page_event(menuDemo, cont, threetone_page);


	   cont = lv_menu_cont_create(menu_page);
	   label = lv_label_create(cont);
	   lv_label_set_text(label, "Set Powers");
	   lv_menu_set_load_page_event(menuDemo, cont, power_page);

	   cont = lv_menu_cont_create(menu_page);
	   label = lv_label_create(cont);
	   lv_label_set_text(label, "Freq Sweep");
	   lv_menu_set_load_page_event(menuDemo, cont, freq_sweep_page);

	   cont = lv_menu_cont_create(menu_page);
	   label = lv_label_create(cont);
	   lv_label_set_text(label, "Power Sweep");
	   lv_menu_set_load_page_event(menuDemo, cont, power_sweep_page);

	   cont = lv_menu_cont_create(menu_page);
	   label = lv_label_create(cont);
	   lv_label_set_text(label, "Settings");
	   lv_menu_set_load_page_event(menuDemo, cont, setting_page);

	   cont = lv_menu_cont_create(menu_page);
	   label = lv_label_create(cont);
	   lv_label_set_text(label, "Info");
	   lv_menu_set_load_page_event(menuDemo, cont, info_page);


	   lv_menu_set_sidebar_page(menuDemo, menu_page);

}
