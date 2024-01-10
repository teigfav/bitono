/*
 * TaskShell.c
 *
 *  Created on: Jul 16, 2023
 *      Author: giuli
 */

#include "cmsis_os.h"
#include "TaskCLI.h"
#include "stdio.h"
#include "Util.h"
#include "stdlib.h"
#include "type.h"
#include "CY15B064Q.h"
#include "Task_bias.h"
#include "TaskSint.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "string.h"

//#include "embedded_cli.h"
#include "stm32h7xx_hal.h"

osThreadId_t CLITaskHandle;
extern osMessageQueueId_t QueueBiasHandle;
extern osMessageQueueId_t QueueSintHandle;
extern osMutexId_t SPI3MutexHandle;
extern struct netconn *newconn;
extern struct netconn *conn;
char ETH_data[200]={0};
char *pETH_data=&ETH_data[0];
EmbeddedCli *cli;
extern UART_HandleTypeDef huart1;
// prototype declaration
void StartCLITask(void *argument);

void InitCLITask(void)
{

const osThreadAttr_t CLITask_attributes = {
  .name = "CLITask",
  .stack_size = CLIStackSize,
  .priority = (osPriority_t) osPriorityNormal,
};
CLITaskHandle = osThreadNew(StartCLITask, NULL, &CLITask_attributes);
};

void StartCLITask(void *argument)
{
  /* USER CODE BEGIN 5 */
	EmbeddedCliConfig *config = embeddedCliDefaultConfig();
 	config->cliBufferSize = CLI_BUFFER_SIZE;
	config->rxBufferSize = CLI_RX_BUFFER_SIZE;
	config->cmdBufferSize = CLI_CMD_BUFFER_SIZE;
	config->historyBufferSize = CLI_HISTORY_SIZE;
	config->maxBindingCount = CLI_BINDING_COUNT;
	cli = embeddedCliNew(config);

	  CliCommandBinding on_bias_write={
	              "bias_write",
	              "Set Bias : [Bias index] [bias type] [value] [max] [min]",
	              true,
	              NULL,
	              onBiasWrite
	      };
	  embeddedCliAddBinding(cli,on_bias_write);

	  CliCommandBinding on_bias_read={
	              "bias_read",
	              "Read Bias : [Bias index] ",
	              true,
	              NULL,
	              onBiasRead
	      };
	  embeddedCliAddBinding(cli,on_bias_read);

	  CliCommandBinding on_dump_fram={
	              "fram_read",
	              "Read FRAM : [offset] [lenght]",
	              true,
	              NULL,
	              onDumpFram
	      };
	  embeddedCliAddBinding(cli,on_dump_fram);

	  CliCommandBinding on_write_fram={
	              "fram_write",
	              "Read FRAM : [offset] [lenght] [data] ",
	              true,
	              NULL,
	              onWriteFram
	      };
	  embeddedCliAddBinding(cli,on_write_fram);

	  CliCommandBinding on_write_pwrctrl={
	              "pwrctrl_write",
	              "Write pwrctrl table : [type] [chain] [index] [value] [freq]",
	              true,
	              NULL,
	              onWritePwrCtrl
	      };
	  embeddedCliAddBinding(cli,on_write_pwrctrl);

	  CliCommandBinding on_read_pwrctrl={
	              "pwrctrl_read",
	              "Read pwrctrl table : [type] [chain]",
	              true,
	              NULL,
	              onReadPwrCtrl
	      };
	  embeddedCliAddBinding(cli,on_read_pwrctrl);

	  CliCommandBinding on_set_pwrctrl={
	              "pwr_set",
	              "set power : [chain ] [power] [freq]",
	              true,
	              NULL,
	              onSetPwr
	      };
	  embeddedCliAddBinding(cli,on_set_pwrctrl);

	  CliCommandBinding on_save_pwr_table={
	              "pwrctrl_save",
	              "Save power control table",
	              false,
	              NULL,
				  onSavePwrTable
	      };
	  embeddedCliAddBinding(cli, on_save_pwr_table );

	  CliCommandBinding on_load_pwr_table={
	              "pwrctrl_load",
	              "Load power control table",
	              false,
	              NULL,
				  onLoadTable
	      };
	  embeddedCliAddBinding(cli, on_load_pwr_table );

	  CliCommandBinding on_save_bias_table={
	              "bias_save",
	              "Save the bias configuration to FRAM",
	              false,
	              NULL,
				  onSaveBias
	      };
	  embeddedCliAddBinding(cli, on_save_bias_table );

	  CliCommandBinding on_save_sint={
	              "sint_save",
	              "Save the sint configuration to FRAM",
	              false,
	              NULL,
				  onSaveSint
	      };
	  embeddedCliAddBinding(cli, on_save_sint );

	  CliCommandBinding on_load_sint={
	              "sint_load",
	              "Load the sint configuration from FRAM",
	              false,
	              NULL,
				  onLoadSint
	      };
	  embeddedCliAddBinding(cli, on_load_sint );

	  CliCommandBinding on_set_sint={
	              "sint_set",
	              "set the sint freq and mode : <op:0-8> <mode:0-2> <sint:0-2> <value:Hz>",
	              true,
	              NULL,
				  onSetSint
	      };
	  embeddedCliAddBinding(cli, on_set_sint );

	  CliCommandBinding on_read_sint={
	              "sint_read",
	              "Read all sint parameters",
	              false,
	              NULL,
				  onReadSint
	      };
	  embeddedCliAddBinding(cli, on_read_sint );

	  CliCommandBinding on_dump_sdram={
	              "sdram_read",
	              "Read SDRAM : [offset] [lenght]",
	              true,
	              NULL,
	              onDumpsdram
	      };
	  embeddedCliAddBinding(cli,on_dump_sdram);

	cli->onCommand = onCommand;
	cli->writeChar = writeChar;
  /* Infinite loop */
  for(;;)
  {
	  embeddedCliProcess(cli);
  }
  /* USER CODE END 5 */
}

