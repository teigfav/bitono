/*
 * TaskGFX.c
 *
 *  Created on: Jul 1, 2023
 *      Author: giuli
 */
#include "cmsis_os.h"
#include "Taskctrl.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ER-TPC050A2-2_GSL1680F_H.h"

#include "Util.h"
#include "CY15B064Q.h"
#include "stm32h7xx_hal.h"
#include "main.h"
#include "adf4372.h"
#include "ad5592r.h"
#include "gui.h"

#define DAC_CONFIG  0b0101
#define ADC_CONFIG  0b0100
#define PD_REF_CTRL 0b1011
#define ADC_SEQ     0b0010
#define DAC_RESET   0b1111
#define READ_BACK   0b0111

#define AD5592_REG(x)			(x)

/* Definitions for SPI3Mutex */
osMutexId_t SPI3MutexHandle;
const osMutexAttr_t SPI3Mutex_attributes = {
  .name = "SPI3Mutex"
};

osThreadId_t ctrlTaskHandle;

osMessageQueueId_t QueueMsgHandle;
const osMessageQueueAttr_t Queue_msg_attributes = {
  .name = "Queue_msg"
};
//extern osMutexId_t SPI3MutexHandle;
struct parameters parameters;
struct adf4371_dev adf4372_dev;
extern struct fwd_ram_pwr_ctrl fwd_ram_pwr_ctrl;

bool freq_update=true;
bool power_update=true;
uint64_t freqcounter=0;
double powercounter=0;

const struct parameters default_parameters=
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
		.sint_par={
			.modo=indep,
			.freq[0]=72000000000,
			.freq[1]=73000000000,
			.freq[2]=14000000000,
			.deltatone=100000000,
			.f1_sweep_min=81000000000,
			.f1_sweep_max=85000000000,
			.sweep_step=100000000,
			.sint_on_off[0]=0,
			.sint_on_off[1]=0,
			.sint_on_off[2]=0,
		},
		.settings={
			.power[0]=0,
			.power[1]=0,
			.power[2]=0,
			.sweep_pmin=0,
			.sweep_pmax=1,
			.power_sweep_step=1,
			.p_mode=auto_control,
		},
		.end_marker=0x8765432112345678
};

void StartctrlTask(void *argument);
void InitctrlTask(void)
{

const osThreadAttr_t ctrlTask_attributes = {
  .name = "ctrlTask",
  .stack_size = ctrlStackSize,
  .priority = (osPriority_t) osPriorityNormal,
};
	QueueMsgHandle = osMessageQueueNew (16, sizeof(struct msg_t), &Queue_msg_attributes);
	ctrlTaskHandle = osThreadNew(StartctrlTask, NULL, &ctrlTask_attributes);
};

