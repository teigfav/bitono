/*
 * Tasklvgl.c
 *
 *  Created on: Nov 29, 2023
 *      Author: giuli
 */

#include "cmsis_os.h"
#include "TaskTest.h"
#include "stdio.h"
#include "Tasklvgl.h"
#include "lvgl/lvgl.h"
#include "tft.h"
#include "ER-TPC050A2-2_GSL1680F_H.h"

osThreadId lvgl_tickHandle;
osThreadId lvgl_timerHandle;

osMutexId mutex_lvgl_id;

// prototype declaration
void StartTickTask(void *argument);
void StartTimerTask(void *argument);

void InitlvglTask(void)
{

	mutex_lvgl_id = osMutexNew (NULL);

	const osThreadAttr_t TickTask_attributes = {
	  .name = "TickTask",
	  .stack_size = TickStackSize,
	  .priority = (osPriority_t) osPriorityNormal,
	};

	const osThreadAttr_t TimerTask_attributes = {
		  .name = "TimerTask",
		  .stack_size = TimerStackSize,
		  .priority = (osPriority_t) osPriorityNormal,
		};
	lvgl_tickHandle = osThreadNew(StartTickTask, NULL, &TickTask_attributes);
	lvgl_timerHandle = osThreadNew(StartTimerTask, NULL, &TimerTask_attributes);
}

void StartTickTask(void *argument)
{
	GSL1680_Init();
  for(;;)
  {
    lv_tick_inc(10);
    osDelay(5);
  }
}

void StartTimerTask(void *argument)
{
  for(;;)
  {
//	osMutexWait(mutex_lvgl_id, osWaitForever);
	lv_timer_handler();
//	osMutexRelease(mutex_lvgl_id);
	osDelay(20);
  }
}
