/*
 * ad5592r.h
 *
 *  Created on: 9 lug 2023
 *      Author: giuli
 */

#ifndef AD5592R_AD5592R_H_
#define AD5592R_AD5592R_H_

#include "type.h"

int32_t DAC_setup (void);
int32_t dac_write(dac_t ic,uint32_t reg, uint32_t value);
int32_t adc_read(dac_t ic,uint32_t reg);

#endif /* AD5592R_AD5592R_H_ */
