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


#include "lwip/api.h"
#include "lwip/sys.h"
#include "string.h"
#include <ctype.h>

//#include "embedded_cli.h"
#include "stm32h7xx_hal.h"

osThreadId_t CLITaskHandle;

extern osMessageQueueId_t QueueMsgHandle;
extern osMutexId_t SPI3MutexHandle;
extern struct netconn *newconn;
extern struct netconn *conn;
extern char fw_version[];
extern char hw_version[];
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

	  CliCommandBinding on_dump_sdram={
	              "sdram_read",
	              "Read SDRAM : [offset] [lenght]",
	              true,
	              NULL,
	              onDumpsdram
	      };
	  embeddedCliAddBinding(cli,on_dump_sdram);

	  CliCommandBinding on_en_drain={
	              "en_drain",
	              "en_drain : [ON/OFF]",
	              true,
	              NULL,
	              onEnDrain
	      };
	  embeddedCliAddBinding(cli,on_en_drain);

	  CliCommandBinding on_read_version={
	              "version",
	              "Read Hardware and Firmware Version",
	              true,
	              NULL,
	              onReadVersion
	      };
	  embeddedCliAddBinding(cli,on_read_version);

	  CliCommandBinding on_cmd={
	              "cmd",
	              "Send command: cmd <op code> <par1> <par2> <par3> <par4>",
	              true,
	              NULL,
	              onCmd
	      };
	  embeddedCliAddBinding(cli,on_cmd);

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
//	print_k("Received command:");
//	print_k("command name %s",command->name);
//	print_k("\n");
    embeddedCliTokenizeArgs(command->args);
    for (int i = 1; i <= embeddedCliGetTokenCount(command->args); ++i)
    {
    	print_k("arg ");
    	print_k("%d",i);
    	print_k(": ");
    	print_k("arg: %s",embeddedCliGetToken(command->args, i));
    	//print_k("\n");
    }
}

void onDumpFram(EmbeddedCli *cli, char *args, void *context)
{
	uint16_t offset=0;
	uint32_t n_byte=0;
	uint8_t vect[4]={0};

	if(embeddedCliGetTokenCount(args)==2)
	{
		offset=strtoul(embeddedCliGetToken(args,1),NULL,0);
		n_byte=strtoul(embeddedCliGetToken(args,2),NULL,0);
		if((offset+n_byte)<(8*1024))
		{
		osMutexAcquire(SPI3MutexHandle,osWaitForever);
		config_for_save_SPI3();
		for(uint32_t i=0;i<n_byte;i=i+4)
		{
			ReadByteFram(offset+(uint16_t) i,&vect[0],4);
			print_k("%04X : %02X %02X %02X %02X",offset+(uint16_t)i,vect[0],vect[1],vect[2],vect[3]);
		}
		config_for_dac_SPI3();
		osMutexRelease(SPI3MutexHandle);
		}
	}
	else
	{
		print_k("Error: Wrong number of parameters");
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
		print_k("Error: Wrong number of parameters");
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
			print_k("Error: Wrong parameters value");
		}

	}
	else
	{
		print_k("Error: Wrong number of parameters");
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
			print_k("Error: Wrong parameters value");
		}

	}
	else
	{
		print_k("Error: Wrong number of parameters");
	}

}

void onSavePwrTable(EmbeddedCli *cli, char *args, void *context)
{
	save_pwr_table();
}

void onEnDrain(EmbeddedCli *cli, char *args, void *context)
{
	if(embeddedCliGetTokenCount(args)==1)
	{
		if(!strcmp(embeddedCliGetToken(args,1),"on"))
		{
			print_k("turn on");
			HAL_GPIO_WritePin(EN_Drain_GPIO_Port,EN_Drain_Pin,GPIO_PIN_SET);
		}
		else if(!strcmp(embeddedCliGetToken(args,1),"off"))
		{
			print_k("turn off");
			HAL_GPIO_WritePin(EN_Drain_GPIO_Port,EN_Drain_Pin,GPIO_PIN_RESET);
		}
		else
		{
			print_k("command parameter must be on or off");
		}
	}
	else
	{
		print_k("Error: Wrong number of parameters");
	}

}

