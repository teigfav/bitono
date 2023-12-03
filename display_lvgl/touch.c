/*
 * touch.c
 *
 *  Created on: 2 dic 2023
 *      Author: giuli
 */

#include "touch.h"
#include "string.h"
#include "stdio.h"
#include "lvgl/lvgl.h"
#include "main.h"
#include "ER-TPC050A2-2_GSL1680F_H.h"
#include "Util.h"

static volatile uint8_t touch_irq = 0;

static void lvgl_touchscreen_read (lv_indev_drv_t *indev, lv_indev_data_t *data);

void lvgl_touchscreen_init (void)
{
  static lv_indev_drv_t indev_drv;
  /* basic LVGL driver initialization */
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = lvgl_touchscreen_read;
  /* register the driver in LVGL */
  lv_indev_drv_register(&indev_drv);
}

static void lvgl_touchscreen_read (lv_indev_drv_t  *indev, lv_indev_data_t *data)
{
	  static lv_coord_t last_x = 0;
	  static lv_coord_t last_y = 0;
	  uint8_t touch_data[8];
	if(touch_irq)
	{
		gsl_ts_read(0x80, touch_data, 8);
		touch_irq=0;
		//coordinate.touch=touch_data[0];
		last_x = (((uint16_t)touch_data[5]<<8) | (uint16_t)touch_data[4] ) & 0x0FFF; // 12 bits of X coord
		last_y = (((uint16_t)touch_data[7]<<8) | (uint16_t)touch_data[6] ) & 0x0FFF;
		data->state = LV_INDEV_STATE_PRESSED;
		print_k("%u  %u\r\n",last_x,last_y);
	}
	else
	{
		data->state = LV_INDEV_STATE_RELEASED;
	}
	  /* set the last pressed coordinates */
	  data->point.x = last_x;
	  data->point.y = last_y;
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == TOUCH_INT_Pin)
    touch_irq = 1;
}