void StartctrlTask(void *argument)
{
	osStatus_t status;
	struct msg_t messaggio;
	uint64_t counter=0;
	uint32_t att;
	uint32_t pa;
	//Init Bias
	osDelay(200);  //era 2000
	Load_parameters();
	osDelay(1);
	DAC_setup();
	bool first=false;	//serve per caricare i DAC all'accensione, poi diventa true
	//Init sint
	osDelay(1000);
	LOG_DBG("PLL init");
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
	LOG_DBG("GUI init");
	init_freq_gui();
	init_power_gui();
	  for(;;)
	  {
			if(osMessageQueueGetCount(QueueMsgHandle))
			{
				LOG_DBG("elementi in coda=%u",osMessageQueueGetCount(QueueMsgHandle));
			}
			status=osMessageQueueGet(QueueMsgHandle,&messaggio,NULL,1);

		if(status==osOK)
			{
			LOG_DBG("messaggio: op=%u par1=%.2f  par2=%.2f  par3=%.2f  par4=%.2f source=%u",messaggio.op,messaggio.par1,messaggio.par2,messaggio.par3,messaggio.par4,messaggio.source);
			switch(messaggio.op)
				{
				case set_freq:
					Freq_Set(messaggio.par1,messaggio.par2,messaggio.source);
					break;
				case sint_en:
					En_Sint(messaggio.par1,messaggio.par2,messaggio.source);
					break;
				case set_fmin:
					Set_Sweep_Fmin(messaggio.par1,messaggio.source);
					break;
				case set_fmax:
					Set_Sweep_Fmax(messaggio.par1,messaggio.source);
					break;
				case set_delta_tone:
					Set_Delta_Tone(messaggio.par1,messaggio.source);
					break;
				case set_swp_time:
					Set_Sweep_Time(messaggio.par1,messaggio.source);
					break;
				case set_freq_mode:
					Set_Freq_Mode(messaggio.par1,messaggio.source);
					break;
				case get_struct:
					Get_Structure(messaggio.source);
					break;
				case set_sint_reg:
					Set_Reg(messaggio.par1,messaggio.par2,messaggio.par3,messaggio.source);
					break;
				case set_pol_value:
					Set_Pol_Value_pre(messaggio.par1,messaggio.par2,messaggio.source);
					break;
				case set_type_min_max_pol:
					Set_Type_Min_Max_Pol(messaggio.par1,messaggio.par2,messaggio.par3,messaggio.par4,messaggio.source);
					break;
				case get_pol_struct:
					Get_Pol_Struct(messaggio.par1,messaggio.source);
					break;
				case set_power:
					Set_Power(messaggio.par1,messaggio.par2,messaggio.source);
					break;
				case set_pmin:
					Set_Sweep_Pmin(messaggio.par1,messaggio.source);
					break;
				case set_pmax:
					Set_Sweep_Pmax(messaggio.par1,messaggio.source);
					break;
				case set_pow_mode:
					Set_Power_Mode(messaggio.par1,messaggio.source);
					break;
				case get_struct_power:
					Get_Struct_Power(messaggio.source);
					break;
				case save_parameters:
					Save_parameters();
					break;
				case load_parameters:
					Load_parameters();
					break;
				case set_sweep_power:
					Set_Sweep_Power(messaggio.par1,messaggio.source);
					break;
				default:
					LOG_DBG("Wrong opto code");
				}
			print_k("done");
			}
		if(parameters.sint_par.modo==indep)
		{
			if(freq_update)
			{
				if(parameters.sint_par.sint_on_off[0]) set_frequency(0);
				if(parameters.sint_par.sint_on_off[1]) set_frequency(1);
				if(parameters.sint_par.sint_on_off[2]) set_frequency(2);
				freq_update=false;
				power_update=true;
			}
			if(parameters.settings.p_mode==auto_control && power_update)
			{
				set_pwr_tot(0,parameters.settings.power[0],parameters.sint_par.freq[0],&att,&pa);
				Set_Pol_Value(8,att,remote);
				Set_Pol_Value(0,pa,remote);
				LOG_DBG("Set Power(0) =%.2f Freq=%11.0lf ATT=0X%04X PA=0X%04X",parameters.settings.power[0],(double)parameters.sint_par.freq[0],att,pa);
				set_pwr_tot(1,parameters.settings.power[1],parameters.sint_par.freq[1],&att,&pa);
				Set_Pol_Value(9,att,remote);
				Set_Pol_Value(1,pa,remote);
				LOG_DBG("Set Power(1)=%.2f Freq=%11.0lf ATT=0X%04X PA=0X%04X",parameters.settings.power[1],(double)parameters.sint_par.freq[1],att,pa);
				set_pwr_tot(2,parameters.settings.power[2],parameters.sint_par.freq[2],&att,&pa);
				Set_Pol_Value(10,att,remote);
				LOG_DBG("Set Power(2)=%.2f Freq=%11.0lf ATT=0X%04X",parameters.settings.power[2],(double)parameters.sint_par.freq[2],att);
				power_update=false;
			}
		}
		else if(parameters.sint_par.modo==sweepsingle)
		{
			if (!(counter % 4))
			{
				adf4371_pll_set_freq(&adf4372_dev,freqcounter,0);
				set_pwr_tot(0,parameters.settings.power[0],freqcounter,&att,&pa);
				Set_Pol_Value(8,att,remote);
				Set_Pol_Value(0,pa,remote);
				LOG_DBG("Set Power(0) =%.2f Freq=%11.0lf ATT=0X%04X PA=0X%04X",parameters.settings.power[0],(double)freqcounter,att,pa);
				freqcounter=freqcounter+parameters.sint_par.sweep_step;
			}
			counter=counter+1;
			if(freqcounter>=parameters.sint_par.f1_sweep_max)
			{
				freqcounter=parameters.sint_par.f1_sweep_min;
				counter=0;
			}

		}
		else if(parameters.sint_par.modo==tone1_track_tone2)
		{
			if(freq_update)
			{  //ancora da scrivere per ora c'è solo questo
				parameters.sint_par.freq[1]=parameters.sint_par.freq[0]+parameters.sint_par.deltatone;
				if(parameters.sint_par.sint_on_off[0]) set_frequency(0);
				if(parameters.sint_par.sint_on_off[1]) set_frequency(1);
				if(parameters.sint_par.sint_on_off[2]) set_frequency(2);
				freq_update=false;
				power_update=true;
			}
			if(parameters.settings.p_mode==auto_control && power_update)
			{
				set_pwr_tot(0,parameters.settings.power[0],parameters.sint_par.freq[0],&att,&pa);
				Set_Pol_Value(8,att,remote);
				Set_Pol_Value(0,pa,remote);
				LOG_DBG("Set Power(0) =%.2f Freq=%11.0lf ATT=0X%04X PA=0X%04X",parameters.settings.power[0],(double)parameters.sint_par.freq[0],att,pa);
				set_pwr_tot(1,parameters.settings.power[1],parameters.sint_par.freq[1],&att,&pa);
				Set_Pol_Value(9,att,remote);
				Set_Pol_Value(1,pa,remote);
				LOG_DBG("Set Power(1)=%.2f Freq=%11.0lf ATT=0X%04X PA=0X%04X",parameters.settings.power[1],(double)parameters.sint_par.freq[1],att,pa);
				set_pwr_tot(2,parameters.settings.power[2],parameters.sint_par.freq[2],&att,&pa);
				Set_Pol_Value(10,att,remote);
				LOG_DBG("Set Power(2)=%.2f Freq=%11.0lf ATT=0X%04X",parameters.settings.power[2],(double)parameters.sint_par.freq[2],att);
				power_update=false;
			}
		}
		else if(parameters.sint_par.modo==sweeppower)
		{
			if (!(counter % 4))
			{
				set_pwr_tot(0,powercounter,parameters.sint_par.freq[0],&att,&pa);
				Set_Pol_Value(8,att,remote);
				Set_Pol_Value(0,pa,remote);
				LOG_DBG("Set Power(0) =%.2f Freq=%11.0lf ATT=0X%04X PA=0X%04X",powercounter,(double)parameters.sint_par.freq[0],att,pa);
				powercounter=powercounter+parameters.settings.power_sweep_step;
			}
			counter=counter+1;
			if(powercounter>=parameters.settings.sweep_pmax)
			{
				powercounter=parameters.settings.sweep_pmin;
				counter=0;
			}
		}
		if(!first)
			{
			Init_DAC_SINT();
			first=true;
			}
		osDelay(500);
//			  if(memcmp(&default_parameters,&parameters,sizeof(struct parameters))!=0)
//			  {
//				  print_k("wrong");
//			  }
//			  else
//			  {
//				  print_k("correct");
//			  }
//			  Load_parameters();
	  }
}

