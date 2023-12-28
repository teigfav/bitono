/*
 * gui.c
 *
 *  Created on: 1 dic 2023
 *      Author: giuli
 */

#include "cmsis_os.h"
#include "stdio.h"
#include "stdlib.h"
#include "Tasklvgl.h"
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "math.h"
#include "stm32h7xx_hal.h"
#include "gui.h"

extern osMutexId mutex_lvgl_id;

uint64_t f[3] = {71000000000,72000000000,13000000000};
lv_obj_t* f_digit[3][11];
lv_obj_t* f2_digit[11];
lv_obj_t* f3_digit[11];
lv_obj_t* label_switch_1;
lv_obj_t* label_switch_2;
lv_obj_t* label_switch_3;
lv_obj_t* switch_f1;
lv_obj_t* switch_f2;
lv_obj_t* switch_f3;
lv_obj_t* but_up;
lv_obj_t* but_down;
lv_obj_t* but_left;
lv_obj_t* but_right;
uint8_t index_raw=0;
uint8_t index_col=0;
div_t num;
uint64_t num_tmp = 0;

#define roller_width 34
#define my_bg_color lv_color_hex(0xD3D3D3)


static void event_handler_threetone_page(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* target = lv_event_get_target(e);
    LV_LOG_USER("Code=%d\n\r",code);
    if (code == LV_EVENT_PRESSED) {
       // LV_LOG_USER("Clicked %lld",target);
        //if (target == f1_digit[0])
        //{
    	for(int j=0;j<3;j++)
    	{
			for(int i=0;i<11;i++)
			{
				lv_obj_set_style_border_width(f_digit[j][i], 0, LV_PART_MAIN);
				if(target==f_digit[j][i])
				{
					index_raw=j;
					index_col=i;
				}

			}
    	}

        if(target==but_left && index_col<11)
        {
        	index_col=index_col+1;
        }
        else if(target==but_right && index_col>0)
        {
        	index_col=index_col-1;
        }
        else if(target==but_up)
        {
        	f[index_raw]=f[index_raw]+(uint64_t)pow(10,index_col);
    		num_tmp=f[index_raw];
    		for (int i = 0;i<11;i++)
    		{
    			int mod = num_tmp % 10;  //split last digit from number
    			num_tmp=num_tmp/10;
    			lv_label_set_text_fmt(f_digit[index_raw][i], "%u",(uint8_t)mod);
    		}

        }
        else if(target==but_down)
        {
        	f[index_raw]=f[index_raw]-(uint64_t)pow(10,index_col);
    		num_tmp=f[index_raw];
    		for (int i = 0;i<11;i++)
    		{
    			int mod = num_tmp % 10;  //split last digit from number
    			num_tmp=num_tmp/10;
    			lv_label_set_text_fmt(f_digit[index_raw][i], "%u",(uint8_t)mod);
    		}

        }
    		LV_LOG_USER("Index j=%d  i=%d\n\r",index_raw,index_col);
            lv_obj_set_style_border_width(f_digit[index_raw][index_col], 2, LV_PART_MAIN);
            //lv_obj_set_style_bg_color(f1_digit[1], lv_color_hex(0xFEFEFE), LV_PART_MAIN);
        //}

    }
    else if (code == LV_EVENT_VALUE_CHANGED) {
    	if(target==switch_f1)
    	{
    		lv_label_set_text_fmt(label_switch_1, "%s\n", lv_obj_has_state(switch_f1, LV_STATE_CHECKED) ? "On" : "Off");
    	}
    	else if(target==switch_f2)
    	{
    		lv_label_set_text_fmt(label_switch_2, "%s\n", lv_obj_has_state(switch_f2, LV_STATE_CHECKED) ? "On" : "Off");
    	}
    	else if(target==switch_f3)
    	{
    		lv_label_set_text_fmt(label_switch_3, "%s\n", lv_obj_has_state(switch_f3, LV_STATE_CHECKED) ? "On" : "Off");
    	}
    }
}

static void textarea_event_handler(lv_event_t * e)
{
    lv_obj_t * ta = lv_event_get_target(e);
    LV_LOG_USER("Enter was pressed. The current text is: %s", lv_textarea_get_text(ta));
}

static void btnm_event_handler(lv_event_t* e)
{
	float freq;
    lv_obj_t* obj = lv_event_get_target(e);
    lv_obj_t* ta = lv_event_get_user_data(e);
    const char* txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));

    if (strcmp(txt, LV_SYMBOL_BACKSPACE) == 0)
    	{
    	lv_textarea_del_char(ta);
    	}
    else if (strcmp(txt, "GHz") == 0)
		{
    	freq=atof(lv_textarea_get_text(ta));
    	lv_event_send(ta, LV_EVENT_READY, NULL);
		}
    else
    	{
    	lv_textarea_add_text(ta, txt);
    	}


}
#if 0
void gui_define(void)
{
	static lv_style_t style_button_small;
		lv_style_init(&style_button_small);
		lv_style_set_text_font(&style_button_small, &lv_font_montserrat_12);
//		lv_style_set_bg_color(&style_button_small, lv_color_black());
		lv_style_set_radius(&style_button_small,5);
		lv_style_set_shadow_width(&style_button_small, 0);
		lv_style_set_bg_color(&style_button_small, lv_color_hex(0xD3D3D3));
		lv_style_set_text_color(&style_button_small, lv_color_black());
		//lv_style_set_border_width(&style_button_small, 1);


		/*Create a container with COLUMN flex direction*/
		lv_obj_t* cont_col = lv_obj_create(lv_scr_act());
		lv_obj_set_size(cont_col, 150, 100);
		lv_obj_align(cont_col,LV_ALIGN_LEFT_MID, 0, 0);
		lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);
		//lv_obj_add_style(cont_col, &style_button_small,0 );


