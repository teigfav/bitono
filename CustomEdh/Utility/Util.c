/*
 * Util.c
 *
 *  Created on: Jul 16, 2023
 *      Author: giuli
 */

#include "Util.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"
#include "main.h"
#include "type.h"
#include "CY15B064Q.h"
#include "lwipopts.h"
#include "cmsis_os.h"
#include "lwip/opt.h"
#include "stdio.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "string.h"
#include "embedded_cli.h"

#define VREFDAC 2.5

extern SPI_HandleTypeDef hspi3;
extern osMessageQueueId_t QueueBiasHandle;
extern osMutexId_t SPI3MutexHandle;
struct fwd_ram_pwr_ctrl fwd_ram_pwr_ctrl;
extern struct netconn *newconn;
extern struct netconn *conn;
extern EmbeddedCli *cli;

void LOG_DBG(char *format, ...) {
   va_list args;
   uint32_t time_ms=osKernelGetTickCount();
   uint32_t h,sec,min,ms;
   char stringa[200];
   char timestamp[200];
//   stringa[0]='p';
//   stringa[1]='i';
//   memcpy(stringa,'pippo',5);
   memset(timestamp,0,200);
   memset(stringa,0,200);
   h=time_ms/3600000;
   min=(time_ms-(h*3600000))/60000;
   sec=(time_ms-(h*3600000)-(min*60000))/1000;
   ms=(time_ms-(h*3600000)-(min*60000)-sec*1000);

   printf("[%ld:%ld:%ld:%.3ld] ",h,min,sec,ms);   //con printf non esce mai sulla rete
   va_start(args, format);
//   if(osMutexAcquire(PrintMutexHandle,osWaitForever)==osOK)
//      {
//      vprintf(format, args);
   	  snprintf(timestamp,40,"[%ld:%ld:%ld:%.3ld] ",h,min,sec,ms);   //con printf non esce mai sulla rete
   	  vsprintf(stringa,format, args);
   	  strcat(timestamp,stringa);
      embeddedCliPrint(cli, timestamp);
//      osMutexRelease(PrintMutexHandle);
//      }

   va_end(args);
}
/**
  * @brief  per ora scrive sia su uart che su tcp se connesso.
  * @param  None
  * @retval None
  */
void print_k(char *format, ...) {
	char msg_ETH[200];
	va_list args;
   va_start(args, format);
//   if(osMutexAcquire(PrintMutexHandle,osWaitForever)==osOK)
//      {
	   vsprintf(msg_ETH,format, args);
   	   size_t len = strlen(msg_ETH);
   	   if (newconn!=NULL)
   	   {
   	   netconn_write(newconn, msg_ETH, len, NETCONN_COPY);  //serve per scrivere su tcpip
   	   }
   	  // vprintf(format, args);
   	   embeddedCliPrint(cli, msg_ETH);
//      osMutexRelease(PrintMutexHandle);
//      }

   va_end(args);
}

/**
  * @brief  Re-Initialize the SPI peripheral for FRAM READ/WRITE.
  * @param  None
  * @retval None
  */
