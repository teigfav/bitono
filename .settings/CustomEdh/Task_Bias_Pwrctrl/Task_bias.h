/*
 * Task_bias.h
 *
 *  Created on: 8 lug 2023
 *      Author: giuli
 */

#ifndef TASK_BIAS_PWRCTRL_TASK_BIAS_H_
#define TASK_BIAS_PWRCTRL_TASK_BIAS_H_



#define BiasStackSize 1024*4

void InitBiasTask(void);
void config_for_save_SPI3(void);
void config_for_dac_SPI3(void);
void SaveBiasParam(void);


#endif /* TASK_BIAS_PWRCTRL_TASK_BIAS_H_ */