//		/*primo item del menu*/
		lv_obj_t* obj_three_tone_indep;
		lv_obj_t* label_three_tone_indep;
		obj_three_tone_indep = lv_btn_create(cont_col);
//		lv_obj_set_size(obj_three_tone_indep, LV_PCT(100), LV_SIZE_CONTENT);
//		//lv_obj_set_size(obj_three_tone_indep, LV_PCT(110), LV_SIZE_CONTENT);
		lv_obj_set_size(obj_three_tone_indep, 110, 30);
		label_three_tone_indep = lv_label_create(obj_three_tone_indep);
		lv_label_set_text_fmt(label_three_tone_indep, "Three tones");
		lv_obj_center(label_three_tone_indep);
		lv_obj_add_style(obj_three_tone_indep, &style_button_small,0 );
//
//		/*secondo item del menu*/
		lv_obj_t* obj_set_power;
//		lv_obj_t* label_set_power;
		obj_set_power = lv_btn_create(cont_col);
		lv_obj_set_size(obj_set_power, LV_PCT(100), LV_SIZE_CONTENT);
//		lv_obj_set_size(obj_set_power, 120, 30);
//		label_set_power = lv_label_create(obj_set_power);
//		lv_label_set_text_fmt(label_set_power, "Set power");
//		lv_obj_center(label_set_power);
//		lv_obj_add_style(obj_set_power, &style_button_small, 0);
//
//		/*terzo item del menu*/
		lv_obj_t* obj_one_tone_sweep;
//		lv_obj_t* label_one_tone_sweep;
		obj_one_tone_sweep = lv_btn_create(cont_col);
		lv_obj_set_size(obj_one_tone_sweep, LV_PCT(100), LV_SIZE_CONTENT);
//		lv_obj_set_size(obj_one_tone_sweep, 120, 30);
//		label_one_tone_sweep = lv_label_create(obj_one_tone_sweep);
//		lv_label_set_text_fmt(label_one_tone_sweep, "One tone sweep");
//		lv_obj_center(label_one_tone_sweep);
//		lv_obj_add_style(obj_one_tone_sweep, &style_button_small, 0);

	//	lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x00FF00), LV_PART_MAIN);
	//	lv_obj_t * spinner = lv_spinner_create(lv_scr_act(), 1000, 60);
	//	lv_obj_set_size(spinner, 64, 64);
	//	lv_obj_align(spinner, LV_ALIGN_CENTER, 0, 0);
	//	lv_example_grid_1();
	//	lv_example_flex_1();
}
#endif

