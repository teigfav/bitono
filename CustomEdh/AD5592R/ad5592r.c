/*
 * ad5592r.c
 *
 *  Created on: 9 lug 2023
 *      Author: giuli
 */
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"
#include "ad5592r.h"
#include "main.h"
#include "Util.h"
#include "stdio.h"
#include "no_os_util.h"


#define DAC_CONFIG  0b0101
#define ADC_CONFIG  0b0100
#define PD_REF_CTRL 0b1011
#define ADC_SEQ     0b0010
#define DAC_RESET   0b1111
#define READ_BACK   0b0111

#define AD5592_REG(x)			(x)

extern SPI_HandleTypeDef hspi3;
extern osMutexId_t SPI3MutexHandle;
HAL_StatusTypeDef ret;
osStatus_t status;
int32_t DAC_setup (void)
{
    uint8_t buf[2];
    // set DAC IC27
    buf[0]=(DAC_RESET<<3);
    buf[0]=buf[0] & 0x7F;
    buf[0]=buf[0] | 0x05;
    buf[1]=0xAC;    //reset
    status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
    print_k("DAC : %02X  %02X\r\n",buf[0],buf[1]);
    HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_RESET);
	ret=HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10);
	HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_SET);
	//status=osMutexRelease(SPI3MutexHandle);
	if(ret!=HAL_OK)
	{
		LOG_DBG("AD5592 SPI error\r\n");
		return HAL_ERROR;
	}
    osDelay(10);
    buf[0]=(DAC_CONFIG<<3);
    buf[0]=buf[0] & 0x7F;
    buf[1]=0xF0;    //  I/O 7,6,5,4 -> DAC
    //status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
    print_k("DAC : %02X  %02X\r\n",buf[0],buf[1]);
    HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_RESET);
    ret=HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10);
	HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_SET);
	//status=osMutexRelease(SPI3MutexHandle);
	if(ret!=HAL_OK)
	{
		LOG_DBG("AD5592 SPI error\r\n");
		return HAL_ERROR;
	}
    osDelay(1);
    buf[0]=(ADC_CONFIG<<3);
    buf[0]=buf[0] & 0x7F;
    buf[1]=0x0F;    //  I/O 0,1,2,3 -> ADC
    //status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
    print_k("DAC : %02X  %02X\r\n",buf[0],buf[1]);
    HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_RESET);
    ret=HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10);
	HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_SET);
	//status=osMutexRelease(SPI3MutexHandle);
	if(ret!=HAL_OK)
	{
		LOG_DBG("AD5592 SPI error\r\n");
		return HAL_ERROR;
	}
    osDelay(1);
    buf[0]=(PD_REF_CTRL<<3);
    buf[0]=buf[0] | 0x02; 	// SET internal reference
    buf[0]=buf[0] & 0x7A;
    buf[1]=0x00;			// all channel are operating, no power down
    //status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
    print_k("DAC : %02X  %02X\r\n",buf[0],buf[1]);
    HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_RESET);
    ret=HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 100);
	HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_SET);
	//status=osMutexRelease(SPI3MutexHandle);
	if(ret!=HAL_OK)
	{
		LOG_DBG("AD5592 SPI error\r\n");
		return HAL_ERROR;
	}

	 // set DAC IC28
	buf[0]=(DAC_RESET<<3);
	buf[0]=buf[0] & 0x7F;
	buf[0]=buf[0] | 0x05;
	buf[1]=0xAC;    //reset
	//status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	print_k("DAC : %02X  %02X\r\n",buf[0],buf[1]);
	HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_RESET);
	ret=HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10);
	HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_SET);
	//status=osMutexRelease(SPI3MutexHandle);
	if(ret!=HAL_OK)
	{
		LOG_DBG("AD5592 SPI error\r\n");
		return HAL_ERROR;
	}
	osDelay(10);
	buf[0]=(DAC_CONFIG<<3);
	buf[0]=buf[0] & 0x7F;
	buf[1]=0xE0;    //  I/O 7,6,5 -> DAC
	//status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	print_k("DAC : %02X  %02X\r\n",buf[0],buf[1]);
	HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_RESET);
	ret=HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10);
	HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_SET);
	//status=osMutexRelease(SPI3MutexHandle);
	if(ret!=HAL_OK)
	{
		LOG_DBG("AD5592 SPI error\r\n");
		return HAL_ERROR;
	}
	osDelay(1);
	buf[0]=(ADC_CONFIG<<3);
	buf[0]=buf[0] & 0x7F;
	buf[1]=0x1F;    //  I/O 0,1,2,3,4 -> ADC
	//status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	print_k("DAC : %02X  %02X\r\n",buf[0],buf[1]);
	HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_RESET);
	ret=HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10);
	HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_SET);
	//status=osMutexRelease(SPI3MutexHandle);
	if(ret!=HAL_OK)
	{
		LOG_DBG("AD5592 SPI error\r\n");
		return HAL_ERROR;
	}
	osDelay(1);
	buf[0]=(PD_REF_CTRL<<3);
	buf[0]=buf[0] | 0x02; 	// SET internal reference
	buf[0]=buf[0] & 0x7A;
	buf[1]=0x00;			// all channel are operating, no power down
	//status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	print_k("DAC : %02X  %02X\r\n",buf[0],buf[1]);
	HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_RESET);
	ret=HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10);
	HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_SET);
	//status=osMutexRelease(SPI3MutexHandle);
	if(ret!=HAL_OK)
	{
		LOG_DBG("AD5592 SPI error\r\n");
		return HAL_ERROR;
	}

	 // set DAC IC31
	buf[0]=(DAC_RESET<<3);
	buf[0]=buf[0] & 0x7F;
	buf[0]=buf[0] | 0x05;
	buf[1]=0xAC;    //reset
	//status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	print_k("DAC : %02X  %02X\r\n",buf[0],buf[1]);
	HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_RESET);
	ret=HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10);
	HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_SET);
	//status=osMutexRelease(SPI3MutexHandle);
	if(ret!=HAL_OK)
	{
		LOG_DBG("AD5592 SPI error\r\n");
		return HAL_ERROR;
	}
	osDelay(10);
	buf[0]=(DAC_CONFIG<<3);
	buf[0]=buf[0] & 0x7F;
	buf[1]=0xE0;    //  I/O 7,6,5 -> DAC
	//status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	print_k("DAC : %02X  %02X\r\n",buf[0],buf[1]);
	HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_RESET);
	ret=HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10);
	HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_SET);
	//status=osMutexRelease(SPI3MutexHandle);
	if(ret!=HAL_OK)
	{
		LOG_DBG("AD5592 SPI error\r\n");
		return HAL_ERROR;
	}
	osDelay(1);
	buf[0]=(ADC_CONFIG<<3);
	buf[0]=buf[0] & 0x7F;
	buf[1]=0x1F;    //  I/O 0,1,2,3,4 -> ADC
	//status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	print_k("DAC : %02X  %02X\r\n",buf[0],buf[1]);
	HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_RESET);
	ret=HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10);
	HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_SET);
	//status=osMutexRelease(SPI3MutexHandle);
	if(ret!=HAL_OK)
	{
		LOG_DBG("AD5592 SPI error\r\n");
		return HAL_ERROR;
	}
	osDelay(1);
	buf[0]=(PD_REF_CTRL<<3);
	buf[0]=buf[0] | 0x02; 	// SET internal reference
	buf[0]=buf[0] & 0x7A;
	buf[1]=0x00;			// all channel are operating, no power down
	//status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	print_k("DAC : %02X  %02X\r\n",buf[0],buf[1]);
	HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_RESET);
	ret=HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10);
	HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_SET);
	status=osMutexRelease(SPI3MutexHandle);
	if(ret!=HAL_OK)
	{
		LOG_DBG("AD5592 SPI erro\r\n");
		return HAL_ERROR;
	}
	//settare tutti i DAC al massimo per avere i gate al minimo.  Verificare se vala anche per i PA.
    return 0;

}