void set_frequency (uint8_t index)
{
	adf4371_pll_set_freq(&adf4372_dev,parameters.sint_par.freq[index],index);
}

void Init_DAC_SINT(void)
{
	osMutexAcquire(SPI3MutexHandle,osWaitForever);
	LOG_DBG("First DAC writing");
	dac_write(IC27,DAC_6,parameters.polarizzazione[0].valore);
	dac_write(IC27,DAC_7,parameters.polarizzazione[1].valore);
	dac_write(IC28,DAC_5,parameters.polarizzazione[2].valore);
	dac_write(IC28,DAC_6,parameters.polarizzazione[3].valore);
	dac_write(IC28,DAC_7,parameters.polarizzazione[4].valore);
	dac_write(IC31,DAC_5,parameters.polarizzazione[5].valore);
	dac_write(IC31,DAC_6,parameters.polarizzazione[6].valore);
	dac_write(IC31,DAC_7,parameters.polarizzazione[7].valore);
	dac_write(IC27,DAC_4,parameters.polarizzazione[8].valore);
	dac_write(IC27,DAC_5,parameters.polarizzazione[9].valore);
	osMutexRelease(SPI3MutexHandle);
	Load_pwr_table();
	adf4372_dev.channel=ADF4371_CH_RF16;
	adf4372_dev.fpfd=100000000;
	adf4372_dev.ref_div_factor=1;
	adf4371_pll_set_freq(&adf4372_dev,parameters.sint_par.freq[0],Sint1);
	osDelay(10);
	adf4371_pll_set_freq(&adf4372_dev,parameters.sint_par.freq[1],Sint2);
	osDelay(10);
	adf4371_pll_set_freq(&adf4372_dev,parameters.sint_par.freq[2],Sint3);
	if(!parameters.sint_par.sint_on_off[0])
	  HAL_GPIO_WritePin(CE1_GPIO_Port,CE1_Pin,GPIO_PIN_RESET);
	if(!parameters.sint_par.sint_on_off[1])
	  HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin,GPIO_PIN_RESET);
	if(!parameters.sint_par.sint_on_off[2])
	  HAL_GPIO_WritePin(CE3_GPIO_Port,CE3_Pin,GPIO_PIN_RESET);
}

