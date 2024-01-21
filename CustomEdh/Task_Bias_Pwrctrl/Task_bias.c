/*
 * Task_bias.c
 *
 *  Created on: 8 lug 2023
 *      Author: giuli
 */

#include "cmsis_os.h"
#include "Task_bias.h"
#include "stdio.h"
#include "Util.h"
#include "ad5592r.h"
#include "string.h"
#include "CY15B064Q.h"
#include <stdbool.h>
#include "stm32h7xx_hal.h"



#define DAC_CONFIG  0b0101
#define ADC_CONFIG  0b0100
#define PD_REF_CTRL 0b1011
#define ADC_SEQ     0b0010
#define DAC_RESET   0b1111
#define READ_BACK   0b0111

#define AD5592_REG(x)			(x)

osThreadId_t BiasTaskHandle;

/* Definitions for SPI3Mutex */
osMutexId_t SPI3MutexHandle;
const osMutexAttr_t SPI3Mutex_attributes = {
  .name = "SPI3Mutex"
};

/* Definitions for Queue_shelltoBias */
osMessageQueueId_t QueueBiasHandle;
const osMessageQueueAttr_t Queue_Bias_attributes = {
  .name = "Queue_Bias"
};

struct bias_parameters ram_bias;

static const struct bias_parameters default_bias_parameters=
{
		.start_marker=0x1234567887654321,
		.polarizzazione={
				[0]= { .index = 0, .op = 1, .tipo = 0, .valore = 0x300, .min = 0, .max = 0xFFF},
				[1]= { .index = 1, .op = 1, .tipo = 0, .valore = 0x300, .min = 0, .max = 0xFFF},
				[2]= { .index = 2, .op = 1, .tipo = 0, .valore = 0xFF0, .min = 0, .max = 0xFFF},
				[3]= { .index = 3, .op = 1, .tipo = 0, .valore = 0xFF0, .min = 0, .max = 0xFFF},
				[4]= { .index = 4, .op = 1, .tipo = 0, .valore = 0xFF0, .min = 0, .max = 0xFFF},
				[5]= { .index = 5, .op = 1, .tipo = 0, .valore = 0xFF0, .min = 0, .max = 0xFFF},
				[6]= { .index = 6, .op = 1, .tipo = 0, .valore = 0xFF0, .min = 0, .max = 0xFFF},
				[7]= { .index = 7, .op = 1, .tipo = 0, .valore = 0xFF0, .min = 0, .max = 0xFFF},
				[8]= { .index = 8, .op = 1, .tipo = 0, .valore = 0xFF0, .min = 0, .max = 0xFFF},
				[9]= { .index = 9, .op = 1, .tipo = 0, .valore = 0xFF0, .min = 0, .max = 0xFFF},
		},
		.end_marker=0x8765432112345678
};
// prototype declaration
void StartBiasTask(void *argument);
void LoadBiasParam(void);
void SaveBiasParam(void);

void InitBiasTask(void)
{

const osThreadAttr_t BiasTask_attributes = {
  .name = "BiasTask",
  .stack_size = BiasStackSize,
  .priority = (osPriority_t) osPriorityNormal,
};

QueueBiasHandle = osMessageQueueNew (16, sizeof(struct bias), &Queue_Bias_attributes);
SPI3MutexHandle = osMutexNew(&SPI3Mutex_attributes);

BiasTaskHandle = osThreadNew(StartBiasTask, NULL, &BiasTask_attributes);
};

