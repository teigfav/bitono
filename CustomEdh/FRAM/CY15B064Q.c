#include "CY15B064Q.h"
#include "stdio.h"
#include "string.h"
#include "cmsis_os.h"
#include "main.h"

#define CY5B064Q_SPI_PORT		hspi3
#define FRAM_SIZE (8*1024)
extern SPI_HandleTypeDef CY5B064Q_SPI_PORT;
extern osMutexId_t SPI3MutexHandle;

void ReadByteFram(uint16_t offset,uint8_t * data, uint32_t num_bytes)
{

	uint8_t FLASH_buffer[4];
	uint16_t tmp_offset;
	osStatus_t status;
	//status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	//print_k("Read data FRAM %p",data);
	if(offset+num_bytes<=FRAM_SIZE)
	{
//	HAL_GPIO_WritePin(FRAM_CS_GPIO_Port,FRAM_CS_Pin,GPIO_PIN_SET);
//	osDelay(3);
	HAL_GPIO_WritePin(FRAM_CS_GPIO_Port,FRAM_CS_Pin,GPIO_PIN_RESET);
	//osDelay(2);
	FLASH_buffer[0]=FRAM_CMD_READ;
	tmp_offset=(offset & 0xFF00)>>8;
	FLASH_buffer[1]=(uint8_t)tmp_offset;
	tmp_offset=(offset & 0xFF);
	FLASH_buffer[2]=(uint8_t)tmp_offset;
	HAL_SPI_Transmit(&CY5B064Q_SPI_PORT, FLASH_buffer, 3, HAL_MAX_DELAY);
	HAL_SPI_Receive(&CY5B064Q_SPI_PORT, data, num_bytes, HAL_MAX_DELAY);
	//osDelay(2);
	HAL_GPIO_WritePin(FRAM_CS_GPIO_Port,FRAM_CS_Pin,GPIO_PIN_SET);
	//status=osMutexRelease(SPI3MutexHandle);
	}
}
void WriteByteFram(uint16_t offset,uint8_t * data, uint32_t num_bytes)
{

	uint8_t FLASH_buffer[4];
	uint16_t tmp_offset;
	osStatus_t status;
	//status=osMutexAcquire(SPI3MutexHandle,osWaitForever);
	//Enable Write	
	HAL_GPIO_WritePin(FRAM_CS_GPIO_Port,FRAM_CS_Pin,GPIO_PIN_RESET);
	osDelay(5);
	FLASH_buffer[0]=FRAM_CMD_WREN;
	HAL_SPI_Transmit(&CY5B064Q_SPI_PORT, FLASH_buffer, 1, HAL_MAX_DELAY);
	osDelay(5);
	HAL_GPIO_WritePin(FRAM_CS_GPIO_Port,FRAM_CS_Pin,GPIO_PIN_SET);
	
	
	if(offset+num_bytes<=FRAM_SIZE)
	{
	HAL_GPIO_WritePin(FRAM_CS_GPIO_Port,FRAM_CS_Pin,GPIO_PIN_SET);
	osDelay(5);
	HAL_GPIO_WritePin(FRAM_CS_GPIO_Port,FRAM_CS_Pin,GPIO_PIN_RESET);
	osDelay(5);
	FLASH_buffer[0]=FRAM_CMD_WRITE;
	tmp_offset=(offset & 0xFF00)>>8;
	FLASH_buffer[1]=(uint8_t)tmp_offset;
	tmp_offset=(offset & 0xFF);
	FLASH_buffer[2]=(uint8_t)tmp_offset;
	//print_k("fRAM Write %02X, %02X, %02X\r\n",FLASH_buffer[0],FLASH_buffer[1],FLASH_buffer[2]);
	HAL_SPI_Transmit(&CY5B064Q_SPI_PORT, FLASH_buffer, 3, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&CY5B064Q_SPI_PORT, data, num_bytes, HAL_MAX_DELAY);
	osDelay(5);
	HAL_GPIO_WritePin(FRAM_CS_GPIO_Port,FRAM_CS_Pin,GPIO_PIN_SET);
	//status=osMutexRelease(SPI3MutexHandle);
	}
}

