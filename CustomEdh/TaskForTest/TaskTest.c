/*
 * TaskTest.c
 *
 *  Created on: Jun 17, 2023
 *      Author: giuli
 */
#include "cmsis_os.h"
#include "TaskTest.h"
#include "stdio.h"
#include "string.h"
#include "ER-TPC050A2-2_GSL1680F_H.h"
#include "Util.h"
#include "type.h"
#include "Taskctrl.h"
osThreadId_t TestTaskHandle;

// prototype declaration
void StartTestTask(void *argument);

void InitTestTask(void)
{

const osThreadAttr_t TestTask_attributes = {
  .name = "TestTask",
  .stack_size = TestStackSize,
  .priority = (osPriority_t) osPriorityNormal,
};

	TestTaskHandle = osThreadNew(StartTestTask, NULL, &TestTask_attributes);
};


/**
  * @brief  Function implementing the TestTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartTestTask(void *argument)
{
  /* USER CODE BEGIN 5 */
//	extern const struct parameters default_parameters;
//	extern struct parameters parameters;
  /* Infinite loop */
  for(;;)
  {
//    print_k("Test Task running");
    //gsl_ts_read(0xB0, read_buf, 4);
	 // dataread();
    //LOG_DBG("data read %02X %02X %02X %02X",read_buf[0],read_buf[1],read_buf[2],read_buf[3]);
//	  if(memcmp(&default_parameters,&parameters,sizeof(struct parameters))!=0)
//	  {
//		  print_k("wrong");
//	  }
//	  else
//	  {
//		  print_k("correct");
//	  }
	 // Load_parameters();
    osDelay(1000);
  }
  /* USER CODE END 5 */
}
