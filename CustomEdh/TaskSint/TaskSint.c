/*
 * TaskSint.c
 *
 *  Created on: Jul 27, 2023
 *      Author: giuli
 */
#include "cmsis_os.h"
#include "TaskSint.h"
#include "stdio.h"
#include "Util.h"
#include "adf4372.h"
#include "main.h"
#include <stdbool.h>
#include "CY15B064Q.h"
#include "string.h"
#include "stdlib.h"

osThreadId_t SintTaskHandle;

/* Definitions for Queue_shelltoBias */
osMessageQueueId_t QueueSintHandle;
const osMessageQueueAttr_t Queue_Sint_attributes = {
  .name = "Queue_Sint"
};

/* Definitions for SPI3Mutex */
osMutexId_t SPI2MutexHandle;
extern osMutexId_t SPI3MutexHandle;
void StartSintTask(void *argument);

const osMutexAttr_t SPI2Mutex_attributes = {
  .name = "SPI2Mutex"
};
struct adf4371_dev adf4372_dev;
struct sint_fram_parameters sint_fram_parameters;
struct fwd_ram_pwr_ctrl pwr_ctrl;	//serve solo per avere una struttura da cui ricavare il sizeof nel load dei parametri

void InitSintTask(void)
{

const osThreadAttr_t SintTask_attributes = {
  .name = "SintTask",
  .stack_size = SintStackSize,
  .priority = (osPriority_t) osPriorityNormal,
};

SPI2MutexHandle = osMutexNew(&SPI2Mutex_attributes);
QueueSintHandle = osMessageQueueNew (16, sizeof(struct sint_msg_t), &Queue_Sint_attributes);
SintTaskHandle = osThreadNew(StartSintTask, NULL, &SintTask_attributes);
};

void StartSintTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	osStatus_t status;
	struct sint_msg_t messaggio;
	osDelay(1000);
	LOG_DBG("PLL init \r\n");
	HAL_GPIO_WritePin(CE1_GPIO_Port,CE1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CE3_GPIO_Port,CE3_Pin,GPIO_PIN_SET);
	osDelay(1000);
	osDelay(10);
	adf4371_pll_init(Sint1);
	osDelay(10);
	adf4371_pll_init(Sint2);
	osDelay(10);
	adf4371_pll_init(Sint3);
	osDelay(10);
	adf4371_setup(&adf4372_dev,Sint1);
	osDelay(10);
	adf4371_setup(&adf4372_dev,Sint2);
	osDelay(10);
	adf4371_setup(&adf4372_dev,Sint3);
	osDelay(10);
	LoadSintParam();
	bool first=false;

  for(;;)
  {
	  //LOG_DBG("task sint\r\n");
	  status=osMessageQueueGet(QueueSintHandle,&messaggio,NULL,0);
	  if (status==osOK)
	  {
		  if(messaggio.op==set_f)
		  {
			  if(messaggio.sint==Sint1)
			  {
			  sint_fram_parameters.sint_par.freq_1=messaggio.value;
			  adf4371_pll_set_freq(&adf4372_dev,sint_fram_parameters.sint_par.freq_1,Sint1);
			  }
		  else if (messaggio.sint==Sint2)
			  {
				  sint_fram_parameters.sint_par.freq_2=messaggio.value;
				  adf4371_pll_set_freq(&adf4372_dev,sint_fram_parameters.sint_par.freq_2,Sint2);
			  }
		  else if(messaggio.sint==Sint3)
			  {
				  sint_fram_parameters.sint_par.freq_3=messaggio.value;
				  adf4371_pll_set_freq(&adf4372_dev,sint_fram_parameters.sint_par.freq_3,Sint3);
			  }
		  }
		  else if(messaggio.op==on_f)
		  {
			  if(messaggio.sint==Sint1)
			  {
				  HAL_GPIO_WritePin(CE1_GPIO_Port,CE1_Pin,GPIO_PIN_SET);
				  osDelay(1000);
				  adf4371_pll_init(Sint1);
				  osDelay(10);
				  adf4371_setup(&adf4372_dev,Sint1);
				  osDelay(10);
				  adf4371_pll_set_freq(&adf4372_dev,sint_fram_parameters.sint_par.freq_1,Sint1);
			  }
			  else if(messaggio.sint==Sint2)
			  {
				  HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin,GPIO_PIN_SET);
				  osDelay(1000);
				  adf4371_pll_init(Sint2);
				  osDelay(10);
				  adf4371_setup(&adf4372_dev,Sint2);
				  osDelay(10);
				  adf4371_pll_set_freq(&adf4372_dev,sint_fram_parameters.sint_par.freq_2,Sint2);
			  }
			  else if(messaggio.sint==Sint3)
			  {
				  HAL_GPIO_WritePin(CE3_GPIO_Port,CE3_Pin,GPIO_PIN_SET);
				  osDelay(1000);
				  adf4371_pll_init(Sint3);
				  osDelay(10);
				  adf4371_setup(&adf4372_dev,Sint3);
				  osDelay(10);
				  adf4371_pll_set_freq(&adf4372_dev,sint_fram_parameters.sint_par.freq_3,Sint3);
			  }
		  }
		  else if(messaggio.op==off_f)
		  {
			  if(messaggio.sint==Sint1)
			  {
				  HAL_GPIO_WritePin(CE1_GPIO_Port,CE1_Pin,GPIO_PIN_RESET);
			  }
			  else if(messaggio.sint==Sint2)
			  {
				  HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin,GPIO_PIN_RESET);
			  }
			  else if(messaggio.sint==Sint3)
			  {
				  HAL_GPIO_WritePin(CE3_GPIO_Port,CE3_Pin,GPIO_PIN_RESET);
			  }
		  }
		  else if(messaggio.op==set_f1_max)
		  {
			  sint_fram_parameters.sint_par.f1_sweep_max=messaggio.value;
		  }
		  else if(messaggio.op==set_delta_f)
		  {
			  sint_fram_parameters.sint_par.deltatone=messaggio.value;
		  }
		  else if(messaggio.op==set_sweep_time)
		  {
			  sint_fram_parameters.sint_par.sweep_time=messaggio.value;
		  }
		  else if(messaggio.op==set_mode)
		  {
			  sint_fram_parameters.sint_par.modo=messaggio.mode;
		  }
		  else if(messaggio.op==read_sint)
		  {
			  print_k("Sint operative mode=%d\r\n",sint_fram_parameters.sint_par.modo);
			  print_k("Sint f1=%lf\r\n",(double)sint_fram_parameters.sint_par.freq_1);
			  print_k("Sint f2=%lf\r\n",(double)sint_fram_parameters.sint_par.freq_2);
			  print_k("Sint f3=%lf\r\n",(double)sint_fram_parameters.sint_par.freq_3);
			  print_k("Sint delta tone=%lf\r\n",(double)sint_fram_parameters.sint_par.deltatone);
			  print_k("Sint f1 sweep max=%lf\r\n",(double)sint_fram_parameters.sint_par.f1_sweep_max);
			  print_k("Sint 1 on=%d\r\n",sint_fram_parameters.sint_par.sint1_on_off);
			  print_k("Sint 2 on=%d\r\n",sint_fram_parameters.sint_par.sint2_on_off);
			  print_k("Sint 3 on=%d\r\n",sint_fram_parameters.sint_par.sint3_on_off);
		  }
		  else if(messaggio.op==write_reg)
		  {
			  if(messaggio.sint==Sint1)
			  {
				  adf4371_write(messaggio.addr,messaggio.data,Sint1);
			  }
			  else if(messaggio.sint==Sint2)
			  {
				  adf4371_write(messaggio.addr,messaggio.data,Sint2);
			  }
			  else if(messaggio.sint==Sint3)
			  {
				  adf4371_write(messaggio.addr,messaggio.data,Sint3);
			  }
		  }
	  }
	  if(sint_fram_parameters.sint_par.modo==sweepsingle)
	  {
		  //inserire la funzione di sweep
	  }
	  else if (sint_fram_parameters.sint_par.modo==tone1_track_tone2)
	  {
		  //inserire la funzione di sweep con i due toni in tracking
	  }
	  if(!first)
	  	  {
		  adf4372_dev.channel=ADF4371_CH_RF16;
		  adf4372_dev.fpfd=100000000;
		  adf4372_dev.ref_div_factor=1;
		  adf4371_pll_set_freq(&adf4372_dev,sint_fram_parameters.sint_par.freq_1,Sint1);
		  osDelay(10);
		  adf4371_pll_set_freq(&adf4372_dev,sint_fram_parameters.sint_par.freq_2,Sint2);
		  osDelay(10);
		  adf4371_pll_set_freq(&adf4372_dev,sint_fram_parameters.sint_par.freq_3,Sint3);
		  if(!sint_fram_parameters.sint_par.sint1_on_off)
			  HAL_GPIO_WritePin(CE1_GPIO_Port,CE1_Pin,GPIO_PIN_RESET);
		  if(!sint_fram_parameters.sint_par.sint2_on_off)
			  HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin,GPIO_PIN_RESET);
		  if(!sint_fram_parameters.sint_par.sint3_on_off)
			  HAL_GPIO_WritePin(CE3_GPIO_Port,CE3_Pin,GPIO_PIN_RESET);
		  first=true;
	  	  }
	  osDelay(5);
