/*
 * TaskGFX.h
 *
 *  Created on: Jul 1, 2023
 *      Author: giuli
 */

#ifndef TASKGFX_TASKGFX_H_
#define TASKGFX_TASKGFX_H_

#include "type.h"

#define ctrlStackSize 4096
void InitctrlTask(void);
void Save_parameters(void);
void Load_parameters(void);
void Freq_Set(double sint,double freq,source_t source);
void Init_DAC_SINT(void);
void En_Sint(double sint,double onoff,source_t source);
void Set_Reg(double sint,double addr,double value,source_t source);
void Set_Sweep_Fmin(double fmin,source_t source);
void Set_Sweep_Fmax(double fmax,source_t source);
void Set_Delta_Tone(double dtone,source_t source);
void Set_Sweep_Time(double s_time,source_t source);
void Set_Freq_Mode(double f_mode,source_t source);
void Set_Power(double sint,double power,source_t source);
void Set_Sweep_Pmin(double power,source_t source);
void Set_Sweep_Pmax(double power,source_t source);
void Set_Power_Mode(double power_mode,source_t source);
void Set_Pol_Value_pre(double pol_index,double value,source_t source);
void Set_Pol_Value(uint8_t pol_index,uint32_t value,source_t source);
void Set_Type_Min_Max_Pol(double pol_index,double type,double min,double max,source_t source);
void Get_Pol_Struct(double pol_index,source_t source);
void Get_Structure(source_t source);
void set_frequency (uint8_t index);
void Get_Struct_Power(source_t source);
void Set_Sweep_Power(double s_power,source_t source);

#endif /* TASKGFX_TASKGFX_H_ */