void Set_Power(double sint,double power,source_t source)
{
	uint8_t index=(uint8_t)sint;
	if (power<=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index].pmax && power>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index].pmin)
	{
	parameters.settings.power[index]=power;
	power_update=true;
	if(source!=gui) init_power_gui(); //serve per evitare che si alluppi con la gui che manda il messaggio e questo aggiorna le freq nella gui etc...
	}
	else
	{
		LOG_DBG("Warning: Power out of calibrated range.");
	}
}

void Set_Sweep_Pmin(double power,source_t source)
{
	uint8_t index=0;  //lo sweep di potenza si fa solo sul tono 0
	if (power<=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index].pmax && power>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index].pmin)
	{
	parameters.settings.sweep_pmin=power;
	}
	else
	{
		LOG_DBG("Warning: Power out of calibrated range.");
	}
}

void Set_Sweep_Pmax(double power,source_t source)
{
	uint8_t index=0;
	if (power<=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index].pmax && power>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index].pmin)
	{
	parameters.settings.sweep_pmax=power;
	}
	else
	{
		LOG_DBG("Warning: Power out of calibrated range.");
	}
}

void Set_Power_Mode(double power_mode,source_t source)
{
	power_mode_t modo =(power_mode_t) power_mode;
	if (modo<end_power_mode)
		  {
			  LOG_DBG("Set sint mode %u",modo);
			  parameters.settings.p_mode=modo;
			  if(source!=gui) init_freq_gui(); //serve per evitare che si alluppi con la gui che manda il messaggio e questo aggiorna le freq nella gui etc...
		  }
		  else
		  {
			  LOG_DBG("Power mode = %u higher than %u",modo,end_power_mode);
		  }
}
void Set_Pol_Value_pre(double pol_index,double value,source_t source)
{
	uint8_t index=(uint8_t)pol_index;
	uint32_t valore=(uint32_t)value;
	if(parameters.settings.p_mode==no_control)
	{
		if(index<11 && valore>=parameters.polarizzazione[index].min && valore<=parameters.polarizzazione[index].max)
		{
			Set_Pol_Value(index,valore,source);
		}
		else
		{
			LOG_DBG("Value out of range or wrong bias index");
		}
	}
	else
	{
		LOG_DBG("Before setting bias value, power mode 0 must be set");
	}
}