int32_t dac_write(dac_t ic,uint32_t reg, uint32_t value)
 {
    uint32_t tmp;
    uint8_t buf[2];
    buf[0]=reg<<4;
    buf[0]=buf[0] | 0x80;
    tmp=value;
    tmp=tmp>>8;
    buf[0]=buf[0] | tmp;
    buf[1]=value;
    switch (ic)
    {
    case IC27:
    	print_k("DAC IC27 : %02X  %02X\r\n",buf[0],buf[1]);
    	//status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
    	HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_RESET);
    	if(HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10)!=HAL_OK)
    	{
    		LOG_DBG("AD5592 SPI error\r\n");
    		HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_SET);
    		status=osMutexRelease(SPI3MutexHandle);
    		return HAL_ERROR;
    	}
    	HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_SET);
    	//status=osMutexRelease(SPI3MutexHandle);
        break;
    case IC28:
    	print_k("DAC IC28 : %02X  %02X\r\n",buf[0],buf[1]);
    	//status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
    	HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_RESET);
    	if(HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10)!=HAL_OK)
    	{
    		LOG_DBG("AD5592 SPI error\r\n");
    		HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_SET);
    		status=osMutexRelease(SPI3MutexHandle);
    		return HAL_ERROR;
    	}
    	HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_SET);
    	//status=osMutexRelease(SPI3MutexHandle);
        break;

    case IC31:
    	print_k("DAC IC31 : %02X  %02X\r\n",buf[0],buf[1]);
    	//status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
    	HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_RESET);
    	if(HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10)!=HAL_OK)
    	{
    		LOG_DBG("AD5592 SPI error\r\n");
    		HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_SET);
    		status=osMutexRelease(SPI3MutexHandle);
    		return HAL_ERROR;
    	}
    	HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_SET);
    	//status=osMutexRelease(SPI3MutexHandle);
        break;
    default:
    	LOG_DBG("Wrong IC number \r\n");
    	return -1;
    	break;
    }
    return 0;
 }