/**
  * @brief  Function implementing the TestTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartBiasTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	osStatus_t status;
	struct bias messaggio;
	uint32_t ADC=0;
	char car;
	osDelay(200);  //era 2000
	LoadBiasParam();
	osDelay(1);
	//Load_pwr_table();
	osDelay(1);
	DAC_setup();
	bool first=false;	//serve per caricare i DAC all'accensione, poi diventa true
  /* Infinite loop */
  for(;;)
  {
//	  if(osMessageQueueGetCount( QueueBiasHandle ))
//		  LOG_DBG("NR1 %u",osMessageQueueGetCount( QueueBiasHandle ));
	  status=osMessageQueueGet(QueueBiasHandle,&messaggio,NULL,0);
	  //print_k("size of double %d",sizeof(double));
	  //uint32_t a=bias.polarizzazione[0].valore;
	  if (status==osOK)
	  {
//		  print_k("R %u %u  NR2 %u\r\n",messaggio.op,messaggio.index,osMessageQueueGetCount( QueueBiasHandle ));
		  if (messaggio.op==write || messaggio.op==setpower)
		  {
			  if(messaggio.op==write)
			  {
				  memcpy(&ram_bias.polarizzazione[messaggio.index],&messaggio,sizeof(messaggio));
			  }
			  else if (messaggio.op==setpower)
			  {
				  ram_bias.polarizzazione[messaggio.index].valore=messaggio.valore;
			  }

		  LOG_DBG("OP=%d  Bias number=%d   Bias type=%d   bias value=0X%04X    bias max=0X%04X    bias min=0X%04X",ram_bias.polarizzazione[messaggio.index].op,ram_bias.polarizzazione[messaggio.index].index, ram_bias.polarizzazione[messaggio.index].tipo, ram_bias.polarizzazione[messaggio.index].valore, ram_bias.polarizzazione[messaggio.index].max, ram_bias.polarizzazione[messaggio.index].min);
		  if(ram_bias.polarizzazione[messaggio.index].valore>=ram_bias.polarizzazione[messaggio.index].min && ram_bias.polarizzazione[messaggio.index].valore<=ram_bias.polarizzazione[messaggio.index].max)
		  {
				status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
			  switch(messaggio.index)
				  {
				  case 0:
					  dac_write(IC27,DAC_6,ram_bias.polarizzazione[messaggio.index].valore);
					  break;
				  case 1:
					  dac_write(IC27,DAC_7,ram_bias.polarizzazione[messaggio.index].valore);
					  break;
				  case 2:
					  dac_write(IC28,DAC_5,ram_bias.polarizzazione[messaggio.index].valore);
					  break;
				  case 3:
					  dac_write(IC28,DAC_6,ram_bias.polarizzazione[messaggio.index].valore);
					  break;
				  case 4:
					  dac_write(IC28,DAC_7,ram_bias.polarizzazione[messaggio.index].valore);
					  break;
				  case 5:
					  dac_write(IC31,DAC_5,ram_bias.polarizzazione[messaggio.index].valore);
					  break;
				  case 6:
					  dac_write(IC31,DAC_6,ram_bias.polarizzazione[messaggio.index].valore);
					  break;
				  case 7:
					  dac_write(IC31,DAC_7,ram_bias.polarizzazione[messaggio.index].valore);
					  break;
				  case 8:
					  dac_write(IC27,DAC_5,ram_bias.polarizzazione[messaggio.index].valore);
					  break;
				  case 9:
					  dac_write(IC27,DAC_4,ram_bias.polarizzazione[messaggio.index].valore);
					  break;
				  case 10:
					  //inserire scrittura attenuatore seriale caten LO
					  break;
				  default:
					  break;
				  }
			  status=osMutexRelease(SPI3MutexHandle);
		  }
		  else
		  {
			  print_k("bias value out of range");
		  }

		  }
		  if (messaggio.op==read)
		  {
			  status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
			  switch (messaggio.index)
			  {
			  case 0:
				  ADC=adc_read(IC28,2);		//bias_PA_1
				  break;
			  case 1:
				  ADC=adc_read(IC31,2);		//bias_PA_2
				  break;
			  case 2:
				  ADC=adc_read(IC28,4);		//x2_1
				  break;
			  case 3:
				  ADC=adc_read(IC28,8);		//x3_1
				  break;
			  case 4:
				  ADC=adc_read(IC28,16);	//amp_mult_1
				  break;
			  case 5:
				  ADC=adc_read(IC31,4);		//x2_2
				  break;
			  case 6:
				  ADC=adc_read(IC31,8);		//x3_2
				  break;
			  case 7:
				  ADC=adc_read(IC31,16);	//amp_mult_2
				  break;
			  case 8:
				  ADC=ram_bias.polarizzazione[8].valore;	//att_1
				  break;
			  case 9:
				  ADC=ram_bias.polarizzazione[9].valore;	//att_2
				  break;
			  case 10:
				  ADC=ram_bias.polarizzazione[10].valore;	//att_3
				  break;
			  default:
				  break;
			  }
			  status=osMutexRelease(SPI3MutexHandle);
			  print_k("\r\nBias OP= %d",ram_bias.polarizzazione[messaggio.index].op);
			  print_k("Bias Index= %d",ram_bias.polarizzazione[messaggio.index].index);
			  print_k("Bias Type= %d",ram_bias.polarizzazione[messaggio.index].tipo);
//			  print_k("Bias Set Value= %3.2f  (0x%04lX)",DAC_to_volt(ram_bias.polarizzazione[messaggio.index].valore,13,10),ram_bias.polarizzazione[messaggio.index].valore); // esempio per tornare alla vgate
			  print_k("Bias Set Value= 0x%04lX",ram_bias.polarizzazione[messaggio.index].valore);
			  print_k("Bias Max Value= 0x%04lX",ram_bias.polarizzazione[messaggio.index].max);
			  print_k("Bias Min Value= 0x%04lX",ram_bias.polarizzazione[messaggio.index].min);
			  if(messaggio.index<8)
			  {
				  //print_k("Bias Current value= %3.2f  (0x%04lX)\r\n",DAC_to_amp(ADC,0.1,4.7,0),ADC);
				  print_k("Bias Current value= 0x%04lX",ADC);
			  }
			  else if (messaggio.index==8 || messaggio.index==9 || messaggio.index==10)  //controllo attenuatori per cui nessuna lettura di corrente
			  {
				  print_k("Bias Current value= NA");
			  }
		  }
	  }

	  if(!first)
		  {
		  	  status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
		  	  LOG_DBG("First DAC writing");
			  dac_write(IC27,DAC_6,ram_bias.polarizzazione[0].valore);
			  dac_write(IC27,DAC_7,ram_bias.polarizzazione[1].valore);
			  dac_write(IC28,DAC_5,ram_bias.polarizzazione[2].valore);
			  dac_write(IC28,DAC_6,ram_bias.polarizzazione[3].valore);
			  dac_write(IC28,DAC_7,ram_bias.polarizzazione[4].valore);
			  dac_write(IC31,DAC_5,ram_bias.polarizzazione[5].valore);
			  dac_write(IC31,DAC_6,ram_bias.polarizzazione[6].valore);
			  dac_write(IC31,DAC_7,ram_bias.polarizzazione[7].valore);
			  dac_write(IC27,DAC_4,ram_bias.polarizzazione[8].valore);
			  dac_write(IC27,DAC_5,ram_bias.polarizzazione[9].valore);
			  status=osMutexRelease(SPI3MutexHandle);
			  Load_pwr_table();
			  first=true;
		  }
    osDelay(10);
  }
  /* USER CODE END 5 */
}