#if 0
void gui_define(void)
{
	 uint8_t dec_GHz = 7;
	 uint8_t un_GHz = 1;
	 uint64_t f1=71000000000;
	 uint64_t f2 = 72000000000;
	 uint64_t f3 = 13000000000;
	 lv_obj_t* act_scr = lv_scr_act();
	 lv_obj_t* label;
	 lv_obj_t* label_f1;
	 lv_obj_t* label_f2;
	 lv_obj_t* label_f3;
	 lv_obj_t* label_f1_val;
	 lv_obj_t* label_f2_val;
	 lv_obj_t* label_f3_val;
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
	 const char* opts = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9";

	 //Write codes menuDemo
	 lv_obj_t* menuDemo = lv_menu_create(act_scr);
	 lv_obj_set_size(menuDemo, 480, 272);
	 lv_obj_set_style_bg_color(menuDemo, lv_color_hex(0xD3D3D3), 0);

	 static lv_style_t style_text_fixed;
	 lv_style_init(&style_text_fixed);
	 lv_style_set_text_font(&style_text_fixed, &lv_font_montserrat_12);


	 //Crete threetonepage
	 lv_obj_t* threetone_page = lv_menu_page_create(menuDemo, (char*)"                         Three tones");
	 //lv_obj_set_scrollbar_mode(threetone_page, LV_SCROLLBAR_MODE_OFF); //altrimenti si vede una riga in basso con colore strano
	 lv_obj_clear_flag(threetone_page, LV_OBJ_FLAG_SCROLLABLE);
	 label_f1 = lv_label_create(threetone_page);
	 lv_obj_set_parent(label_f1, threetone_page);
	 lv_label_set_text(label_f1, "Freq 1");
	 lv_obj_add_flag(label_f1, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_set_pos(label_f1, 70, 25);
	 lv_obj_add_style(label_f1, &style_text_fixed, 0);



	 switch_f1 = lv_switch_create(threetone_page);
	 lv_obj_add_flag(switch_f1, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_set_size(switch_f1, 35, 15);
	 lv_obj_set_pos(switch_f1, 10, 25);

	 label_f2 = lv_label_create(threetone_page);
	 lv_obj_set_parent(label_f2, threetone_page);
	 lv_label_set_text_fmt(label_f2, "Freq 2", f2);
	 lv_obj_add_flag(label_f2, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_set_pos(label_f2, 70, 50);

	 label_f2_val = lv_label_create(threetone_page);
	 lv_obj_set_parent(label_f2_val, threetone_page);
	 lv_label_set_text_fmt(label_f2_val, "%lld", f2);
	 lv_obj_add_flag(label_f2_val, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_set_pos(label_f2_val, 140, 50);

	 switch_f2 = lv_switch_create(threetone_page);
	 lv_obj_add_flag(switch_f2, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_set_size(switch_f2, 35, 15);
	 lv_obj_set_pos(switch_f2, 10, 50);

	 roller_container = lv_menu_cont_create(threetone_page);
	 lv_obj_add_flag(roller_container, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_set_pos(roller_container, 10, 120);
	 lv_obj_set_height(roller_container, 100);
	 //lv_obj_add_style(roller_container, &style_text_fixed, 0);

	 roller_10G = lv_roller_create(roller_container);
	 lv_roller_set_options(roller_10G, opts, LV_ROLLER_MODE_NORMAL);
	 lv_roller_set_visible_row_count(roller_10G, 2);
	 //lv_obj_set_width(roller_10G, 40);
	 //lv_obj_set_style_bg_color(roller_10G, lv_color_hex(0xD3D3D3), LV_PART_SELECTED);
	 //lv_obj_set_style_border_width(roller_10G, 0, LV_PART_SELECTED);
	 //lv_obj_set_style_border_side(roller_10G, LV_BORDER_SIDE_LEFT, LV_PART_SELECTED);
	 //lv_obj_set_style_border_color(roller_10G, lv_color_hex(0x000000), LV_PART_SELECTED);
	 lv_obj_set_style_text_font(roller_10G, &lv_font_montserrat_12, LV_PART_MAIN);
	 lv_obj_set_style_text_color(roller_10G, lv_color_hex(0x7F7F7F), LV_PART_MAIN);
	 lv_obj_set_style_bg_color(roller_10G, lv_color_hex(0xE6E6E6), LV_PART_MAIN);
	 lv_obj_set_style_text_font(roller_10G, &lv_font_montserrat_12, LV_PART_SELECTED);
	 lv_obj_set_style_text_color(roller_10G, lv_color_hex(0x7F7F7F), LV_PART_SELECTED);
	 lv_obj_set_style_bg_color(roller_10G, lv_color_hex(0xFFFFFF), LV_PART_SELECTED);
	 lv_obj_set_style_border_width(roller_10G, 0, LV_PART_MAIN);
	 lv_obj_set_style_radius(roller_10G, 0, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_10G, 10, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_10G, 10, LV_PART_SELECTED);
	 //lv_obj_set_style_border_width(roller_10G, 0, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
	 //lv_obj_set_style_outline_width(roller_10G, 0, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
	 // lv_obj_add_event_cb(rollerL, time_roller_event_cb, LV_EVENT_ALL, NULL);

	 lv_obj_set_size(roller_10G, roller_width, 70);

	 roller_1G = lv_roller_create(roller_container);
	 lv_roller_set_options(roller_1G, opts, LV_ROLLER_MODE_NORMAL);
	 lv_roller_set_visible_row_count(roller_1G, 2);
	 lv_obj_set_style_text_font(roller_1G, &lv_font_montserrat_12, LV_PART_MAIN);
	 lv_obj_set_style_text_color(roller_1G, lv_color_hex(0x7F7F7F), LV_PART_MAIN);
	 lv_obj_set_style_bg_color(roller_1G, lv_color_hex(0xE6E6E6), LV_PART_MAIN);
	 lv_obj_set_style_text_font(roller_1G, &lv_font_montserrat_12, LV_PART_SELECTED);
	 lv_obj_set_style_text_color(roller_1G, lv_color_hex(0x7F7F7F), LV_PART_SELECTED);
	 lv_obj_set_style_bg_color(roller_1G, lv_color_hex(0xFFFFFF), LV_PART_SELECTED);
	 lv_obj_set_style_border_width(roller_1G, 0, LV_PART_MAIN);
	 lv_obj_set_style_radius(roller_1G, 0, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_1G, 10, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_1G, 10, LV_PART_SELECTED);
	 lv_obj_set_size(roller_1G, roller_width, 70);
	 lv_obj_add_flag(roller_1G, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_align_to(roller_1G, roller_10G, LV_ALIGN_OUT_RIGHT_MID, 5, 0);


	 roller_100M = lv_roller_create(roller_container);
	 lv_roller_set_options(roller_100M, opts, LV_ROLLER_MODE_NORMAL);
	 lv_roller_set_visible_row_count(roller_100M, 2);
	 lv_obj_set_style_text_font(roller_100M, &lv_font_montserrat_12, LV_PART_MAIN);
	 lv_obj_set_style_text_color(roller_100M, lv_color_hex(0x7F7F7F), LV_PART_MAIN);
	 lv_obj_set_style_bg_color(roller_100M, lv_color_hex(0xE6E6E6), LV_PART_MAIN);
	 lv_obj_set_style_text_font(roller_100M, &lv_font_montserrat_12, LV_PART_SELECTED);
	 lv_obj_set_style_text_color(roller_100M, lv_color_hex(0x7F7F7F), LV_PART_SELECTED);
	 lv_obj_set_style_bg_color(roller_100M, lv_color_hex(0xFFFFFF), LV_PART_SELECTED);
	 lv_obj_set_style_border_width(roller_100M, 0, LV_PART_MAIN);
	 lv_obj_set_style_radius(roller_100M, 0, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_100M, 10, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_100M, 10, LV_PART_SELECTED);
	 lv_obj_set_size(roller_100M, roller_width, 70);
	 lv_obj_add_flag(roller_100M, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_align_to(roller_100M, roller_1G, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

	 roller_10M = lv_roller_create(roller_container);
	 lv_roller_set_options(roller_10M, opts, LV_ROLLER_MODE_NORMAL);
	 lv_roller_set_visible_row_count(roller_10M, 2);
	 lv_obj_set_style_text_font(roller_10M, &lv_font_montserrat_12, LV_PART_MAIN);
	 lv_obj_set_style_text_color(roller_10M, lv_color_hex(0x7F7F7F), LV_PART_MAIN);
	 lv_obj_set_style_bg_color(roller_10M, lv_color_hex(0xE6E6E6), LV_PART_MAIN);
	 lv_obj_set_style_text_font(roller_10M, &lv_font_montserrat_12, LV_PART_SELECTED);
	 lv_obj_set_style_text_color(roller_10M, lv_color_hex(0x7F7F7F), LV_PART_SELECTED);
	 lv_obj_set_style_bg_color(roller_10M, lv_color_hex(0xFFFFFF), LV_PART_SELECTED);
	 lv_obj_set_style_border_width(roller_10M, 0, LV_PART_MAIN);
	 lv_obj_set_style_radius(roller_10M, 0, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_10M, 10, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_10M, 10, LV_PART_SELECTED);
	 lv_obj_set_size(roller_10M, roller_width, 70);
	 lv_obj_add_flag(roller_10M, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_align_to(roller_10M, roller_100M, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

	 roller_1M = lv_roller_create(roller_container);
	 lv_roller_set_options(roller_1M, opts, LV_ROLLER_MODE_NORMAL);
	 lv_roller_set_visible_row_count(roller_1M, 2);
	 lv_obj_set_style_text_font(roller_1M, &lv_font_montserrat_12, LV_PART_MAIN);
	 lv_obj_set_style_text_color(roller_1M, lv_color_hex(0x7F7F7F), LV_PART_MAIN);
	 lv_obj_set_style_bg_color(roller_1M, lv_color_hex(0xE6E6E6), LV_PART_MAIN);
	 lv_obj_set_style_text_font(roller_1M, &lv_font_montserrat_12, LV_PART_SELECTED);
	 lv_obj_set_style_text_color(roller_1M, lv_color_hex(0x7F7F7F), LV_PART_SELECTED);
	 lv_obj_set_style_bg_color(roller_1M, lv_color_hex(0xFFFFFF), LV_PART_SELECTED);
	 lv_obj_set_style_border_width(roller_1M, 0, LV_PART_MAIN);
	 lv_obj_set_style_radius(roller_1M, 0, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_1M, 10, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_1M, 10, LV_PART_SELECTED);
	 lv_obj_set_size(roller_1M, roller_width, 70);
	 lv_obj_add_flag(roller_1M, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_align_to(roller_1M, roller_10M, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

	 roller_100K = lv_roller_create(roller_container);
	 lv_roller_set_options(roller_100K, opts, LV_ROLLER_MODE_NORMAL);
	 lv_roller_set_visible_row_count(roller_100K, 2);
	 lv_obj_set_style_text_font(roller_100K, &lv_font_montserrat_12, LV_PART_MAIN);
	 lv_obj_set_style_text_color(roller_100K, lv_color_hex(0x7F7F7F), LV_PART_MAIN);
	 lv_obj_set_style_bg_color(roller_100K, lv_color_hex(0xE6E6E6), LV_PART_MAIN);
	 lv_obj_set_style_text_font(roller_100K, &lv_font_montserrat_12, LV_PART_SELECTED);
	 lv_obj_set_style_text_color(roller_100K, lv_color_hex(0x7F7F7F), LV_PART_SELECTED);
	 lv_obj_set_style_bg_color(roller_100K, lv_color_hex(0xFFFFFF), LV_PART_SELECTED);
	 lv_obj_set_style_border_width(roller_100K, 0, LV_PART_MAIN);
	 lv_obj_set_style_radius(roller_100K, 0, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_100K, 10, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_100K, 10, LV_PART_SELECTED);
	 lv_obj_set_size(roller_100K, roller_width, 70);
	 lv_obj_add_flag(roller_100K, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_align_to(roller_100K, roller_1M, LV_ALIGN_OUT_RIGHT_MID, 5, 0);;

	 roller_10K = lv_roller_create(roller_container);
	 lv_roller_set_options(roller_10K, opts, LV_ROLLER_MODE_NORMAL);
	 lv_roller_set_visible_row_count(roller_10K, 2);
	 lv_obj_set_style_text_font(roller_10K, &lv_font_montserrat_12, LV_PART_MAIN);
	 lv_obj_set_style_text_color(roller_10K, lv_color_hex(0x7F7F7F), LV_PART_MAIN);
	 lv_obj_set_style_bg_color(roller_10K, lv_color_hex(0xE6E6E6), LV_PART_MAIN);
	 lv_obj_set_style_text_font(roller_10K, &lv_font_montserrat_12, LV_PART_SELECTED);
	 lv_obj_set_style_text_color(roller_10K, lv_color_hex(0x7F7F7F), LV_PART_SELECTED);
	 lv_obj_set_style_bg_color(roller_10K, lv_color_hex(0xFFFFFF), LV_PART_SELECTED);
	 lv_obj_set_style_border_width(roller_10K, 0, LV_PART_MAIN);
	 lv_obj_set_style_radius(roller_10K, 0, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_10K, 10, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_10K, 10, LV_PART_SELECTED);
	 lv_obj_set_size(roller_10K, roller_width, 70);
	 lv_obj_add_flag(roller_10K, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_align_to(roller_10K, roller_100K, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

	 roller_1K = lv_roller_create(roller_container);
	 lv_roller_set_options(roller_1K, opts, LV_ROLLER_MODE_NORMAL);
	 lv_roller_set_visible_row_count(roller_1K, 2);
	 lv_obj_set_style_text_font(roller_1K, &lv_font_montserrat_12, LV_PART_MAIN);
	 lv_obj_set_style_text_color(roller_1K, lv_color_hex(0x7F7F7F), LV_PART_MAIN);
	 lv_obj_set_style_bg_color(roller_1K, lv_color_hex(0xE6E6E6), LV_PART_MAIN);
	 lv_obj_set_style_text_font(roller_1K, &lv_font_montserrat_12, LV_PART_SELECTED);
	 lv_obj_set_style_text_color(roller_1K, lv_color_hex(0x7F7F7F), LV_PART_SELECTED);
	 lv_obj_set_style_bg_color(roller_1K, lv_color_hex(0xFFFFFF), LV_PART_SELECTED);
	 lv_obj_set_style_border_width(roller_1K, 0, LV_PART_MAIN);
	 lv_obj_set_style_radius(roller_1K, 0, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_1K, 10, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_1K, 10, LV_PART_SELECTED);
	 lv_obj_set_size(roller_1K, roller_width, 70);
	 lv_obj_add_flag(roller_1K, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_align_to(roller_1K, roller_10K, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

	 roller_100 = lv_roller_create(roller_container);
	 lv_roller_set_options(roller_100, opts, LV_ROLLER_MODE_NORMAL);
	 lv_roller_set_visible_row_count(roller_100, 2);
	 lv_obj_set_style_text_font(roller_100, &lv_font_montserrat_12, LV_PART_MAIN);
	 lv_obj_set_style_text_color(roller_100, lv_color_hex(0x7F7F7F), LV_PART_MAIN);
	 lv_obj_set_style_bg_color(roller_100, lv_color_hex(0xE6E6E6), LV_PART_MAIN);
	 lv_obj_set_style_text_font(roller_100, &lv_font_montserrat_12, LV_PART_SELECTED);
	 lv_obj_set_style_text_color(roller_100, lv_color_hex(0x7F7F7F), LV_PART_SELECTED);
	 lv_obj_set_style_bg_color(roller_100, lv_color_hex(0xFFFFFF), LV_PART_SELECTED);
	 lv_obj_set_style_border_width(roller_100, 0, LV_PART_MAIN);
	 lv_obj_set_style_radius(roller_100, 0, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_100, 10, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_100, 10, LV_PART_SELECTED);
	 lv_obj_set_size(roller_100, roller_width, 70);
	 lv_obj_add_flag(roller_100, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_align_to(roller_100, roller_1K, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

	 roller_10 = lv_roller_create(roller_container);
	 lv_roller_set_options(roller_10, opts, LV_ROLLER_MODE_NORMAL);
	 lv_roller_set_visible_row_count(roller_10, 2);
	 lv_obj_set_style_text_font(roller_10, &lv_font_montserrat_12, LV_PART_MAIN);
	 lv_obj_set_style_text_color(roller_10, lv_color_hex(0x7F7F7F), LV_PART_MAIN);
	 lv_obj_set_style_bg_color(roller_10, lv_color_hex(0xE6E6E6), LV_PART_MAIN);
	 lv_obj_set_style_text_font(roller_10, &lv_font_montserrat_12, LV_PART_SELECTED);
	 lv_obj_set_style_text_color(roller_10, lv_color_hex(0x7F7F7F), LV_PART_SELECTED);
	 lv_obj_set_style_bg_color(roller_10, lv_color_hex(0xFFFFFF), LV_PART_SELECTED);
	 lv_obj_set_style_border_width(roller_10, 0, LV_PART_MAIN);
	 lv_obj_set_style_radius(roller_10, 0, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_10, 10, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_10, 10, LV_PART_SELECTED);
	 lv_obj_set_size(roller_10, roller_width, 70);
	 lv_obj_add_flag(roller_10, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_align_to(roller_10, roller_100, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

	 roller_1 = lv_roller_create(roller_container);
	 lv_roller_set_options(roller_1, opts, LV_ROLLER_MODE_NORMAL);
	 lv_roller_set_visible_row_count(roller_1, 2);
	 lv_obj_set_style_text_font(roller_1, &lv_font_montserrat_12, LV_PART_MAIN);
	 lv_obj_set_style_text_color(roller_1, lv_color_hex(0x7F7F7F), LV_PART_MAIN);
	 lv_obj_set_style_bg_color(roller_1, lv_color_hex(0xE6E6E6), LV_PART_MAIN);
	 lv_obj_set_style_text_font(roller_1, &lv_font_montserrat_12, LV_PART_SELECTED);
	 lv_obj_set_style_text_color(roller_1, lv_color_hex(0x7F7F7F), LV_PART_SELECTED);
	 lv_obj_set_style_bg_color(roller_1, lv_color_hex(0xFFFFFF), LV_PART_SELECTED);
	 lv_obj_set_style_border_width(roller_1, 0, LV_PART_MAIN);
	 lv_obj_set_style_radius(roller_1, 0, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_1, 10, LV_PART_MAIN);
	 lv_obj_set_style_text_line_space(roller_1, 10, LV_PART_SELECTED);
	 lv_obj_set_size(roller_1, roller_width, 70);
	 lv_obj_add_flag(roller_1, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_align_to(roller_1, roller_10, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

	 label_f1_val = lv_label_create(roller_container);
	 //lv_obj_set_parent(label_f1_val, threetone_page);
	 lv_label_set_text(label_f1_val, "10G     1G       100M     10M       1M     100K     10K     1K       100U      10U      1U");
	 lv_obj_add_flag(label_f1_val, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 lv_obj_align_to(label_f1_val, roller_10G, LV_ALIGN_OUT_TOP_LEFT, 10, 0);
	 lv_obj_add_style(label_f1_val, &style_text_fixed, 0);

	 //LV_IMG_DECLARE(wheel_130x130);
	 //lv_obj_t* img1 = lv_img_create(threetone_page);
	 //lv_img_set_src(img1, &wheel_130x130);
	 //lv_obj_add_flag(img1, LV_OBJ_FLAG_IGNORE_LAYOUT);
	 //lv_obj_set_pos(img1, 200, 120);




	 //lv_obj_add_flag(but_1, LV_OBJ_FLAG_HIDDEN);

	 //metodo per creare un pulsante attivo ma non visibile
//	 but_1 = lv_but_create(threetone_page);
//	 lv_obj_add_event_cb(but_1, event_handler, LV_EVENT_ALL, NULL);
//	 lv_obj_set_style_opa(but_1, LV_OPA_TRANSP, 0);

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
#endif
lv_obj_t* img_start;
//lv_obj_t* act_scr;
void my_timer(lv_timer_t * timer)
{
	LV_LOG_USER("Timeout\n\r");
	lv_obj_add_flag(img_start, LV_OBJ_FLAG_HIDDEN);
}




void gui_define(void)
{

	lv_obj_t* act_scr = lv_scr_act();
	lv_obj_t* label;
	lv_obj_t* label_f1;
	lv_obj_t* label_f2;
	lv_obj_t* label_f3;
	lv_obj_t* cont;
	lv_obj_t* default_cont;
	lv_obj_t * btnm;
	lv_obj_t* ta;
//	lv_obj_t* cont_set_power;
//	lv_obj_t* cont_freq_sweep;

	   static const char* btnm_map[] = { "1", "2", "3", "GHz","\n",
	                          "4", "5", "6", "MHz", "\n",
	                          "7", "8", "9","KHz", "\n",
	                          LV_SYMBOL_BACKSPACE, "0", ".","Hz", "" };


	//Write codes menuDemo
	lv_obj_t* menuDemo = lv_menu_create(act_scr);
	lv_obj_set_size(menuDemo, 480, 272);
	lv_obj_set_style_bg_color(menuDemo, my_bg_color, 0);
	static lv_style_t style_text_fixed;
	lv_style_init(&style_text_fixed);
	lv_style_set_text_font(&style_text_fixed, &lv_font_montserrat_18);


	//Create threetonepage
	osMutexWait(mutex_lvgl_id, osWaitForever);
	lv_obj_t* threetone_page = lv_menu_page_create(menuDemo, (char*)"                         Three tones");
	//lv_obj_set_scrollbar_mode(threetone_page, LV_SCROLLBAR_MODE_OFF); //altrimenti si vede una riga in basso con colore strano
	lv_obj_clear_flag(threetone_page, LV_OBJ_FLAG_SCROLLABLE);

	label_f1 = lv_label_create(threetone_page);
	lv_obj_set_parent(label_f1, threetone_page);
	lv_label_set_text(label_f1, "Freq 1");
	lv_obj_add_flag(label_f1, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_pos(label_f1, 50, 25);
	lv_obj_add_style(label_f1, &style_text_fixed, 0);
	lv_obj_add_event_cb(label_f1, event_handler_threetone_page, LV_EVENT_PRESSED, NULL);

	label_f2 = lv_label_create(threetone_page);
	lv_obj_set_parent(label_f2, threetone_page);
	lv_label_set_text(label_f2, "Freq 2");
	lv_obj_add_flag(label_f2, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_pos(label_f2, 50, 50);
	lv_obj_add_style(label_f2, &style_text_fixed, 0);
	lv_obj_add_event_cb(label_f2, event_handler_threetone_page, LV_EVENT_PRESSED, NULL);

	label_f3 = lv_label_create(threetone_page);
	lv_obj_set_parent(label_f3, threetone_page);
	lv_label_set_text(label_f3, "Freq 3");
	lv_obj_add_flag(label_f3, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_pos(label_f3, 50, 75);
	lv_obj_add_style(label_f3, &style_text_fixed, 0);
	lv_obj_add_event_cb(label_f3, event_handler_threetone_page, LV_EVENT_PRESSED, NULL);

	for (int j=0;j<3;j++)
	{
		num_tmp=f[j];
//	    while(num_tmp > 0) //do till num greater than  0
//	    {
//	        int mod = num % 10;  //split last digit from number
//	        vect_num[0]=(uint8_t)mod;
//	        printf("%d\n",mod); //print the digit.
//
//	        num = num / 10;    //divide num by 10. num /= 10 also a valid one
//	    }
		for (int i = 0;i<11;i++)
		{
			int mod = num_tmp % 10;  //split last digit from number
			num_tmp=num_tmp/10;
			f_digit[j][i] = lv_label_create(threetone_page);
			lv_obj_set_parent(f_digit[j][i], threetone_page);
			lv_label_set_text_fmt(f_digit[j][i], "%u",(uint8_t)mod);
			lv_obj_add_flag(f_digit[j][i], LV_OBJ_FLAG_IGNORE_LAYOUT);
			lv_obj_set_pos(f_digit[j][i], 270-i*15, 25+j*25);
			lv_obj_set_style_bg_color(f_digit[j][i], my_bg_color, LV_PART_MAIN);
			lv_obj_add_style(f_digit[j][i], &style_text_fixed, 0);
			lv_obj_add_flag(f_digit[j][i], LV_OBJ_FLAG_CLICKABLE);
			lv_obj_add_event_cb(f_digit[j][i], event_handler_threetone_page, LV_EVENT_PRESSED, NULL);
			lv_obj_set_style_bg_color(f_digit[j][i], lv_color_hex(0xFEFEFE), LV_PART_MAIN);
		}
	}

    ta = lv_textarea_create(threetone_page);
    lv_obj_add_flag(ta, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_textarea_set_one_line(ta, true);
    lv_obj_set_pos(ta, 10, 108);
    lv_obj_set_style_text_font(ta, &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_set_width(ta, 180);
    lv_obj_add_event_cb(ta, textarea_event_handler, LV_EVENT_READY, ta);
    lv_obj_add_state(ta, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/

	btnm = lv_btnmatrix_create(threetone_page);
	lv_obj_add_flag(btnm, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_style_text_font(btnm, &lv_font_montserrat_10, LV_PART_MAIN);
	lv_obj_add_event_cb(btnm, btnm_event_handler, LV_EVENT_VALUE_CHANGED, ta);
	lv_obj_set_pos(btnm, 10,140);
	lv_obj_set_size(btnm, 180, 110);
	lv_btnmatrix_set_map(btnm, btnm_map);




//	switch_f1 = lv_switch_create(threetone_page);
//	lv_obj_add_flag(switch_f1, LV_OBJ_FLAG_IGNORE_LAYOUT);
//	lv_obj_set_size(switch_f1, 45, 20);
//	lv_obj_set_pos(switch_f1, 10, 25);
//	label_switch_1 = lv_label_create(threetone_page);
//	lv_obj_add_flag(label_switch_1, LV_OBJ_FLAG_IGNORE_LAYOUT);
//	//lv_obj_set_size(label_switch_1, 45, 20);
//	lv_obj_set_pos(label_switch_1, 18, 10);
//	lv_obj_add_event_cb(switch_f1, event_handler_threetone_page, LV_EVENT_VALUE_CHANGED, NULL);



//	for (int i = 0;i<11;i++)
//	{
//	    f2_digit[i] = lv_label_create(threetone_page);
//	    lv_obj_set_parent(f2_digit[i], threetone_page);
//	    lv_label_set_text(f2_digit[i], "7");
//	    lv_obj_add_flag(f2_digit[i], LV_OBJ_FLAG_IGNORE_LAYOUT);
//	    lv_obj_set_pos(f2_digit[i], 120+i*15, 50);
//	    lv_obj_set_style_bg_color(f2_digit[i], my_bg_color, LV_PART_MAIN);
//	    lv_obj_add_style(f2_digit[i], &style_text_fixed, 0);
//	    lv_obj_add_flag(f2_digit[i], LV_OBJ_FLAG_CLICKABLE);
//	    lv_obj_add_event_cb(f2_digit[i], event_handler_threetone_page, LV_EVENT_PRESSED, NULL);
//	    lv_obj_set_style_bg_color(f2_digit[i], lv_color_hex(0xFEFEFE), LV_PART_MAIN);
//	}

//	switch_f2 = lv_switch_create(threetone_page);
//	lv_obj_add_flag(switch_f2, LV_OBJ_FLAG_IGNORE_LAYOUT);
//	lv_obj_set_size(switch_f2, 45, 20);
//	lv_obj_set_pos(switch_f2, 10, 50);
//	label_switch_2 = lv_label_create(threetone_page);
//	lv_obj_add_flag(label_switch_2, LV_OBJ_FLAG_IGNORE_LAYOUT);
//	//lv_obj_set_size(label_switch_1, 45, 20);
//	lv_obj_set_pos(label_switch_2, 18, 35);
//	lv_obj_add_event_cb(switch_f2, event_handler_threetone_page, LV_EVENT_VALUE_CHANGED, NULL);




//	for (int i = 0;i<11;i++)
//	{
//	    f3_digit[i] = lv_label_create(threetone_page);
//	    lv_obj_set_parent(f3_digit[i], threetone_page);
//	    lv_label_set_text(f3_digit[i], "7");
//	    lv_obj_add_flag(f3_digit[i], LV_OBJ_FLAG_IGNORE_LAYOUT);
//	    lv_obj_set_pos(f3_digit[i], 120+i*15, 75);
//	    lv_obj_set_style_bg_color(f3_digit[i], my_bg_color, LV_PART_MAIN);
//	    lv_obj_add_style(f3_digit[i], &style_text_fixed, 0);
//	    lv_obj_add_flag(f3_digit[i], LV_OBJ_FLAG_CLICKABLE);
//	    lv_obj_add_event_cb(f3_digit[i], event_handler_threetone_page, LV_EVENT_PRESSED, NULL);
//	    lv_obj_set_style_bg_color(f3_digit[i], lv_color_hex(0xFEFEFE), LV_PART_MAIN);
//	}

//	switch_f3 = lv_switch_create(threetone_page);
//	lv_obj_add_flag(switch_f3, LV_OBJ_FLAG_IGNORE_LAYOUT);
//	lv_obj_set_size(switch_f3, 45, 20);
//	lv_obj_set_pos(switch_f3, 10, 75);
//	label_switch_3 = lv_label_create(threetone_page);
//	lv_obj_add_flag(label_switch_3, LV_OBJ_FLAG_IGNORE_LAYOUT);
//	//lv_obj_set_size(label_switch_1, 45, 20);
//	lv_obj_set_pos(label_switch_3, 18, 50);
//	lv_obj_add_event_cb(switch_f3, event_handler_threetone_page, LV_EVENT_VALUE_CHANGED, NULL);

    LV_IMG_DECLARE(wheel_100x100);
    lv_obj_t* img1 = lv_img_create(threetone_page);
    lv_img_set_src(img1, &wheel_100x100);
    lv_obj_add_flag(img1, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_pos(img1, 200, 120);

    //lv_obj_add_flag(but_1, LV_OBJ_FLAG_HIDDEN);


    but_up = lv_btn_create(threetone_page);
    lv_obj_add_event_cb(but_up, event_handler_threetone_page, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(but_up, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(but_up, 25, 38);
    lv_obj_set_pos(but_up, 237, 120);
    //lv_obj_set_style_opa(but_up, LV_OPA_TRANSP, 0);  //metodo per creare un pulsante attivo ma non visibile

    but_down = lv_btn_create(threetone_page);
    lv_obj_add_event_cb(but_down, event_handler_threetone_page, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(but_down, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(but_down, 25, 38);
    lv_obj_set_pos(but_down, 237, 183);
    //lv_obj_set_style_opa(but_down, LV_OPA_TRANSP, 0);  //metodo per creare un pulsante attivo ma non visibile

    but_left = lv_btn_create(threetone_page);
    lv_obj_add_event_cb(but_left, event_handler_threetone_page, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(but_left, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(but_left, 38, 25);
    lv_obj_set_pos(but_left, 200, 158);
    //lv_obj_set_style_opa(but_up, LV_OPA_TRANSP, 0);  //metodo per creare un pulsante attivo ma non visibile

    but_right = lv_btn_create(threetone_page);
    lv_obj_add_event_cb(but_right, event_handler_threetone_page, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(but_right, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(but_right, 38, 25);
    lv_obj_set_pos(but_right, 262, 158);
    //lv_obj_set_style_opa(but_down, LV_OPA_TRANSP, 0);  //metodo per creare un pulsante attivo ma non visibile

    lv_obj_t* power_page = lv_menu_page_create(menuDemo, (char*)"Set Powers");
    lv_obj_t* freq_sweep_page = lv_menu_page_create(menuDemo, (char*)"Freq Sweep");
    lv_obj_t* power_sweep_page = lv_menu_page_create(menuDemo, (char*)"Power Sweep");
    lv_obj_t* setting_page = lv_menu_page_create(menuDemo, (char*)"Settings");
    lv_obj_t* info_page = lv_menu_page_create(menuDemo, (char*)"Informations");




		 /*create menu page*/
		 lv_obj_t* menu_page = lv_menu_page_create(menuDemo, (char*)"      Menu");

		 cont = lv_menu_cont_create(menu_page);
		 default_cont=cont;
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
		 //lv_menu_set_page(menuDemo, menu_page);

			osMutexRelease(mutex_lvgl_id);
			//lv_menu_set_mode_root_back_btn(menu_page, LV_MENU_ROOT_BACK_BTN_DISABLED);
			lv_event_send(default_cont,LV_EVENT_CLICKED,NULL);

		 LV_IMG_DECLARE(ABU);
		 img_start = lv_img_create(act_scr);
		 lv_img_set_src(img_start, &ABU);
		 lv_obj_add_flag(img1, LV_OBJ_FLAG_IGNORE_LAYOUT);

		 lv_timer_t *timer=lv_timer_create(my_timer,1500,NULL);
		 lv_timer_set_repeat_count(timer,1);

	}