void writeChar(EmbeddedCli *embeddedCli, char c)
{
//	*pETH_data=c;
//	if(c!='\n' && c!='\r')
//	{
//		pETH_data++;
//	}
//	else
//	{
//		pETH_data=&ETH_data[0];
//		size_t len_ETH=strlen(ETH_data);
//		if (newconn!=NULL)
//		{
//			   netconn_write(newconn, pETH_data, len_ETH, NETCONN_COPY);  //serve per scrivere su tcpip
//		}
//		pETH_data=&ETH_data[0];
//	}
HAL_UART_Transmit(&huart1,(uint8_t*)&c,1,100);


}

void onCommand(EmbeddedCli *embeddedCli, CliCommand *command)
{
	print_k("Received command:");
	print_k("command name %s",command->name);
	print_k("\n");
    embeddedCliTokenizeArgs(command->args);
    for (int i = 1; i <= embeddedCliGetTokenCount(command->args); ++i)
    {
    	print_k("arg ");
    	print_k("%d",i);
    	print_k(": ");
    	print_k("arg: %s",embeddedCliGetToken(command->args, i));
    	print_k("\n");
    }
}

void onBiasWrite(EmbeddedCli *cli, char *args, void *context)
{
	osStatus_t status;
	struct bias msg;
	double valore,max,min;
	if(embeddedCliGetTokenCount(args)==5)
	{
		msg.op=write;
		msg.index=atoi(embeddedCliGetToken(args,1));
		msg.tipo=atoi(embeddedCliGetToken(args,2));
		valore=strtoul(embeddedCliGetToken(args,3),NULL,0);
		max=strtoul(embeddedCliGetToken(args,4),NULL,0);
		min=strtoul(embeddedCliGetToken(args,5),NULL,0);

		if(msg.index<10)  //solo i bias e non gli attenuatori
		{
			if(valore>=0 && min>=0 && max>=0 && valore<=max && valore>=min && valore<=0xFFF && min<=0xFFF && max<=0xFFF )
			{
				msg.valore=valore;
				msg.max=max;
				msg.min=min;
				status=osMessageQueuePut(QueueBiasHandle,&msg,0,0);
				if(status!=osOK)
				{
					print_k("Error: No queue access\r\n");
				}
			}
			else
			{
				print_k("Error: Parameters out of range\r\n");
			}

		}
		else if (msg.index==10)
		{
			if(valore>=0 && min>=0 && max>=0 && valore<64 && min<64 && max<64 && valore<=max && valore>=min)
			{
			msg.valore=valore;
			msg.max=max;
			msg.min=min;
			status=osMessageQueuePut(QueueBiasHandle,&msg,0,0);
			if(status!=osOK)
				{
				print_k("Error: No queue access\r\n");
				}
			}
			else
			{
				print_k("Error: Parameters out of range 0-63\r\n");
			}
		}
	}
	else
	{
		print_k("Error: Wrong number of parameters\r\n");
	}
}