void LoadBiasParam(void)
{
	uint32_t base_addr=0;
	uint8_t* pfram;
	osStatus_t status;
	status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	config_for_save_SPI3();
	LOG_DBG("Load Bias parameters");
//	ReadByteFram(BIAS_FRAM_ADDR,(uint8_t*)&fram_bias, 176/*(uint32_t)sizeof(fram_bias)*/);
	base_addr=BIAS_FRAM_ADDR;
	pfram=(uint8_t*)&ram_bias;
	LOG_DBG("Bias Param base address 0x%04lX",base_addr);
	while(base_addr<BIAS_FRAM_ADDR+sizeof(ram_bias))
	{
		ReadByteFram(base_addr,pfram, (uint32_t)1);
		base_addr++;
		pfram++;
	}
	config_for_dac_SPI3();
	status=osMutexRelease(SPI3MutexHandle);
//	ReadByteFram(BIAS_FRAM_ADDR,(uint8_t*)&fram_bias, (uint32_t)sizeof(fram_bias));
	if (ram_bias.start_marker!=0x1234567887654321 || ram_bias.end_marker!=0x8765432112345678)
	{
		LOG_DBG("Load default Bias parameters");
		ram_bias=default_bias_parameters;
		SaveBiasParam();
	}

//verifica il marker e se non c'è carica i default in "bias"
}
void SaveBiasParam(void)
{
	osStatus_t status;
	status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	config_for_save_SPI3();
	LOG_DBG("Save Bias parameters");
	WriteByteFram(BIAS_FRAM_ADDR,(uint8_t*)&ram_bias, sizeof(ram_bias));
	osDelay(10);
	config_for_dac_SPI3();
	status=osMutexRelease(SPI3MutexHandle);
}


