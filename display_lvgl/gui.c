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
#include "gui_structures_def.h"
#include "type.h"
#include "TaskSint.h"


extern osMutexId mutex_lvgl_id;
extern struct fwd_ram_pwr_ctrl fwd_ram_pwr_ctrl;
extern struct sint_fram_parameters sint_fram_parameters;
extern osMessageQueueId_t QueueBiasHandle;
extern osMessageQueueId_t QueueSintHandle;

lv_tast_ui tastiera_freq;
lv_tast_ui tastiera_power;
//lv_obj_t* but_up;

typedef enum  {
	TreeTones,
	SweepFrq,
	Powers,
	SweepPow
}tastiera_t;

#define roller_width 34
#define my_bg_color lv_color_hex(0xD3D3D3)

uint64_t f[3] = {71000000000,72000000000,13000000000};
lv_obj_t* f_digit[3][11];
uint8_t index_raw=0;
uint8_t index_col=0;
osStatus_t status;


void init_freq_gui (void)
{
	f[0]=sint_fram_parameters.sint_par.freq_1;
	f[1]=sint_fram_parameters.sint_par.freq_2;
	f[2]=sint_fram_parameters.sint_par.freq_3;
	lv_event_send(tastiera_freq.but_dummy, LV_EVENT_PRESSED, NULL);
}