void onBiasRead(EmbeddedCli *cli, char *args, void *context)
{
	osStatus_t status;
	struct bias msg;
	if(embeddedCliGetTokenCount(args)==1)
	{
		msg.op=read;
		msg.index=atoi(embeddedCliGetToken(args,1));
		if(msg.index<=MAX_BIAS_INDEX)
		{
		status=osMessageQueuePut(QueueBiasHandle,&msg,0,0);
			if(status!=osOK)
			{
				print_k("Error: No queue access\r\n");
			}
		}
	}
	else
	{
		print_k("Error: Wrong number of parameters\r\n");
	}
}
void onDumpFram(EmbeddedCli *cli, char *args, void *context)
{
	uint16_t offset=0;
	uint32_t n_byte=0;
	uint8_t vect[4]={0};
	osStatus_t status;
	if(embeddedCliGetTokenCount(args)==2)
	{
		offset=strtoul(embeddedCliGetToken(args,1),NULL,0);
		n_byte=strtoul(embeddedCliGetToken(args,2),NULL,0);
		if((offset+n_byte)<(8*1024))
		{
		status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
		config_for_save_SPI3();
		for(uint32_t i=0;i<n_byte;i=i+4)
		{
			ReadByteFram(offset+(uint16_t) i,&vect[0],4);
			print_k("%04X : %02X %02X %02X %02X\r\n",offset+(uint16_t)i,vect[0],vect[1],vect[2],vect[3]);
		}
		config_for_dac_SPI3();
		status=osMutexRelease(SPI3MutexHandle);
		}
	}
	else
	{
		print_k("Error: Wrong number of parameters\r\n");
	}
}
void onWriteFram(EmbeddedCli *cli, char *args, void *context)
{
	uint16_t offset=0;
	uint32_t n_byte=0;

	if(embeddedCliGetTokenCount(args)==3)
	{
		offset=(uint32_t)strtoul(embeddedCliGetToken(args,1),NULL,0);
		n_byte=(uint32_t)strtoul(embeddedCliGetToken(args,2),NULL,0);
		uint8_t value[n_byte];
		for(uint32_t i=0;i<n_byte;i++)
		{
			value[i] =(uint8_t) atoi(embeddedCliGetToken(args,3));
		}
		config_for_save_SPI3();
		WriteByteFram(offset,(uint8_t *)&value,n_byte);
		config_for_dac_SPI3();
	}
	else
	{
		print_k("Error: Wrong number of parameters\r\n");
	}
}

void onWritePwrCtrl(EmbeddedCli *cli, char *args, void *context)
{
	uint8_t type,chain,index;
	double value=0;
	double freq;
	if(embeddedCliGetTokenCount(args)==5)
	{
		type=(uint8_t)strtoul(embeddedCliGetToken(args,1),NULL,0);
		chain=(uint8_t)strtoul(embeddedCliGetToken(args,2),NULL,0);
		index=(uint8_t)strtoul(embeddedCliGetToken(args,3),NULL,0);
		value=strtod(embeddedCliGetToken(args,4),NULL);
		freq=(uint8_t)strtoul(embeddedCliGetToken(args,5),NULL,0);
		if(type<6 && chain<3 && index<MAX_PWR_TABLE_SIZE)
		{
			write_pwr_table(type,chain,index,value,freq);
		}
		else
		{
			print_k("Error: Wrong parameters value\r\n");
		}

	}
	else
	{
		print_k("Error: Wrong number of parameters\r\n");
	}

}