int32_t adc_read(dac_t ic,uint32_t reg)
{
   uint32_t result;
   uint8_t buf[2]={0};
   buf[0]=(ADC_SEQ<<3 ) & 0x78;
   buf[1]=reg;
   switch (ic)
   {
   case IC27:
	   //status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
		HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_RESET);
		if(HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10)!=HAL_OK)
		{
			LOG_DBG("AD5592 SPI error\r\n");
			result= HAL_ERROR;
		}
		HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_SET);
		osDelay(1);
	    buf[0]=0x00;
	    buf[1]=0x00;
		HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_RESET);
		if(HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10)!=HAL_OK)
		{
			LOG_DBG("AD5592 SPI error\r\n");
			result=  HAL_ERROR;
		}
		HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_SET);
		osDelay(1);
	    buf[0]=0x00;
	    buf[1]=0x00;
		HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_RESET);
		if(HAL_SPI_Receive(&hspi3, &buf[0], NO_OS_ARRAY_SIZE(buf), 10)!=HAL_OK)
		{
			LOG_DBG("AD5592 SPI error\r\n");
			result= HAL_ERROR;
		}
		HAL_GPIO_WritePin(DtoACS1_GPIO_Port,DtoACS1_Pin,GPIO_PIN_SET);
		//status=osMutexRelease(SPI3MutexHandle);
	    result=buf[0] & 0x0F;
	    result=(result<<8)+buf[1];
	   break;
   case IC28:
	   //status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
		HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_RESET);
		if(HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10)!=HAL_OK)
		{
			LOG_DBG("AD5592 SPI error\r\n");
			result= HAL_ERROR;
		}
		HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_SET);
		osDelay(1);
	    buf[0]=0x00;
	    buf[1]=0x00;
		HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_RESET);
		if(HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10)!=HAL_OK)
		{
			LOG_DBG("AD5592 SPI error\r\n");
			result=  HAL_ERROR;
		}
		HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_SET);
		osDelay(1);
	    buf[0]=0x00;
	    buf[1]=0x00;
		HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_RESET);
		if(HAL_SPI_Receive(&hspi3, &buf[0], NO_OS_ARRAY_SIZE(buf), 10)!=HAL_OK)
		{
			LOG_DBG("AD5592 SPI error\r\n");
			result= HAL_ERROR;
		}
		HAL_GPIO_WritePin(DtoACS2_GPIO_Port,DtoACS2_Pin,GPIO_PIN_SET);
		//status=osMutexRelease(SPI3MutexHandle);
	    result=buf[0] & 0x0F;
	    result=(result<<8)+buf[1];
	   break;
   case IC31:
	   //status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
		HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_RESET);
		if(HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10)!=HAL_OK)
		{
			LOG_DBG("AD5592 SPI error\r\n");
			result=  HAL_ERROR;
		}
		HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_SET);
		osDelay(1);
	    buf[0]=0x00;
	    buf[1]=0x00;
		HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_RESET);
		if(HAL_SPI_Transmit(&hspi3, buf, NO_OS_ARRAY_SIZE(buf), 10)!=HAL_OK)
		{
			LOG_DBG("AD5592 SPI error\r\n");
			result= HAL_ERROR;
		}
		HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_SET);
		osDelay(1);
	    buf[0]=0x00;
	    buf[1]=0x00;
		HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_RESET);
		if(HAL_SPI_Receive(&hspi3, &buf[0], NO_OS_ARRAY_SIZE(buf), 10)!=HAL_OK)
		{
			LOG_DBG("AD5592 SPI error\r\n");
			result= HAL_ERROR;
		}
		HAL_GPIO_WritePin(DtoACS3_GPIO_Port,DtoACS3_Pin,GPIO_PIN_SET);
		//status=osMutexRelease(SPI3MutexHandle);
	    result=buf[0] & 0x0F;
	    result=(result<<8)+buf[1];
	   break;
   default:
	   result= -1;
	   break;
   }
   return result;
}

