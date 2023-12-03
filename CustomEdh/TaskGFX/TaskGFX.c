/*
 * TaskGFX.c
 *
 *  Created on: Jul 1, 2023
 *      Author: giuli
 */
#include "cmsis_os.h"
#include "TaskGFX.h"
#include "stdio.h"
#include "ER-TPC050A2-2_GSL1680F_H.h"

osThreadId_t GFXTaskHandle;
void StartGFXTask(void *argument);

void InitGFXTask(void)
{

const osThreadAttr_t GFXTask_attributes = {
  .name = "GFXTask",
  .stack_size = GFXStackSize,
  .priority = (osPriority_t) osPriorityNormal,
};

	GFXTaskHandle = osThreadNew(StartGFXTask, NULL, &GFXTask_attributes);
};

void StartGFXTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
//	MX_TouchGFX_Init();
	printf("GFX Task\r\n");
	//GSL1680_Init();
  for(;;)
  {
    //MX_TouchGFX_Process();
  }
  /* USER CODE END 5 */
}
