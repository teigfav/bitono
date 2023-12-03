//IC:GSL1680f
//DTAE:Mar-27-2018
//VER:1.1
#ifndef _GSLX680_H_
#define _GSLX680_H_
#include "stm32h7xx_hal.h"
#include "stdbool.h"
//#include "stdio.h"

#define SCREEN_MAX_X 		480
#define SCREEN_MAX_Y 		272

//#define uchar      unsigned char
//#define uint       unsigned int
//#define ulong      unsigned long

extern I2C_HandleTypeDef hi2c4;
void GSL1680_Init(void);
int gsl_ts_read(int addr, uint8_t *pdata, unsigned int datalen);
int gsl_ts_write(int addr, uint8_t *pdata, int datalen);
void check_mem_data(void);
void dataread(void);
#ifdef __cplusplus
extern "C" {
#endif
bool get_stato(int32_t *x,int32_t*y);
#ifdef __cplusplus
}
#endif
struct fw_data
{
    uint8_t offset;
    uint32_t val;
};

struct State {
	uint8_t touch;
    uint16_t x;
    uint16_t y;
};


#endif