//	  HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin,GPIO_PIN_RESET);
	  //osDelay(500);

  }
}

void SaveSintParam(void)
{
	osStatus_t status;
	status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	config_for_save_SPI3();
	uint32_t base_addr=0;
	base_addr=BIAS_FRAM_ADDR+sizeof(struct bias_parameters)+sizeof(pwr_ctrl);
	LOG_DBG("Save Sint table \r\n");
	sint_fram_parameters.start_marker=0x1234567887654321;
	sint_fram_parameters.end_marker=0x8765432112345678;
	WriteByteFram(base_addr,(uint8_t*)&sint_fram_parameters, sizeof(sint_fram_parameters));
	osDelay(10);
	config_for_dac_SPI3();
	status=osMutexRelease(SPI3MutexHandle);
}

void LoadSintParam(void)
{
		uint32_t base_addr=0;
		uint32_t paddr=0;
		uint8_t* pfram;
		osStatus_t status;
		status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
		config_for_save_SPI3();
		LOG_DBG("Load Sint Table\r\n");
		base_addr=BIAS_FRAM_ADDR+sizeof(struct bias_parameters)+sizeof(pwr_ctrl);
		print_k("Sint Param base addrerss 0x%04lX\r\n",base_addr);
		paddr=base_addr;
		pfram=(uint8_t*)&sint_fram_parameters;
		while(paddr<base_addr+sizeof(sint_fram_parameters))
		{
			ReadByteFram(paddr,pfram, (uint32_t)1);
			//print_k("address %lX byte=%02X \r\n",base_addr,*pfram);
			paddr++;
			pfram++;
		}
		config_for_dac_SPI3();
		status=osMutexRelease(SPI3MutexHandle);
	if (sint_fram_parameters.start_marker!=0x1234567887654321 || sint_fram_parameters.end_marker!=0x8765432112345678)
	{
		LOG_DBG("Load default Sint parameters \r\n");
		sint_fram_parameters.sint_par.modo=indep;
		sint_fram_parameters.sint_par.freq_1=14000000000;
		sint_fram_parameters.sint_par.freq_2=14000000000;
		sint_fram_parameters.sint_par.freq_3=14000000000;
		sint_fram_parameters.sint_par.deltatone=1000000000;
		sint_fram_parameters.sint_par.f1_sweep_max=2000000000;
		sint_fram_parameters.sint_par.sweep_time=2;
		sint_fram_parameters.sint_par.sint1_on_off=1;
		sint_fram_parameters.sint_par.sint2_on_off=1;
		sint_fram_parameters.sint_par.sint3_on_off=1;
		SaveSintParam();
	}
}