void config_for_save_SPI3(void)
{

	  /* SPI3 parameter configuration*/
	  if (HAL_SPI_DeInit(&hspi3) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  hspi3.Instance = SPI3;
	  hspi3.Init.Mode = SPI_MODE_MASTER;
	  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
	  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
	  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
	  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
	  hspi3.Init.NSS = SPI_NSS_SOFT;
	  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
	  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
	  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	  hspi3.Init.CRCPolynomial = 0x0;
	  hspi3.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	  hspi3.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
	  hspi3.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
	  hspi3.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	  hspi3.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	  hspi3.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
	  hspi3.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
	  hspi3.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
	  hspi3.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
	  hspi3.Init.IOSwap = SPI_IO_SWAP_DISABLE;
	  if (HAL_SPI_Init(&hspi3) != HAL_OK)
	  {
	    Error_Handler();
	  }

}

/**
  * @brief  Re-Initialize the SPI peripheral for DAC READ/WRITE.
  * @param  None
  * @retval None
  */
void config_for_dac_SPI3(void)
{

	  /* SPI3 parameter configuration*/
	  if (HAL_SPI_DeInit(&hspi3) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  hspi3.Instance = SPI3;
	  hspi3.Init.Mode = SPI_MODE_MASTER;
	  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
	  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
	  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
	  hspi3.Init.CLKPhase = SPI_PHASE_2EDGE;
	  hspi3.Init.NSS = SPI_NSS_SOFT;
	  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
	  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
	  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	  hspi3.Init.CRCPolynomial = 0x0;
	  hspi3.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	  hspi3.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
	  hspi3.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
	  hspi3.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	  hspi3.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	  hspi3.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
	  hspi3.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
	  hspi3.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
	  hspi3.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
	  hspi3.Init.IOSwap = SPI_IO_SWAP_DISABLE;
	  if (HAL_SPI_Init(&hspi3) != HAL_OK)
	  {
	    Error_Handler();
	  }

}
/**
  * @brief  restituisce il valore da impostare nel DAC per avre una certa potenza. La potenza verrà interpolata tra due valori vicini
  * @param  type : indica se si vuole leggere un dato della tabella on uno dei due limiti upper o lower
  * @param  chain : indica a quale catena ci si riferisce se 0,1,2
  * @param  power : potenza che si vuole impostare
  * @retval valore da scrivere del DAC
  */
void read_pwr_table(uint8_t type,uint8_t chain)
{
//	uint16_t res=0;
//	uint16_t res1=0;
//	double int_index=0;
//	uint16_t m,m1;
//	uint8_t start_index,stop_index;
	int k=0;
	//print_k("Power=%fl m=%d  stop_index=%d  res=%d",power,m,stop_index,res);

	if (type==data)
	{
//		modf((power-MINPOWER)/0.5,&int_index);
//		start_index=(uint8_t)int_index;
//		stop_index=start_index+1;
//		m=(fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[stop_index][0]-fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[start_index][0])/(0.5);
//		res=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[start_index][0]+(uint16_t)((power-(start_index*0.5-40))*m);
//		m1=(fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[stop_index][1]-fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[start_index][1])/(0.5);
//		res1=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[start_index][1]+(uint16_t)((power-(start_index*0.5-40))*m1);
//		print_k("value_att=%04X  value_pa=%04X\r\n",res,res1);
		printf("Index Pwr     F0    F1     F2     F3     F4     F5     F6     F7     F8     F9     F10    F11    F12    F13    F14    F15\r\n");
		for(int j=0;j<MAX_PWR_TABLE_SIZE;j++)
			{
			printf(" %02d   ",j);
			if((fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmax-j)==(fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].threshold-1))
			{
				printf("     ");
			}
			else
			{
				printf("%3.0f  ",fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmax-k);
				k++;
			}
			for(int i=0;i<MAX_FREQ_CALIB;i++)
				{
				printf("0X%04X ",fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[j][i]);
				}
			printf("\r\n");
			}
		print_k("");

	}
	else if (type==pmax)
	{
		print_k("pmax =%lf",fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmax);
		print_k("size of double %d",sizeof(double));
	}
	else if (type==pmin)
	{
		print_k("pmin =%lf",fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmin);
	}
	else if (type==threshold)
	{
		print_k("threshold =%lf",fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].threshold);
	}
	else if (type==fminima)
	{
		print_k("fmin =%lf",fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].fmin);
	}
	else if (type==fmassima)
	{
		print_k("fmax =%lf",fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].fmax);
	}
 }

/**
  * @brief  restituisce il valore da impostare nele DAC per avre una certa potenza. La potenza verrà interpolata tra due valori vicini
  * @param  type : indica se si vuole leggere un dato della tabella on uno dei due limiti upper o lower
  * @param  chain : indica a quale catena ci si riferisce se 0,1,2
  * @param  power : potenza che si vuole impostare
  * @retval
  */