void Set_Pol_Value(uint8_t index,uint32_t valore,source_t source)
{
//	uint8_t index=(uint8_t)pol_index;
//	uint32_t valore=(uint32_t)value;
	parameters.polarizzazione[index].valore=valore;
	switch(index)
	  {
	  case 0:
		  dac_write(IC27,DAC_6,parameters.polarizzazione[index].valore);
		  break;
	  case 1:
		  dac_write(IC27,DAC_7,parameters.polarizzazione[index].valore);
		  break;
	  case 2:
		  dac_write(IC28,DAC_5,parameters.polarizzazione[index].valore);
		  break;
	  case 3:
		  dac_write(IC28,DAC_6,parameters.polarizzazione[index].valore);
		  break;
	  case 4:
		  dac_write(IC28,DAC_7,parameters.polarizzazione[index].valore);
		  break;
	  case 5:
		  dac_write(IC31,DAC_5,parameters.polarizzazione[index].valore);
		  break;
	  case 6:
		  dac_write(IC31,DAC_6,parameters.polarizzazione[index].valore);
		  break;
	  case 7:
		  dac_write(IC31,DAC_7,parameters.polarizzazione[index].valore);
		  break;
	  case 8:
		  dac_write(IC27,DAC_5,parameters.polarizzazione[index].valore);
		  break;
	  case 9:
		  dac_write(IC27,DAC_4,parameters.polarizzazione[index].valore);
		  break;
	  case 10:
		  //inserire scrittura attenuatore seriale caten LO
		  break;
	  default:
		  break;
	  }
}

void Set_Type_Min_Max_Pol(double pol_index,double type,double min,double max,source_t source)
{
	uint8_t index=(uint8_t)pol_index;
	uint32_t tipo=(uint8_t)type;
	uint32_t minimo=(uint32_t)min;
	uint32_t massimo=(uint32_t)max;
	if(pol_index<11 && minimo>=0 && minimo<=0xFFF && massimo>=0 && massimo<=0xFFF && tipo<end_bias_type)
	{
	parameters.polarizzazione[index].index=index;
	parameters.polarizzazione[index].tipo=tipo;
	parameters.polarizzazione[index].min=minimo;
	parameters.polarizzazione[index].max=massimo;
	}
	else
	{
		LOG_DBG("Value out of range or wrong bias index");
	}
}

void Get_Pol_Struct(double pol_index,source_t source)
{
	uint8_t index=(uint8_t)pol_index;
	uint32_t ADC=0;
	if(pol_index<11)
		{
		switch (index)
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
				  ADC=parameters.polarizzazione[8].valore;	//att_1
				  break;
			  case 9:
				  ADC=parameters.polarizzazione[9].valore;	//att_2
				  break;
			  case 10:
				  ADC=parameters.polarizzazione[10].valore;	//att_3
				  break;
			  default:
				  break;
			  }
		  print_k("\r\nBias OP= %d",parameters.polarizzazione[index].op);
		  print_k("Bias Index= %d",parameters.polarizzazione[index].index);
		  print_k("Bias Type= %d",parameters.polarizzazione[index].tipo);
//			  print_k("Bias Set Value= %3.2f  (0x%04lX)",DAC_to_volt(ram_bias.polarizzazione[messaggio.index].valore,13,10),ram_bias.polarizzazione[messaggio.index].valore); // esempio per tornare alla vgate
		  print_k("Bias Set Value= 0x%04lX",parameters.polarizzazione[index].valore);
		  print_k("Bias Max Value= 0x%04lX",parameters.polarizzazione[index].max);
		  print_k("Bias Min Value= 0x%04lX",parameters.polarizzazione[index].min);
		  if(index<8)
		  {
			  //print_k("Bias Current value= %3.2f  (0x%04lX)\r\n",DAC_to_amp(ADC,0.1,4.7,0),ADC);
			  print_k("Bias Current value= 0x%04lX",ADC);
		  }
		  else if (index==8 || index==9 || index==10)  //controllo attenuatori per cui nessuna lettura di corrente
		  {
			  print_k("Bias Current value= NA");
		  }
	}
	else
	{
		LOG_DBG("wrong bias index");
	}
	print_k("done");
}