static void event_handler_threetone_page(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* target = lv_event_get_target(e);
    lv_obj_t* ta = lv_event_get_user_data(e);
    uint64_t num_tmp = 0;
    uint64_t freq_tmp = 0;
    struct sint_msg_t msg;
    char* s;
    double freq;
    LV_LOG_USER("threetonehandler code %d\n\r",code);
//    if(target==tastiera_freq.btnm)
    if(code==LV_EVENT_VALUE_CHANGED)  //evento generato dalla tastiera
    {
    	const char* txt = lv_btnmatrix_get_btn_text(target, lv_btnmatrix_get_selected_btn(target));
        if (strcmp(txt, LV_SYMBOL_BACKSPACE) == 0)
        	{
        	lv_textarea_del_char(tastiera_freq.ta);
        	}
        else if (strcmp(txt, ".") == 0)  //serve per verificare se nella stringa c'è già un punto e in caso non ne fa mettere un altro
    		{
        	s=strchr(lv_textarea_get_text(tastiera_freq.ta),'.');
        	if(s==NULL)
        		{
        		lv_textarea_add_text(tastiera_freq.ta, txt);
        		}
    		}
        else
        	{
        	lv_textarea_add_text(tastiera_freq.ta, txt);
        	}
    	LV_LOG_USER("tastiera_freq\n\r");
    }
    else if (code==LV_EVENT_PRESSED)   //evento generato dalla selezione del digit o o dai pulsanti (non quelli numerici della tastiera)
    {
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
        if(target==tastiera_freq.but_left && index_col<11)
        {
        	index_col=index_col+1;
        }
        else if(target==tastiera_freq.but_right && index_col>0)
		{
			index_col=index_col-1;
		}
		else if(target==tastiera_freq.but_up)
		{
			freq=f[index_raw];
			freq=freq+(uint64_t)pow(10,index_col);
			if (freq<=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index_raw].fmax && freq>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index_raw].fmin)
			{
				f[index_raw]=freq;
			}
			else
			{
				lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Warning", "Frequency out of range.", NULL, true);
			}

			//f[index_raw]=f[index_raw]+(uint64_t)pow(10,index_col);
		}
		else if(target==tastiera_freq.but_down)
		{
			freq=f[index_raw];
			freq=freq-(uint64_t)pow(10,index_col);
			if (freq<=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index_raw].fmax && freq>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index_raw].fmin)
			{
				f[index_raw]=freq;
			}
			else
			{
				lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Warning", "Frequency out of range.", NULL, true);
			}
		}
		else if(target==tastiera_freq.but_GHz)
		{
			freq=(uint64_t)(strtod(lv_textarea_get_text(tastiera_freq.ta),NULL)*1e9);
			if (freq<=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index_raw].fmax && freq>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index_raw].fmin)
			{
				f[index_raw]=freq;
			}
			else
			{
				lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Warning", "Frequency out of range.", NULL, true);
			}
		}
		else if(target==tastiera_freq.but_MHz)
		{
			freq=(uint64_t)(strtod(lv_textarea_get_text(tastiera_freq.ta),NULL)*1e6);
			if (freq<=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index_raw].fmax && freq>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index_raw].fmin)
			{
				f[index_raw]=freq;
			}
			else
			{
				lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Warning", "Frequency out of range.", NULL, true);
			}
		}
		else if(target==tastiera_freq.but_KHz)
		{
			freq=(uint64_t)(strtod(lv_textarea_get_text(tastiera_freq.ta),NULL)*1e3);
			if (freq<=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index_raw].fmax && freq>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index_raw].fmin)
			{
				f[index_raw]=freq;
			}
			else
			{
				lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Warning", "Frequency out of range.", NULL, true);
			}
		}
		else if(target==tastiera_freq.but_Hz)
		{
			freq=(uint64_t)(strtod(lv_textarea_get_text(tastiera_freq.ta),NULL)*1);
			if (freq<=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index_raw].fmax && freq>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index_raw].fmin)
			{
				f[index_raw]=freq;
			}
			else
			{
				lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Warning", "Frequency out of range.", NULL, true);
			}
		}
		else if(target==tastiera_freq.but_dummy)  //serve solo per aggiornare le frequenze conun evento generato manualmente in init_freq_gui()
		{
			for(int j=0;j<3;j++)
			{
				num_tmp=f[j];
				for (int i = 0;i<11;i++)
				{
					int mod = num_tmp % 10;  //split last digit from number
					num_tmp=num_tmp/10;
					lv_label_set_text_fmt(f_digit[j][i], "%u",(uint8_t)mod);
				}
			}
		}
        num_tmp=f[index_raw];
		for (int i = 0;i<11;i++)
		{
			int mod = num_tmp % 10;  //split last digit from number
			num_tmp=num_tmp/10;
			lv_label_set_text_fmt(f_digit[index_raw][i], "%u",(uint8_t)mod);
		}
		msg.op=write;
		msg.mode=indep;
		msg.sint=index_raw;
		msg.value=f[index_raw];
		msg.source=gui;
		status=osMessageQueuePut(QueueSintHandle,&msg,0,0);
		if(status!=osOK)
		{
			print_k("Error: No queue access\r\n");
		}

    }
	lv_obj_set_style_border_width(f_digit[index_raw][index_col], 2, LV_PART_MAIN);
}

static void event_handler_powers_page(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* target = lv_event_get_target(e);
    lv_obj_t* ta = lv_event_get_user_data(e);
    LV_LOG_USER("powershandler");
    if(target==tastiera_power.btnm)
    {
    	LV_LOG_USER("tastiera_power");
    }
}