void set_pwr(uint8_t chain,double power,double freq)
{
	osStatus_t status;
	struct bias msg;
	uint16_t res_0=0;
	uint16_t res_1=0;
	uint16_t res_2=0;
	uint16_t res_3=0;
	uint16_t res_att=0;
	uint16_t res_PA=0;
	double int_index=0;
	double p1=0;
	double p2=0;
	double f1=0;
	double f2=0;
	double freq_step,fmin,fmax,ftmp;
	uint8_t start_index,stop_index,threshold_index,pmin_index,freq_start_index,freq_stop_index;

	// --------------calcolo indici frequenza--------------
	fmin=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].fmin;
	fmax=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].fmax;
	freq_step=((fmax-fmin)/15);
	if(freq>=fmax)
	{
		freq_start_index=14;
		freq_stop_index=15;
		f1=fmax-freq_step;
		f2=fmax;
	}
	else
	{
	freq_start_index=(uint8_t)((freq-fmin)/freq_step);
	freq_stop_index=freq_start_index+1;
	modf(((freq-fmin)/freq_step),&ftmp);
	f1=fmin+ftmp*freq_step;
	f2=fmin+(ftmp+1)*freq_step;
	}

	LOG_DBG("freq=%lf  f1=%lf  f2=%lf  freq_start_index=%d   freq_stop_index=%d", freq,f1,f2,freq_start_index,freq_stop_index);

	// --------------calcolo indici potenza--------------

	modf(power,&p1);
	threshold_index=(uint8_t)(fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmax-fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].threshold);

	if(power>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].threshold)
	{
		pmin_index=(uint8_t)(fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmax-fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmin);
	}
	else
	{
		pmin_index=(uint8_t)(fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmax-fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmin+1);
	}

	if(power<=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmax && power>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmin && freq>=fmin && freq<=fmax)
	{
	if(power==fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmax)
		{
		res_0=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[0][freq_start_index];
		res_1=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[threshold_index+1][freq_start_index];
		res_2=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[0][freq_stop_index];
		res_3=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[threshold_index+1][freq_stop_index];
		res_att=interpolation(freq,f1,f2,res_0,res_2);	//interpola tra le due frequenze
		res_PA=interpolation(freq,f1,f2,res_1,res_3);	//interpola tra le due frequenze
		}
	else if(power==fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmin)
		{
		res_0=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[threshold_index][freq_start_index];
		res_1=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[pmin_index][freq_start_index];
		res_2=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[threshold_index][freq_stop_index];
		res_3=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[pmin_index][freq_stop_index];
		res_att=interpolation(freq,f1,f2,res_0,res_2);	//interpola tra le due frequenze
		res_PA=interpolation(freq,f1,f2,res_1,res_3);	//interpola tra le due frequenze
		}
	else
		{
		if(power>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].threshold)
		{
			modf((power),&int_index);
			start_index=(uint8_t)(fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmax-int_index);
			stop_index=start_index-1;
			p2=p1+1;
			res_0=interpolation(power,p1, p2, fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[start_index][freq_start_index],fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[stop_index][freq_start_index]);
			res_1=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[threshold_index+1][freq_start_index];
			res_2=interpolation(power,p1, p2, fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[start_index][freq_stop_index],fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[stop_index][freq_stop_index]);
			res_3=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[threshold_index+1][freq_stop_index];
			res_att=interpolation(freq,f1,f2,res_0,res_2);	//interpola tra le due frequenze
			res_PA=interpolation(freq,f1,f2,res_1,res_3);	//interpola tra le due frequenze
		}
		else
		{
			modf((fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmax-power),&int_index);
			start_index=(uint8_t)int_index+1;
			stop_index=start_index+1;
			p2=p1-1;
			res_0=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[threshold_index][freq_start_index];
			res_1=interpolation(power,p1, p2, fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[start_index][freq_start_index],fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[stop_index][freq_start_index]);
			res_2=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[threshold_index][freq_stop_index];
			res_3=interpolation(power,p1, p2, fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[start_index][freq_stop_index],fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[stop_index][freq_stop_index]);
			res_att=interpolation(freq,f1,f2,res_0,res_2);	//interpola tra le due frequenze
			res_PA=interpolation(freq,f1,f2,res_1,res_3);	//interpola tra le due frequenze
		}
		}
	LOG_DBG("Power=%.2f p1=%.2f p2=%.2f start_index=%d stop_index=%d threshold_index=%d pmin_index=%d",power,p1,p2,start_index,stop_index,threshold_index,pmin_index);
	LOG_DBG("res_0=0X%04X res_1=0X%04X res_2=0X%04X res_3=0X%04X res_att=0X%04X res_PA=0X%04X",res_0,res_1,res_2,res_3,res_att,res_PA);
	msg.op=setpower;
	msg.tipo=0;
	msg.min=0; //don't care
	msg.max=0; //don't care
	switch(chain)
	{
	case 0:
		msg.index=8;
		msg.valore=res_att;
		status=osMessageQueuePut(QueueBiasHandle,&msg,0,0);	//messaggio per attenuatore variabile
		msg.index=0;
		msg.valore=res_PA;
		status=osMessageQueuePut(QueueBiasHandle,&msg,0,0);	//messaggio per depolarizzazione PA
		break;
	case 1:
		msg.index=9;
		msg.valore=res_att;
		status=osMessageQueuePut(QueueBiasHandle,&msg,0,0);	//messaggio per attenuatore variabile
		msg.index=1;
		msg.valore=res_PA;
		status=osMessageQueuePut(QueueBiasHandle,&msg,0,0);	//messaggio per depolarizzazione PA
		break;
	case 2:
		msg.index=10;
		msg.valore=res_att;
		status=osMessageQueuePut(QueueBiasHandle,&msg,0,0);	//messaggio per attenuatore variabile
		break;
	default:
		break;
	}

		if(status!=osOK)
		{
			print_k("Error: No queue access");
		}
	}
	else
	{
		print_k("Error: power out of range [%.2f  ->  %.2f]  or freq out of range [%f  ->  %f]",fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmin,fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmax,fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].fmin,fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].fmax);
	}
}

