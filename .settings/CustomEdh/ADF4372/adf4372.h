/*
 * adf4372.h
 *
 *  Created on: Jul 27, 2023
 *      Author: giuli
 */

#ifndef ADF4372_ADF4372_H_
#define ADF4372_ADF4372_H_

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include <stdint.h>
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"
#include "type.h"


/******************************************************************************/
/********************** Macros and Types Declarations *************************/
/******************************************************************************/




extern SPI_HandleTypeDef hspi2;

int32_t adf4371_write( uint16_t reg, uint8_t val, enum Sint_number sint);
void adf4371_pll_init(enum Sint_number sint);
int32_t adf4371_pll_set_freq(struct adf4371_dev *dev,uint64_t frequenza,enum Sint_number sint);
int32_t adf4371_setup(struct adf4371_dev *dev,enum Sint_number sint);
int32_t adf4371_update(uint16_t reg,uint8_t mask,uint8_t val,enum Sint_number sint);
int32_t adf4371_read(uint16_t reg,uint8_t *val,enum Sint_number sint);




#endif /* ADF4372_ADF4372_H_ */