void onReadPwrCtrl(EmbeddedCli *cli, char *args, void *context)
{
	uint8_t type,chain;
	if(embeddedCliGetTokenCount(args)==2)
	{
		type=(uint8_t)strtoul(embeddedCliGetToken(args,1),NULL,0);
		chain=(uint8_t)strtoul(embeddedCliGetToken(args,2),NULL,0);
		if(type<6 && chain<3)
		{
			read_pwr_table(type,chain);

		}
		else
		{
			print_k("Error: Wrong parameters value\r\n");
		}

	}
	else
	{
		print_k("Error: Wrong number of parameters\r\n");
	}

}

void onSetPwr(EmbeddedCli *cli, char *args, void *context)
{

	double power;
	double freq;
	if(embeddedCliGetTokenCount(args)==3)
	{
		uint8_t chain;
		chain=atoi(embeddedCliGetToken(args,1));
		power=strtod(embeddedCliGetToken(args,2),NULL);
		freq=strtod(embeddedCliGetToken(args,3),NULL);
		if(chain<3)
			{
			set_pwr(chain,power,freq);
			}
		else
			{
			print_k("Error: Wrong parameters value\r\n");
			}
	}
	else
	{
		print_k("Error: Wrong number of parameters\r\n");
	}

}

void onSavePwrTable(EmbeddedCli *cli, char *args, void *context)
{
	save_pwr_table();
}

void onLoadTable(EmbeddedCli *cli, char *args, void *context)
{
	Load_pwr_table();
}

void onSaveBias(EmbeddedCli *cli, char *args, void *context)
{
	SaveBiasParam();
}
void onLoadSint(EmbeddedCli *cli, char *args, void *context)
{
	LoadSintParam();
}

void onSaveSint(EmbeddedCli *cli, char *args, void *context)
{
	SaveSintParam();
}

void onSetSint(EmbeddedCli *cli, char *args, void *context)
{
	struct sint_msg_t msg;
	osStatus_t status;
	if(embeddedCliGetTokenCount(args)>0)
	{
		msg.op=atoi(embeddedCliGetToken(args,1));
		if(msg.op==write_reg && embeddedCliGetTokenCount(args)==4)
		{
			msg.source=remote;
			msg.sint=atoi(embeddedCliGetToken(args,2));
			msg.addr=(uint16_t)strtoul(embeddedCliGetToken(args,3),NULL,0);
			msg.data=(uint8_t)strtoul(embeddedCliGetToken(args,4),NULL,0);
			status=osMessageQueuePut(QueueSintHandle,&msg,0,0);
		}
		else if(msg.op!=write_reg && embeddedCliGetTokenCount(args)==4)
		{
			msg.source=remote;
			msg.mode=atoi(embeddedCliGetToken(args,2));
			msg.sint=atoi(embeddedCliGetToken(args,3));
			msg.value=(uint64_t)strtod(embeddedCliGetToken(args,4),NULL);
			status=osMessageQueuePut(QueueSintHandle,&msg,0,0);
		}
		else
		{
			print_k("Error: Wrong number of parameters\r\n");
		}
	}
	if(status!=osOK)
	{
		print_k("Error: No queue access\r\n");
	}
}

void onReadSint(EmbeddedCli *cli, char *args, void *context)
{
	struct sint_msg_t msg;
	osStatus_t status;
	msg.source=remote;
	msg.op=read_sint;
	msg.mode=indep; //don't care
	msg.value=0; //don't care
	status=osMessageQueuePut(QueueSintHandle,&msg,0,0);
	if(status!=osOK)
		{
		print_k("Error: No queue access\r\n");
		}
}
void onDumpsdram(EmbeddedCli *cli, char *args, void *context)
{
	uint32_t offset=0;
	uint32_t n_byte=0;
	uint8_t vect[4]={0};
	osStatus_t status;
	if(embeddedCliGetTokenCount(args)==2)
	{
		offset=strtoul(embeddedCliGetToken(args,1),NULL,0);
		n_byte=strtoul(embeddedCliGetToken(args,2),NULL,0);

		for(uint32_t i=0;i<n_byte;i=i+4)
		{
			memcpy(&vect[0],offset+i,4);
			print_k("%04X : %02X %02X %02X %02X\r\n",offset+(uint32_t)i,vect[0],vect[1],vect[2],vect[3]);
		}
	}
	else
	{
		print_k("Error: Wrong number of parameters\r\n");
	}
}
