/*
 * gui_structures_def.h
 *
 *  Created on: 28 dic 2023
 *      Author: giuli
 */

#ifndef GUI_STRUCTURES_DEF_H_
#define GUI_STRUCTURES_DEF_H_

#include "lvgl/lvgl.h"

typedef struct
{
	lv_obj_t* ground;
	lv_obj_t* parent;
	lv_obj_t* but_up;
	lv_obj_t* but_down;
	lv_obj_t* but_left;
	lv_obj_t* but_right;
	lv_obj_t* but_GHz;
	lv_obj_t* but_MHz;
	lv_obj_t* but_KHz;
	lv_obj_t* but_Hz;
	lv_obj_t* but_dbm;
	lv_obj_t* but_ms;
	lv_obj_t* label_but_up;
	lv_obj_t* label_but_down;
	lv_obj_t* label_but_left;
	lv_obj_t* label_but_right;
	lv_obj_t* label_but_GHz;
	lv_obj_t* label_but_MHz;
	lv_obj_t* label_but_KHz;
	lv_obj_t* label_but_Hz;
	lv_obj_t* label_but_dbm;
	lv_obj_t* label_but_ms;
	lv_obj_t* btnm;
	lv_obj_t* ta;
	lv_obj_t* but_dummy;	//serve solo per avere un evento manuale, questo tasto non sarà visibile
}lv_tast_ui;




#endif /* GUI_STRUCTURES_DEF_H_ */
