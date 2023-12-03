/*
 * Init.c
 *
 *  Created on: Jun 17, 2023
 *      Author: giuli
 */

#include "stdio.h"
#include "TaskTest.h"
#include "Task_bias.h"
#include "TaskGFX.h"
#include "TaskCLI.h"
#include "TaskSint.h"
#include "ER-TPC050A2-2_GSL1680F_H.h"
#include "ad5592r.h"
#include "main.h"
#include "cmsis_os.h"
#include "tft.h"
#include "Tasklvgl.h"
#include "gui.h"
#include "touch.h"

void InitGPIO(void);

void Initialization(void)
{
	//GSL1680_Init();
	InitGPIO();
	tft_init();
	gui_define();
	lvgl_touchscreen_init();
	InitBiasTask();
	InitTestTask();
	InitCLITask();
	InitGFXTask();
	InitSintTask();
	InitlvglTask();

}

void InitGPIO(void)
{
	HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS2_GPIO_Port,CS2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS1_GPIO_Port,CS1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS3_GPIO_Port,CS3_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CE1_GPIO_Port,CE1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CE3_GPIO_Port,CE3_Pin,GPIO_PIN_SET);
}