void Set_Freq_Mode(double f_mode,source_t source)
{
	sint_mode_t modo=(sint_mode_t)f_mode;
	if (modo<end_sint_mode)
		  {
			  LOG_DBG("Set sint mode %u",modo);
			  if(modo==indep && parameters.sint_par.f1_sweep_max>parameters.sint_par.f1_sweep_min && parameters.sint_par.sweep_step!=0)
			  {
				  parameters.sint_par.modo=modo;
			  }
			  else if (modo==sweepsingle)
			  {
				  parameters.sint_par.modo=modo;
			  }
			  else if (modo==tone1_track_tone2 && (parameters.sint_par.freq[0]+parameters.sint_par.deltatone)<=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[0].fmax)  //f1 è sempre minore di f2
			  {
				  parameters.sint_par.modo=modo;
			  }
			  else if (modo==sweeppower && parameters.settings.sweep_pmax>parameters.settings.sweep_pmin)
			  {
			  	  parameters.sint_par.modo=modo;
			  }
			  else
			  {
				  LOG_DBG("set mode not successful");
			  }

			  if(source!=gui)
				  {
				  init_freq_gui(); //serve per evitare che si alluppi con la gui che manda il messaggio e questo aggiorna le freq nella gui etc...
				  init_power_gui();
				  }
		  }
		  else
		  {
			  LOG_DBG("Sint mode = %u higher than %u",modo,end_sint_mode);
		  }
	if(modo==sweepsingle)
	{
		En_Sint(1,0,remote);	//spegne sint 1
		En_Sint(2,0,remote);	//spegne sint 2
		En_Sint(0,1,remote);	//accende sint 0
		freqcounter=parameters.sint_par.f1_sweep_min;
	}
	else if(modo==indep || modo==tone1_track_tone2)
	{
		En_Sint(0,1,remote);	//accende sint 0
		En_Sint(1,1,remote);	//accende sint 1
		En_Sint(2,1,remote);	//accende sint 2
	}
	if(modo==sweeppower)
	{
		En_Sint(1,0,remote);	//spegne sint 1
		En_Sint(2,0,remote);	//spegne sint 2
		En_Sint(0,1,remote);	//accende sint 0
		powercounter=parameters.settings.sweep_pmin;
	}
}

void Set_Sweep_Time(double s_time,source_t source)
{
	uint64_t frequ=(uint64_t) s_time;
	char buffer[15];
	if (frequ<=MAX_SWEEP_STEP && frequ>0 && frequ<(parameters.sint_par.f1_sweep_max-parameters.sint_par.f1_sweep_min))
		  {
			  LOG_DBG("Set sweep step");
			  parameters.sint_par.sweep_step=frequ;
			  if(source!=gui) init_freq_gui(); //serve per evitare che si alluppi con la gui che manda il messaggio e questo aggiorna le freq nella gui etc...
		  }
		  else
		  {
			  snprintf(buffer, 15, "%06.2lf\n", MAX_SWEEP_STEP);
			 // itoa(MAX_SWEEP_STEP,buffer,10);
			  LOG_DBG("Sweep Time = %d higher than %s",s_time,buffer);
		  }
}

void Set_Sweep_Power(double s_power,source_t source)
{
	//uint64_t poweru=(uint64_t) s_power;
	char buffer[15];
	if (s_power<=MAX_SWEEP_POWER_STEP && s_power>0 && s_power<(parameters.settings.sweep_pmax-parameters.settings.sweep_pmin))
		  {
			  LOG_DBG("Set power sweep step");
			  parameters.settings.power_sweep_step=s_power;
			  if(source!=gui) init_power_gui(); //serve per evitare che si alluppi con la gui che manda il messaggio e questo aggiorna le freq nella gui etc...
		  }
		  else
		  {
			  snprintf(buffer, 15, "%d\n", MAX_SWEEP_POWER_STEP);
			 // itoa(MAX_SWEEP_STEP,buffer,10);
			  LOG_DBG("Sweep power step = %d higher than %s",s_power,buffer);
		  }
}

void Set_Delta_Tone(double dtone,source_t source)
{
	uint64_t frequ=(uint64_t) dtone;
	char buffer[15];
	if (frequ<=MAX_DELTA_TONE && frequ>0)
		  {
			  LOG_DBG("Set delta tone");
			  parameters.sint_par.deltatone=frequ;
			  if(source!=gui) init_freq_gui(); //serve per evitare che si alluppi con la gui che manda il messaggio e questo aggiorna le freq nella gui etc...
			  freq_update=true;
		  }
		  else
		  {
			  snprintf(buffer, 15, "%06.2lf\n", MAX_DELTA_TONE);
			 // itoa(MAX_SWEEP_TIME,buffer,10);
			  LOG_DBG("Frequency delta tone= %lf higher than %s",dtone,buffer);
		  }
}