void gen_tastiera (lv_tast_ui *ui,lv_obj_t* parent, lv_event_cb_t func_cb)
{
	//tastiera
	ui->ground=lv_obj_create(parent);
	lv_obj_add_flag(ui->ground, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_pos(ui->ground, 0, 110);
	lv_obj_set_size(ui->ground, 340, 162);
	lv_obj_set_style_bg_color(ui->ground,lv_color_hex(0x766F6F),0);
	lv_obj_set_style_radius(ui->ground, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(ui->ground, 0, LV_PART_MAIN);


	ui->btnm = lv_btnmatrix_create(parent);
	lv_obj_add_flag(ui->btnm, LV_OBJ_FLAG_IGNORE_LAYOUT);
	static const char* btnm_map[] = { "1", "2", "3","\n",
	                          "4", "5", "6", "\n",
	                          "7", "8", "9", "\n",
	                          LV_SYMBOL_BACKSPACE, "0", ".", "" };

	lv_btnmatrix_set_map(ui->btnm, btnm_map);
	lv_obj_set_pos(ui->btnm, 10, 140);
	lv_obj_set_size(ui->btnm, 138, 110);
	lv_obj_add_event_cb(ui->btnm, func_cb, LV_EVENT_VALUE_CHANGED, NULL);

	//Write style for menuDemo_phone_btnm, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->btnm, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->btnm, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->btnm, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->btnm, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->btnm, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui->btnm, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui->btnm, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->btnm, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ////Write style for menuDemo_phone_btnm, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->btnm, 1, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->btnm, lv_color_hex(0x000000), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->btnm, lv_color_hex(0xFFFFFF), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->btnm, 5, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->btnm, lv_color_hex(0x414141), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->btnm, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_width(ui->btnm, 22, LV_PART_ITEMS | LV_STATE_DEFAULT);

    ////Write style for menuDemo_phone_btnm, Part: LV_PART_ITEMS, State: LV_STATE_PRESSED.
    lv_obj_set_style_border_width(ui->btnm, 0, LV_PART_ITEMS | LV_STATE_PRESSED);
    //lv_obj_set_style_text_color(ui->btnm, lv_color_hex(0x000000), LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(ui->btnm, lv_color_hex(0xFFFFFF), LV_PART_ITEMS | LV_STATE_PRESSED);

    ui->but_Hz = lv_btn_create(parent);
    lv_obj_add_event_cb(ui->but_Hz, func_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(ui->but_Hz, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(ui->but_Hz, 38, 23);
    lv_obj_set_pos(ui->but_Hz, 152, 219);

    //Write style for but_Hz, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->but_Hz, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->but_Hz, lv_color_hex(0x414141), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->but_Hz, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->but_Hz, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->but_Hz, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->but_Hz, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for but_Hz, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_color(ui->but_Hz, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
//
    ui->label_but_Hz = lv_label_create(parent);
    lv_obj_add_flag(ui->label_but_Hz, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_label_set_text(ui->label_but_Hz, "Hz");
    lv_obj_set_style_text_font(ui->label_but_Hz, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_pos(ui->label_but_Hz, 164, 223);
    lv_obj_set_style_text_color(ui->label_but_Hz, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->but_KHz = lv_btn_create(parent);
    lv_obj_add_event_cb(ui->but_KHz, func_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(ui->but_KHz, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(ui->but_KHz, 38, 23);
    lv_obj_set_pos(ui->but_KHz, 152, 193);

    //Write style for but_KHz, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->but_KHz, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->but_KHz, lv_color_hex(0x414141), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->but_KHz, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->but_KHz, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->but_KHz, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->but_KHz, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for but_KHz, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_color(ui->but_KHz, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
//
    ui->label_but_KHz = lv_label_create(parent);
    lv_obj_add_flag(ui->label_but_KHz, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_label_set_text(ui->label_but_KHz, "KHz");
    lv_obj_set_style_text_font(ui->label_but_KHz, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_pos(ui->label_but_KHz, 158, 196);
    lv_obj_set_style_text_color(ui->label_but_KHz, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->but_MHz = lv_btn_create(parent);
    lv_obj_add_event_cb(ui->but_MHz, func_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(ui->but_MHz, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(ui->but_MHz, 38, 23);
    lv_obj_set_pos(ui->but_MHz, 152, 166);

    //Write style for but_MHz, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->but_MHz, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->but_MHz, lv_color_hex(0x414141), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->but_MHz, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->but_MHz, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->but_MHz, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->but_MHz, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

     //Write style for but_MHz, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_color(ui->but_MHz, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);

    ui->label_but_MHz = lv_label_create(parent);
    lv_obj_add_flag(ui->label_but_MHz, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_label_set_text(ui->label_but_MHz, "MHz");
    lv_obj_set_style_text_font(ui->label_but_MHz, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_pos(ui->label_but_MHz, 158, 170);
    lv_obj_set_style_text_color(ui->label_but_MHz, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->but_GHz = lv_btn_create(parent);
    lv_obj_add_event_cb(ui->but_GHz, func_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(ui->but_GHz, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(ui->but_GHz, 38, 23);
    lv_obj_set_pos(ui->but_GHz, 152, 140);

    //Write style for but_GHz, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->but_GHz, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->but_GHz, lv_color_hex(0x414141), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->but_GHz, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->but_GHz, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->but_GHz, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->but_GHz, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

     //Write style for but_GHz, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_color(ui->but_GHz, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);

    ui->label_but_GHz = lv_label_create(parent);
    lv_obj_add_flag(ui->label_but_GHz, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_label_set_text(ui->label_but_GHz, "GHz");
    lv_obj_set_style_text_font(ui->label_but_GHz, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_pos(ui->label_but_GHz, 158, 144);
    lv_obj_set_style_text_color(ui->label_but_GHz, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->but_dbm = lv_btn_create(parent);
    lv_obj_add_event_cb(ui->but_dbm, func_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(ui->but_dbm, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(ui->but_dbm, 38, 23);
    lv_obj_set_pos(ui->but_dbm, 194, 219);

    //Write style for but_dbm, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->but_dbm, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->but_dbm, lv_color_hex(0x414141), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->but_dbm, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->but_dbm, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->but_dbm, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->but_dbm, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for but_dBm, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_color(ui->but_dbm, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);

    ui->label_but_dbm = lv_label_create(parent);
    lv_obj_add_flag(ui->label_but_dbm, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_label_set_text(ui->label_but_dbm, "dBm");
    lv_obj_set_style_text_font(ui->label_but_dbm, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_pos(ui->label_but_dbm, 198, 223);
    lv_obj_set_style_text_color(ui->label_but_dbm, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->but_ms = lv_btn_create(parent);
    lv_obj_add_event_cb(ui->but_ms, func_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(ui->but_ms, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(ui->but_ms, 38, 22);
    lv_obj_set_pos(ui->but_ms, 194, 193);

    //Write style for but_ms, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->but_ms, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->but_ms, lv_color_hex(0x414141), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->but_ms, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->but_ms, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->but_ms, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->but_ms, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for but_up, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_color(ui->but_ms, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);

    ui->label_but_ms = lv_label_create(parent);
    lv_obj_add_flag(ui->label_but_ms, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_label_set_text(ui->label_but_ms, "ms");
    lv_obj_set_style_text_font(ui->label_but_ms, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_pos(ui->label_but_ms, 202, 196);
    lv_obj_set_style_text_color(ui->label_but_ms, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->but_up = lv_btn_create(parent);
    lv_obj_add_event_cb(ui->but_up, func_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(ui->but_up, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(ui->but_up, 38, 38);
    lv_obj_set_pos(ui->but_up, 245, 120);

    //Write style for but_up, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->but_up, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->but_up, lv_color_hex(0x414141), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->but_up, 33, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->but_up, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->but_up, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->but_up, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for but_up, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_color(ui->but_up, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);

    ui->label_but_up = lv_label_create(parent);
    lv_obj_add_flag(ui->label_but_up, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_label_set_text(ui->label_but_up, "+");
    lv_obj_set_style_text_font(ui->label_but_up, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_pos(ui->label_but_up, 259, 128);
    lv_obj_set_style_text_color(ui->label_but_up, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->but_down = lv_btn_create(parent);
    lv_obj_add_event_cb(ui->but_down, func_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(ui->but_down, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(ui->but_down, 38, 38);
    lv_obj_set_pos(ui->but_down, 245, 170);

    //Write style for bu_down, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->but_down, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->but_down, lv_color_hex(0x414141), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->but_down, 33, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->but_down, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->but_down, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->but_down, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for but_down, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_color(ui->but_down, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);

    ui->label_but_down = lv_label_create(parent);
    lv_obj_add_flag(ui->label_but_down, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_label_set_text(ui->label_but_down, "-");
    lv_obj_set_style_text_font(ui->label_but_down, &lv_font_montserrat_18, LV_PART_MAIN);
    lv_obj_set_pos(ui->label_but_down, 260, 176);
    lv_obj_set_style_text_color(ui->label_but_down, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->but_left = lv_btn_create(parent);
    lv_obj_add_event_cb(ui->but_left, func_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(ui->but_left, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(ui->but_left, 38, 38);
    lv_obj_set_pos(ui->but_left, 210, 145);

    //Write style for but_left, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->but_left, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->but_left, lv_color_hex(0x414141), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->but_left, 33, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->but_left, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->but_left, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->but_left, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for but_left, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_color(ui->but_left, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);

    ui->label_but_left = lv_label_create(parent);
    lv_obj_add_flag(ui->label_but_left, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_label_set_text(ui->label_but_left, "<<");
    lv_obj_set_style_text_font(ui->label_but_left, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_pos(ui->label_but_left, 220, 155);
    lv_obj_set_style_text_color(ui->label_but_left, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->but_right = lv_btn_create(parent);
    lv_obj_add_event_cb(ui->but_right, func_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(ui->but_right, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(ui->but_right, 38, 38);
    lv_obj_set_pos(ui->but_right, 280, 145);

    //Write style for but_right, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->but_right, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->but_right, lv_color_hex(0x414141), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->but_right, 33, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->but_right, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->but_right, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->but_right, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for but_right, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_color(ui->but_right, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);

    ui->label_but_right = lv_label_create(parent);
    lv_obj_add_flag(ui->label_but_right, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_label_set_text(ui->label_but_right, ">>");
    lv_obj_set_style_text_font(ui->label_but_right, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_pos(ui->label_but_right, 290, 155);
    lv_obj_set_style_text_color(ui->label_but_right, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for but_right, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_color(ui->but_left, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);

    ui->but_dummy = lv_btn_create(parent);
    lv_obj_add_event_cb(ui->but_dummy, func_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_flag(ui->but_dummy, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(ui->but_dummy, 10, 10);
    lv_obj_set_pos(ui->but_dummy, 320, 195);
    lv_obj_set_style_bg_color(ui->but_dummy, lv_color_hex(0x766F6F), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->ta = lv_textarea_create(parent);
    lv_obj_add_flag(ui->ta, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_textarea_set_one_line(ui->ta, true);
    lv_obj_set_pos(ui->ta, 10, 114);
    lv_obj_set_style_text_font(ui->ta, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_width(ui->ta, 180);
    lv_obj_add_event_cb(ui->ta, func_cb, LV_EVENT_READY, ui->ta);
    lv_obj_add_state(ui->ta, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/
    lv_obj_set_style_pad_top(ui->ta, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->ta, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->ta, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->ta, my_bg_color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->ta, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->ta, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
}


void gui_define(void)
{

	lv_obj_t* act_scr = lv_scr_act();
	lv_obj_t* label;
	lv_obj_t* cont;
	lv_obj_t* label_f1;
	lv_obj_t* label_f2;
	lv_obj_t* label_f3;
	lv_obj_t* label_f1_end;
	lv_obj_t* label_f2_end;
	lv_obj_t* label_f3_end;
	uint64_t num_tmp = 0;



	//Write codes menuDemo
	lv_obj_t* menuDemo = lv_menu_create(act_scr);
	lv_obj_set_size(menuDemo, 480, 272);
	lv_obj_set_style_bg_color(menuDemo, my_bg_color, 0);
	static lv_style_t style_text_fixed;
	lv_style_init(&style_text_fixed);
	lv_style_set_text_font(&style_text_fixed, &lv_font_montserrat_18);


	//---------------------Create threetonepage-----------------------------
	osMutexWait(mutex_lvgl_id, osWaitForever);
	lv_obj_t* threetone_page = lv_menu_page_create(menuDemo, (char*)"                         Two tones + OL");
	//lv_obj_set_scrollbar_mode(threetone_page, LV_SCROLLBAR_MODE_OFF); //altrimenti si vede una riga in basso con colore strano
	lv_obj_clear_flag(threetone_page, LV_OBJ_FLAG_SCROLLABLE);

	gen_tastiera(&tastiera_freq,threetone_page,event_handler_threetone_page);

	label_f1 = lv_label_create(threetone_page);
	lv_obj_set_parent(label_f1, threetone_page);
	lv_label_set_text(label_f1, "Tone 1");
	lv_obj_add_flag(label_f1, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_pos(label_f1, 50, 25);
	lv_obj_add_style(label_f1, &style_text_fixed, 0);

	label_f1_end = lv_label_create(threetone_page);
	lv_obj_set_parent(label_f1_end, threetone_page);
	lv_label_set_text(label_f1_end, "Hz");
	lv_obj_add_flag(label_f1_end, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_pos(label_f1_end, 290, 25);
	lv_obj_add_style(label_f1_end, &style_text_fixed, 0);


	label_f2 = lv_label_create(threetone_page);
	lv_obj_set_parent(label_f2, threetone_page);
	lv_label_set_text(label_f2, "Tone 2");
	lv_obj_add_flag(label_f2, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_pos(label_f2, 50, 50);
	lv_obj_add_style(label_f2, &style_text_fixed, 0);

	label_f2_end = lv_label_create(threetone_page);
	lv_obj_set_parent(label_f2_end, threetone_page);
	lv_label_set_text(label_f2_end, "Hz");
	lv_obj_add_flag(label_f2_end, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_pos(label_f2_end, 290, 50);
	lv_obj_add_style(label_f2_end, &style_text_fixed, 0);


	label_f3 = lv_label_create(threetone_page);
	lv_obj_set_parent(label_f3, threetone_page);
	lv_label_set_text(label_f3, "  LO");
	lv_obj_add_flag(label_f3, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_pos(label_f3, 50, 75);
	lv_obj_add_style(label_f3, &style_text_fixed, 0);

	label_f3_end = lv_label_create(threetone_page);
	lv_obj_set_parent(label_f3_end, threetone_page);
	lv_label_set_text(label_f3_end, "Hz");
	lv_obj_add_flag(label_f3_end, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_pos(label_f3_end, 290, 75);
	lv_obj_add_style(label_f3_end, &style_text_fixed, 0);


	for (int j=0;j<3;j++)
	{
		num_tmp=f[j];
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

	//----------------------Create power page---------------------------------
    lv_obj_t* power_page = lv_menu_page_create(menuDemo, (char*)"                          Set Powers");
    lv_obj_clear_flag(power_page, LV_OBJ_FLAG_SCROLLABLE);

    gen_tastiera(&tastiera_power,power_page,event_handler_powers_page);

    lv_obj_t* freq_sweep_page = lv_menu_page_create(menuDemo, (char*)"Freq Sweep");
    lv_obj_t* power_sweep_page = lv_menu_page_create(menuDemo, (char*)"Power Sweep");
    lv_obj_t* setting_page = lv_menu_page_create(menuDemo, (char*)"Settings");
    lv_obj_t* info_page = lv_menu_page_create(menuDemo, (char*)"Informations");

//	menuDemo_phone_btnm = lv_btnmatrix_create(threetone_page);
//	static const char *menuDemo_phone_btnm_text_map[] = {"1", "2", "3", "\n", "4", "5", "6", "\n", "7", "8", "9", "\n", "*", "0", "#", "",};
//	lv_btnmatrix_set_map(menuDemo_phone_btnm, menuDemo_phone_btnm_text_map);
//	lv_obj_set_pos(menuDemo_phone_btnm, 71, 38);
//	lv_obj_set_size(menuDemo_phone_btnm, 164, 177);



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
		 //lv_obj_set_style_max_width(lv_menu_get_cur_sidebar_page(menuDemo), 40, LV_STATE_DEFAULT);
		 //lv_menu_set_page(menuDemo, menu_page);


	}
