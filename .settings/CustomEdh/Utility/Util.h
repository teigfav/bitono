/*
 * Util.h
 *
 *  Created on: 9 lug 2023
 *      Author: giuli
 */

#ifndef UTILITY_UTIL_H_
#define UTILITY_UTIL_H_

#include "stdint.h"

void LOG_DBG(char *format, ...);
void print_k(char *format, ...);
void config_for_save_SPI3(void);
void config_for_dac_SPI3(void);
void write_pwr_table(uint8_t type,uint8_t chain,uint8_t index,double value,uint8_t freq);
void read_pwr_table(uint8_t type,uint8_t chain);
uint32_t volt_to_DAC(double volt, double R1, double R2);
double DAC_to_volt(uint32_t dac, double R1, double R2);
double DAC_to_amp(uint32_t dac, double Rsens, double R1,double R2);
void save_pwr_table(void);
void Load_pwr_table(void);
void set_pwr(uint8_t chain,double power,double freq);
uint16_t interpolation(double xp,double x0, double x1, uint16_t y0,uint16_t y1);


#endif /* UTILITY_UTIL_H_ */