void Set_Sweep_Fmax(double fmax,source_t source)
{
	uint64_t frequ=(uint64_t) fmax;
	if (frequ<=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[1].fmax && frequ>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[1].fmin)
		  {
			  LOG_DBG("Set fmax");
			  parameters.sint_par.f1_sweep_max=frequ;
			  if(source!=gui) init_freq_gui(); //serve per evitare che si alluppi con la gui che manda il messaggio e questo aggiorna le freq nella gui etc...
		  }
		  else
		  {
			  LOG_DBG("Frequency fmax= %lf out of calibrated range",fmax);
		  }
}

void Set_Sweep_Fmin(double fmin,source_t source)
{
	uint64_t frequ=(uint64_t) fmin;
	if (frequ<=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[1].fmax && frequ>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[1].fmin)
		  {
			  LOG_DBG("Set fmin");
			  parameters.sint_par.f1_sweep_min=frequ;
			  if(source!=gui) init_freq_gui(); //serve per evitare che si alluppi con la gui che manda il messaggio e questo aggiorna le freq nella gui etc...
		  }
		  else
		  {
			  LOG_DBG("Frequency fmin= %lf out of calibrated range",fmin);
		  }
}

void Set_Reg(double sint,double addr,double value,source_t source)
{
	uint8_t index=(uint8_t)sint;
	uint16_t ind=(uint16_t) addr;
	uint8_t valore=(uint8_t) value;
	adf4371_write(ind,valore,index);
}

void Freq_Set(double sint,double freq,source_t source)
{
	uint8_t index=(uint8_t)sint;
	uint64_t frequ=(uint64_t) freq;
	  if (frequ<=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index].fmax && frequ>=fwd_ram_pwr_ctrl.fwd_pwr_ctrl[index].fmin)
	  {
		  LOG_DBG("Set f1");
		  parameters.sint_par.freq[index]=frequ;
		  freq_update=true;
		  //adf4371_pll_set_freq(&adf4372_dev,parameters.sint_par.freq[index],index);
		  if(source!=gui) init_freq_gui(); //serve per evitare che si alluppi con la gui che manda il messaggio e questo aggiorna le freq nella gui etc...
	  }
	  else
	  {
		  LOG_DBG("Frequency Sint %u out of calibrated range",index);
	  }
}

void En_Sint(double sint,double onoff,source_t source)
{
	uint8_t index=(uint8_t)sint;
	on_off_t stato=(on_off_t)onoff;
	if(stato==s_ON)
	{
		if(index==0)
		{
			HAL_GPIO_WritePin(CE1_GPIO_Port,CE1_Pin,GPIO_PIN_SET);
		}
		else if(index==1)
		{
			HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin,GPIO_PIN_SET);
		}
		else if(index==2)
		{
			HAL_GPIO_WritePin(CE3_GPIO_Port,CE3_Pin,GPIO_PIN_SET);
		}
		else
		{
			LOG_DBG("Wrong sint number");
			return;
		}
		osDelay(1000);
		adf4371_pll_init(index);
		osDelay(10);
		adf4371_setup(&adf4372_dev,index);
		osDelay(10);
		adf4371_pll_set_freq(&adf4372_dev,parameters.sint_par.freq[index],index);
	}
	else if(stato==s_OFF)
	{
		if(index==0)
		{
			HAL_GPIO_WritePin(CE1_GPIO_Port,CE1_Pin,GPIO_PIN_RESET);
		}
		else if(index==1)
		{
			HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin,GPIO_PIN_RESET);
		}
		else if(index==2)
		{
			HAL_GPIO_WritePin(CE3_GPIO_Port,CE3_Pin,GPIO_PIN_RESET);
		}
		else
		{
			LOG_DBG("Wrong sint number");
			return;
		}

	}
	parameters.sint_par.sint_on_off[index]=stato;
	if(source!=gui) init_power_gui(); //serve per evitare che si alluppi con la gui che manda il messaggio e questo aggiorna le freq nella gui etc...
}