void onLoadTable(EmbeddedCli *cli, char *args, void *context)
{
	Load_pwr_table();
}

void onDumpsdram(EmbeddedCli *cli, char *args, void *context)
{
	uint32_t offset=0;
	uint32_t n_byte=0;
	uint8_t vect[4]={0};
	if(embeddedCliGetTokenCount(args)==2)
	{
		offset=strtoul(embeddedCliGetToken(args,1),NULL,0);
		n_byte=strtoul(embeddedCliGetToken(args,2),NULL,0);

		for(uint32_t i=0;i<n_byte;i=i+4)
		{
			memcpy(&vect[0],(uint8_t*)(offset+i),4);
			print_k("%04X : %02X %02X %02X %02X",offset+(uint32_t)i,vect[0],vect[1],vect[2],vect[3]);
		}
	}
	else
	{
		print_k("Error: Wrong number of parameters");
	}
}
void onReadVersion(EmbeddedCli *cli, char *args, void *context)
{
	print_k("FW : %s",fw_version);
	print_k("HW : %s",hw_version);
}

void onCmd(EmbeddedCli *cli, char *args, void *context)
{
	osStatus_t status;
	struct msg_t msg;
	msg.op=0;
	msg.par1=0;
	msg.par2=0;
	msg.par3=0;
	msg.par4=0;
	if(embeddedCliGetTokenCount(args)>=1 && embeddedCliGetTokenCount(args)<6)
		{
		msg.op=(uint8_t)atoi(embeddedCliGetToken(args,1));
		if(embeddedCliGetTokenCount(args)>1)
			{
			if(isdecimal(embeddedCliGetToken(args,2)))
				{
					msg.par1=strtod(embeddedCliGetToken(args,2),NULL);
				}
			else if(isHexadecimal(embeddedCliGetToken(args,2)))
				{
					msg.par1=strtol(embeddedCliGetToken(args,2),NULL,0);
				}
			}
		if(embeddedCliGetTokenCount(args)>2)
			{
			if(isdecimal(embeddedCliGetToken(args,3)))
				{
					msg.par2=strtod(embeddedCliGetToken(args,3),NULL);
				}
			else if(isHexadecimal(embeddedCliGetToken(args,3)))
				{
					msg.par2=strtol(embeddedCliGetToken(args,3),NULL,0);
				}
			}
		if(embeddedCliGetTokenCount(args)>3)
			{
			if(isdecimal(embeddedCliGetToken(args,4)))
				{
					msg.par3=strtod(embeddedCliGetToken(args,4),NULL);
				}
			else if(isHexadecimal(embeddedCliGetToken(args,4)))
				{
					msg.par3=strtol(embeddedCliGetToken(args,4),NULL,0);
				}
			}
		if(embeddedCliGetTokenCount(args)>4)
			{
			if(isdecimal(embeddedCliGetToken(args,5)))
				{
					msg.par4=strtod(embeddedCliGetToken(args,5),NULL);
				}
			else if(isHexadecimal(embeddedCliGetToken(args,5)))
				{
					msg.par4=strtol(embeddedCliGetToken(args,5),NULL,0);
				}
			}
		msg.source=remote;
		LOG_DBG("message queue put");
		status=osMessageQueuePut(QueueMsgHandle,&msg,0,0);
		if(status!=osOK)
			{
			LOG_DBG("Error: No message queue access");
			}
	}
}

bool isHexadecimal(const char *s)
{
   int n = strlen(s);
   char ch;

   for (int i = 0; i < n; i++)
   {
	  ch=s[i];
      if (!isxdigit(ch)) {
    	  if (ch!='x' && ch!='X')
    	        {
    		  	  return false;
    	        }
      }
   }
   return true;
}

bool isdecimal(const char *s)
{
   int n = strlen(s);
   char ch;
   for (int i = 0; i < n; i++) {
	  ch=s[i];
      if (!isdigit(ch))
      {
    	  if (!ispunct(ch))
    	        {
    		  	  return false;
    	        }
      }
   }
   return true;
}