/**
  * @brief  scrive la pabella potenza->tensione attuatore, index va da 0 a 160 per potenze da -40dBm a 40dBm
  * @param  type indica se è da scrivere un dato o un parametro upper o lower
  * @param  chain : si riferisce alla catena interessata
  * @param  index: l'inice del dato da scrivere
  * @param  value_att: valore da scrivere in colonna 0
  * @param  value_pa: valore da scrivere in colonna 1
  * @retval
  */
void write_pwr_table(uint8_t type,uint8_t chain, uint8_t index,double value,uint8_t freq)
{
	if(type<6 && chain < NUM_PWRCTRL && index < MAX_PWR_TABLE_SIZE && freq<MAX_FREQ_CALIB)
	{
		LOG_DBG("write table");
		if (type==data && value<4095 )
		{
			fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].ctrl[index][freq]=(uint16_t)value;
		}
		else if (type==pmin)
		{
			fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmin=value;
		}
		else if (type==pmax)
		{
			fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].pmax=value;
		}
		else if (type==threshold)
		{
			fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].threshold=value;
		}
		else if (type==fminima)
		{
			fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].fmin=value;
		}
		else if (type==fmassima)
		{
			fwd_ram_pwr_ctrl.fwd_pwr_ctrl[chain].fmax=value;
		}
		else
		{
			LOG_DBG("error : write pwr table function parameters errors");
		}
	}
	else
	{
		LOG_DBG("error : write pwr table function parameters errors");
	}
}

/**
  * @brief  converte una tensione di gate in numero da impostare nel DAC
  * @param  volt : indica se è da scrivere un dato o un parametro upper o lower
  * @param  R1 : resistenza di ingresso operazionale
  * @param  R2 : resistenza di feedback dell'operazionale
  * @retval : ritorna il valore da scrivere nel DAC
  */
uint32_t volt_to_DAC(double volt, double R1, double R2)
{
	double voltage;
	double ref_volt=VREFDAC;
	uint32_t res;
	voltage=(-volt*R1/R2);
	res=(uint32_t)(4095/ref_volt*voltage);
	return res;
}

/**
  * @brief  converte in valore letto dal DAC in una tensione di gate
  * @param  dac: indica se è da scrivere un dato o un parametro upper o lower
  * @param  R1 : resistenza di ingresso operazionale
  * @param  R2 : resistenza di feedback dell'operazionale
  * @retval restituisce il valore della tensione di gate
  */
double DAC_to_volt(uint32_t dac, double R1, double R2)
{
	double res;
	res=VREFDAC/4095*dac*(-R2/R1);
return res;
}

/**
  * @brief  converte il valore letto dal ADC in corrente (A)
  * @param  dac : valore letto da ADC
  * @param  Rsens : resistenza di sensing della corrente
  * @param  R1 : resistenza di ingresso operazionale
  * @param  R2 : resistenza di feedback dell'operazionale
  * @retval restituisce il valore di corrente in ampere
  */