void Get_Structure(source_t source)
{
		  print_k("Sint mode=%d",parameters.sint_par.modo);
		  print_k("Sint 0=%11.0lf",(double)parameters.sint_par.freq[0]);
		  print_k("Sint 1=%11.0lf",(double)parameters.sint_par.freq[1]);
		  print_k("Sint 2=%11.0lf",(double)parameters.sint_par.freq[2]);
		  print_k("Sint delta tone=%10.0lf",(double)parameters.sint_par.deltatone);
		  print_k("Sint f1 sweep min=%11.0lf",(double)parameters.sint_par.f1_sweep_min);
		  print_k("Sint f1 sweep max=%11.0lf",(double)parameters.sint_par.f1_sweep_max);
		  print_k("Sint 0 on=%d",parameters.sint_par.sint_on_off[0]);
		  print_k("Sint 1 on=%d",parameters.sint_par.sint_on_off[1]);
		  print_k("Sint 2 on=%d",parameters.sint_par.sint_on_off[2]);
		  print_k("Sweep step=%.0lf",(double)parameters.sint_par.sweep_step);
		  print_k("Power mode=%d",parameters.settings.p_mode);
		  print_k("Power 0=%+06.2lf dBm",parameters.settings.power[0]);
		  print_k("Power 1=%+06.2lf dBm",parameters.settings.power[1]);
		  print_k("Power 2=%+06.2lf dBm",parameters.settings.power[2]);
		  print_k("Power f1 sweep min=%+06.2lf dBm",parameters.settings.sweep_pmin);
		  print_k("Power f1 sweep max=%+06.2lf dBm",parameters.settings.sweep_pmax);
}

void Get_Struct_Power(source_t source)
{
	  print_k("Power mode=%u",parameters.settings.p_mode);
	  print_k("Power 0=%.2f",(double)parameters.settings.power[0]);
	  print_k("Power 1=%.2f",(double)parameters.settings.power[1]);
	  print_k("Power 2=%.2f",(double)parameters.settings.power[2]);
	  print_k("Power min for sweep=%.2f",(double)parameters.settings.sweep_pmin);
	  print_k("Power max for sweep=%.2f",(double)parameters.settings.sweep_pmax);
	  print_k("Power sweep step=%.2f",(double)parameters.settings.power_sweep_step);
	  print_k("done");
}

void Load_parameters(void)
  {
  	uint32_t base_addr=0;
  	uint8_t* pfram;
  	osMutexAcquire(SPI3MutexHandle,osWaitForever);
  	config_for_save_SPI3();
  	LOG_DBG("Loading parameters");
  	osDelay(10);
  //	ReadByteFram(BIAS_FRAM_ADDR,(uint8_t*)&fram_bias, 176/*(uint32_t)sizeof(fram_bias)*/);
  	base_addr=PARAM_FRAM_ADDR;
  	pfram=(uint8_t*)&parameters;
  	LOG_DBG("Parameters base address 0x%04lX",base_addr);
  	while(base_addr<PARAM_FRAM_ADDR+sizeof(parameters))
  	{
  		ReadByteFram(base_addr,pfram, (uint8_t)1);
  		base_addr++;
  		pfram++;
  	}
  	osDelay(10);
  	config_for_dac_SPI3();
  	osMutexRelease(SPI3MutexHandle);
  //	ReadByteFram(BIAS_FRAM_ADDR,(uint8_t*)&fram_bias, (uint32_t)sizeof(fram_bias));
  	if (parameters.start_marker!=0x1234567887654321 || parameters.end_marker!=0x8765432112345678)
  	{
  		LOG_DBG("Load default parameters");
  		parameters=default_parameters;
  		Save_parameters();
  	}
  	LOG_DBG("parameters loaded");
  //verifica il marker e se non c'è carica i default in "bias"
  }

void Save_parameters(void)
  {
	osMutexAcquire(SPI3MutexHandle,osWaitForever);
  	config_for_save_SPI3();
  	LOG_DBG("Save parameters");
  	osDelay(10);
  	WriteByteFram(PARAM_FRAM_ADDR,(uint8_t*)&parameters, sizeof(parameters));
  	osDelay(10);
  	config_for_dac_SPI3();
  	osMutexRelease(SPI3MutexHandle);
  }
  /* USER CODE END 5 */

