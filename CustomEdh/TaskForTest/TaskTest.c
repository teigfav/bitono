/*
 * TaskTest.c
 *
 *  Created on: Jun 17, 2023
 *      Author: giuli
 */
#include "cmsis_os.h"
#include "TaskTest.h"
#include "stdio.h"
#include "ER-TPC050A2-2_GSL1680F_H.h"
#include "Util.h"
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
	//uint8_t read_buf[4] = {0};
  /* Infinite loop */
  for(;;)
  {
//    print_k("Test Task running\r\n");
    //gsl_ts_read(0xB0, read_buf, 4);
	 // dataread();
    //LOG_DBG("data read %02X %02X %02X %02X\r\n",read_buf[0],read_buf[1],read_buf[2],read_buf[3]);
    osDelay(10);
  }
  /* USER CODE END 5 */
}