double DAC_to_amp(uint32_t dac, double Rsens, double R1,double R2)
{
	double res;
	res=(VREFDAC/4095*dac)/(Rsens*50*(1+R2/R1));
return res;
}

void save_pwr_table(void)
{
	osStatus_t status;
	status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	config_for_save_SPI3();
	uint32_t base_addr=0;
	base_addr=BIAS_FRAM_ADDR+sizeof(struct bias_parameters);
	LOG_DBG("Save Pwr table");
	fwd_ram_pwr_ctrl.start_marker=0x1234567887654321;
	fwd_ram_pwr_ctrl.end_marker=0x8765432112345678;
//	WriteByteFram(PWR_TABLE_FRAM_ADDR,(uint8_t*)&fwd_ram_pwr_ctrl, sizeof(fwd_ram_pwr_ctrl));
	WriteByteFram(base_addr,(uint8_t*)&fwd_ram_pwr_ctrl, sizeof(fwd_ram_pwr_ctrl));
	osDelay(10);
	config_for_dac_SPI3();
	status=osMutexRelease(SPI3MutexHandle);
}

void Load_pwr_table(void)
{
	uint32_t base_addr=0;
	uint32_t paddr=0;
	uint8_t* pfram;
	osStatus_t status;
	status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	config_for_save_SPI3();
	LOG_DBG("Load Pwr Table");
//	base_addr=PWR_TABLE_FRAM_ADDR;
	base_addr=BIAS_FRAM_ADDR+sizeof(struct bias_parameters);
	paddr=base_addr;
	LOG_DBG("Pwr Param table base addrerss 0x%04lX",base_addr);
	pfram=(uint8_t*)&fwd_ram_pwr_ctrl;
	while(paddr<base_addr+sizeof(fwd_ram_pwr_ctrl))
	{
		ReadByteFram(paddr,pfram, (uint32_t)1);
		//print_k("address %lX byte=%02X \r\n",base_addr,*pfram);
		paddr++;
		pfram++;
	}
	config_for_dac_SPI3();
	status=osMutexRelease(SPI3MutexHandle);
//	ReadByteFram(BIAS_FRAM_ADDR,(uint8_t*)&fram_bias, (uint32_t)sizeof(fram_bias));
	if (fwd_ram_pwr_ctrl.start_marker!=0x1234567887654321 || fwd_ram_pwr_ctrl.end_marker!=0x8765432112345678)
	{
		LOG_DBG("Set default Pwr table");
		fwd_ram_pwr_ctrl.start_marker=0x1234567887654321;
		fwd_ram_pwr_ctrl.end_marker=0x8765432112345678;
		for (int i=0;i<NUM_PWRCTRL;i++)
			{
				for(int j=0;j<MAX_PWR_TABLE_SIZE;j++)
					{
					for(int k=0;k<MAX_FREQ_CALIB;k++)
						{
							fwd_ram_pwr_ctrl.fwd_pwr_ctrl[i].ctrl[j][k]=4095-j*25;  //solo per prova poi mettere tutto a zero
						}
					}
			fwd_ram_pwr_ctrl.fwd_pwr_ctrl[i].pmax=20;
			fwd_ram_pwr_ctrl.fwd_pwr_ctrl[i].pmin=-19;
			fwd_ram_pwr_ctrl.fwd_pwr_ctrl[i].threshold=2;
			if (i<2)
			{
				fwd_ram_pwr_ctrl.fwd_pwr_ctrl[i].fmax=86000000000;
				fwd_ram_pwr_ctrl.fwd_pwr_ctrl[i].fmin=71000000000;
			}
			else
			{
				fwd_ram_pwr_ctrl.fwd_pwr_ctrl[i].fmax=15000000000;
				fwd_ram_pwr_ctrl.fwd_pwr_ctrl[i].fmin=12000000000;
			}

			}
		save_pwr_table();
	}
}

uint16_t interpolation(double xp,double x0, double x1, uint16_t y0,uint16_t y1)
{
	double yp;
	uint16_t res;
	yp =(y0 + ((y1-y0)/(x1-x0)) * (xp - x0));
	res=(uint16_t)yp;
	return res;
}
