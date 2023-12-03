/*
 * adf4372.c
 *
 *  Created on: Jul 27, 2023
 *      Author: giuli
 */
/***************************************************************************//**
 *   @file   adf4371.c
 *   @brief  Implementation of ADF4371 Setting.
 *   @author GF
********************************************************************************
*/

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include "main.h"
#include "no_os_util.h"
#include "adf4372.h"
#include "Util.h"

#define ADF4371_MODULUS1		33554432ULL
/* Specifications */
#define ADF4371_MIN_VCO_FREQ		4000000000ULL /* 4000 MHz */
#define ADF4371_MAX_VCO_FREQ		8000000000ULL /* 8000 MHz */
#define ADF4371_MAX_OUT_RF8_FREQ	ADF4371_MAX_VCO_FREQ /* Hz */
#define ADF4371_MIN_OUT_RF8_FREQ	(ADF4371_MIN_VCO_FREQ / 64) /* Hz */
#define ADF4371_MAX_OUT_RF16_FREQ	(ADF4371_MAX_VCO_FREQ * 2) /* Hz */
#define ADF4371_MIN_OUT_RF16_FREQ	(ADF4371_MIN_VCO_FREQ * 2) /* Hz */

/* ADF4371_REG17 */
#define ADF4371_FRAC2WORD_L_MSK		NO_OS_GENMASK(7, 1)
#define ADF4371_FRAC2WORD_L(x)		no_os_field_prep(ADF4371_FRAC2WORD_L_MSK, x)
#define ADF4371_FRAC1WORD_MSK		NO_OS_BIT(0)
#define ADF4371_FRAC1WORD(x)		no_os_field_prep(ADF4371_FRAC1WORD_MSK, x)

/* ADF4371_REG18 */
#define ADF4371_FRAC2WORD_H_MSK		NO_OS_GENMASK(6, 0)
#define ADF4371_FRAC2WORD_H(x)		no_os_field_prep(ADF4371_FRAC2WORD_H_MSK, x)

/* ADF4371_REG1A */
#define ADF4371_MOD2WORD_MSK		NO_OS_GENMASK(5, 0)
#define ADF4371_MOD2WORD(x)		no_os_field_prep(ADF4371_MOD2WORD_MSK, x)

/* MOD2 is the programmable, 14-bit auxiliary fractional modulus */
#define ADF4371_MAX_MODULUS2		NO_OS_BIT(14)

/* ADF4371_REG0 */
#define ADF4371_ADDR_ASC_MSK		NO_OS_BIT(2)
#define ADF4371_ADDR_ASC_R_MSK		NO_OS_BIT(5)
#define ADF4371_ADDR_ASC(x)		no_os_field_prep(ADF4371_ADDR_ASC_MSK, x)
#define ADF4371_ADDR_ASC_R(x)		no_os_field_prep(ADF4371_ADDR_ASC_R_MSK, x)

/* Registers address macro */
#define ADF4371_REG(x)			(x)

#define ADF4371_WRITE			(0 << 15)
#define ADF4371_READ			(1 << 15)
#define ADF4371_ADDR(x)			((x) & 0x7FFF)

struct reg_sequence {
	uint16_t reg;
	uint8_t val;
};


#if 0
static const struct reg_sequence adf4371_reg_defaults[] = {
	{ ADF4371_REG(0x01), 0x00 },
	{ ADF4371_REG(0x12), 0x40 },
	{ ADF4371_REG(0x1E), 0x48 },
	{ ADF4371_REG(0x20), 0x14 },
	{ ADF4371_REG(0x22), 0x00 },
	{ ADF4371_REG(0x23), 0x00 },
	{ ADF4371_REG(0x24), 0x80 },
	{ ADF4371_REG(0x25), 0x0B },
	{ ADF4371_REG(0x27), 0xC5 },
	{ ADF4371_REG(0x28), 0x83 },
	{ ADF4371_REG(0x2C), 0x44 },
	{ ADF4371_REG(0x2D), 0x11 },
	{ ADF4371_REG(0x2E), 0x12 },
	{ ADF4371_REG(0x2F), 0x94 },
	{ ADF4371_REG(0x32), 0x04 },
	{ ADF4371_REG(0x35), 0xFA },
	{ ADF4371_REG(0x36), 0x30 },
	{ ADF4371_REG(0x39), 0x07 },
	{ ADF4371_REG(0x3A), 0x55 },
	{ ADF4371_REG(0x3E), 0x0C },
	{ ADF4371_REG(0x3F), 0x80 },
	{ ADF4371_REG(0x40), 0x50 },
	{ ADF4371_REG(0x41), 0x28 },
	{ ADF4371_REG(0x47), 0xC0 },
	{ ADF4371_REG(0x52), 0xF4 },
	{ ADF4371_REG(0x70), 0x03 },
	{ ADF4371_REG(0x71), 0x60 },
	{ ADF4371_REG(0x72), 0x32 },
};
#endif
#if 0
static const struct reg_sequence adf4371_reg_defaults[] = {
	{ ADF4371_REG(0x00), 0x18 },
	{ ADF4371_REG(0x01), 0x00 },
	{ ADF4371_REG(0x0C), 0x00 },
	{ ADF4371_REG(0x0D), 0x00 },
	{ ADF4371_REG(0x11), 0x00 },
	{ ADF4371_REG(0x12), 0x40 },
	{ ADF4371_REG(0x14), 0x00 },
	{ ADF4371_REG(0x15), 0x00 },
	{ ADF4371_REG(0x17), 0x00 },
	{ ADF4371_REG(0x18), 0xC5 },
	{ ADF4371_REG(0x19), 0xE8 },
	{ ADF4371_REG(0x1A), 0x03 },
	{ ADF4371_REG(0x1B), 0x00 },
	{ ADF4371_REG(0x1C), 0x00 },
	{ ADF4371_REG(0x1D), 0x00 },
	{ ADF4371_REG(0x1E), 0x48 },
	{ ADF4371_REG(0x1F), 0x01 },
	{ ADF4371_REG(0x20), 0x14 },
	{ ADF4371_REG(0x21), 0x00 },
	{ ADF4371_REG(0x22), 0x00 },
	{ ADF4371_REG(0x23), 0x00 },
	{ ADF4371_REG(0x24), 0x80 },
	{ ADF4371_REG(0x25), 0x0B },
	{ ADF4371_REG(0x26), 0x20 },
	{ ADF4371_REG(0x28), 0x83 },
	{ ADF4371_REG(0x2A), 0x00 },
	{ ADF4371_REG(0x2B), 0x01 },
	{ ADF4371_REG(0x2C), 0x44 },
	{ ADF4371_REG(0x2D), 0x11 },
	{ ADF4371_REG(0x2E), 0x12 },
	{ ADF4371_REG(0x2F), 0x94 },
	{ ADF4371_REG(0x30), 0x2A },
	{ ADF4371_REG(0x31), 0x02 },
	{ ADF4371_REG(0x32), 0x04 },
	{ ADF4371_REG(0x33), 0x22 },
	{ ADF4371_REG(0x34), 0x85 },
	{ ADF4371_REG(0x35), 0x4C },
	{ ADF4371_REG(0x36), 0x30 },
	{ ADF4371_REG(0x37), 0x00 },
	{ ADF4371_REG(0x38), 0x00 },
	{ ADF4371_REG(0x39), 0x07 },
	{ ADF4371_REG(0x3A), 0x55 },
	{ ADF4371_REG(0x3D), 0x00 },
	{ ADF4371_REG(0x3E), 0x0C },
	{ ADF4371_REG(0x3F), 0x80 },
	{ ADF4371_REG(0x40), 0x50 },
	{ ADF4371_REG(0x41), 0x28 },
	{ ADF4371_REG(0x42), 0x00 },
	{ ADF4371_REG(0x43), 0x00 },
	{ ADF4371_REG(0x44), 0x00 },
	{ ADF4371_REG(0x45), 0x00 },
	{ ADF4371_REG(0x46), 0x00 },
	{ ADF4371_REG(0x47), 0xC0 },
	{ ADF4371_REG(0x52), 0xF4 },
	{ ADF4371_REG(0x6C), 0x00 },
	{ ADF4371_REG(0x6E), 0x00 },
	{ ADF4371_REG(0x6F), 0x00 },
	{ ADF4371_REG(0x70), 0x03 },
	{ ADF4371_REG(0x72), 0x32 },
	{ ADF4371_REG(0x73), 0x00 },
	{ ADF4371_REG(0x79), 0x00 },
	{ ADF4371_REG(0x7A), 0x00 },
	{ ADF4371_REG(0x7B), 0x00 },
	{ ADF4371_REG(0x7C), 0x00 },
	{ ADF4371_REG(0x10), 0x32 },
};
#endif
static const struct reg_sequence adf4371_reg_defaults[] = {
		 { ADF4371_REG(0x00), 0x18},
		 { ADF4371_REG(0x01), 0x00},
		 { ADF4371_REG(0x02), 0x00},
		 { ADF4371_REG(0x04), 0x01},
		 { ADF4371_REG(0x05), 0x00},
		 { ADF4371_REG(0x06), 0x0A},
		 { ADF4371_REG(0x0C), 0x56},
		 { ADF4371_REG(0x0D), 0x04},
		 { ADF4371_REG(0x10), 0x46},
		 { ADF4371_REG(0x11), 0x00},
		 { ADF4371_REG(0x12), 0x40},
		 { ADF4371_REG(0x14), 0x00},
		 { ADF4371_REG(0x15), 0x00},
		 { ADF4371_REG(0x16), 0x00},
		 { ADF4371_REG(0x17), 0x00},
		 { ADF4371_REG(0x18), 0x00},
		 { ADF4371_REG(0x19), 0x01},
		 { ADF4371_REG(0x1A), 0x00},
		 { ADF4371_REG(0x1B), 0x00},
		 { ADF4371_REG(0x1C), 0x00},
		 { ADF4371_REG(0x1D), 0x00},
		 { ADF4371_REG(0x1E), 0x58},
		 { ADF4371_REG(0x1F), 0x01},
		 { ADF4371_REG(0x20), 0x14},
		 { ADF4371_REG(0x21), 0x00},
		 { ADF4371_REG(0x22), 0x00},
		 { ADF4371_REG(0x23), 0x00},
		 { ADF4371_REG(0x24), 0x080},
		 { ADF4371_REG(0x25), 0x0B},
		 { ADF4371_REG(0x26), 0x20},
		 { ADF4371_REG(0x27), 0x0E5},
		 { ADF4371_REG(0x28), 0x083},
		 { ADF4371_REG(0x2A), 0x00},
		 { ADF4371_REG(0x2B), 0x01},
		 { ADF4371_REG(0x2C), 0x44},
		 { ADF4371_REG(0x2D), 0x11},
		 { ADF4371_REG(0x2E), 0x10},
		 { ADF4371_REG(0x2F), 0x092},
		 { ADF4371_REG(0x30), 0x2A},
		 { ADF4371_REG(0x31), 0x02},
		 { ADF4371_REG(0x32), 0x04},
		 { ADF4371_REG(0x33), 0x22},
		 { ADF4371_REG(0x34), 0x085},
		 { ADF4371_REG(0x35), 0x0FA},
		 { ADF4371_REG(0x36), 0x30},
		 { ADF4371_REG(0x37), 0x00},
		 { ADF4371_REG(0x38), 0x00},
		 { ADF4371_REG(0x39), 0x07},
		 { ADF4371_REG(0x3A), 0x55},
		 { ADF4371_REG(0x3D), 0x00},
		 { ADF4371_REG(0x3E), 0x0C},
		 { ADF4371_REG(0x3F), 0x080},
		 { ADF4371_REG(0x40), 0x50},
		 { ADF4371_REG(0x41), 0x28},
		 { ADF4371_REG(0x42), 0x00},
		 { ADF4371_REG(0x43), 0x00},
		 { ADF4371_REG(0x44), 0x00},
		 { ADF4371_REG(0x45), 0x00},
		 { ADF4371_REG(0x46), 0x00},
		 { ADF4371_REG(0x47), 0x0C0},
		 { ADF4371_REG(0x52), 0x0F4},
		 { ADF4371_REG(0x6C), 0x00},
		 { ADF4371_REG(0x6E), 0x0C9},
		 { ADF4371_REG(0x6F), 0x08},
		 { ADF4371_REG(0x70), 0x23},
		 { ADF4371_REG(0x72), 0x32},
		 { ADF4371_REG(0x73), 0x00},
		 { ADF4371_REG(0x79), 0x00},
		 { ADF4371_REG(0x7A), 0x00},
		 { ADF4371_REG(0x7B), 0x00},
		 { ADF4371_REG(0x7C), 0x01},
};
/**
 * Compute PLL parameters.
 * @param vco - The VCO frequency.
 * @param pfd - The PFD frequency.
 * @param integer - The integer division factor.
 * @param fract1 - The fractionality.
 * @param fract2 - The auxiliary fractionality.
 * @param mod2 - The auxiliary modulus.
 * @return None.
 */
static void adf4371_pll_fract_n_compute(uint64_t vco,
					uint64_t pfd,
					uint32_t *integer,
					uint32_t *fract1,
					uint32_t *fract2,
					uint32_t *mod2)
{
	uint64_t tmp;
	uint32_t gcd_div;

	tmp = no_os_do_div(&vco, pfd);  //contiene il resto della divisione
	tmp = tmp * ADF4371_MODULUS1;	//questo calcola il FRAC1
	*fract2 = no_os_do_div(&tmp, pfd);

	*integer = vco;
	*fract1 = tmp;

	*mod2 = pfd;

	while (*mod2 > ADF4371_MAX_MODULUS2) {
		*mod2 >>= 1;
		*fract2 >>= 1;
	}

	gcd_div = no_os_greatest_common_divisor(*fract2, *mod2);
	*mod2 /= gcd_div;
	*fract2 /= gcd_div;
}

/**
 * SPI register write to device.
 * @param dev - The device structure.
 * @param reg - The register address.
 * @param val - The register data.
 * @return 0 in case of success, negative error code otherwise.
 */
int32_t adf4371_write(uint16_t reg, uint8_t val, enum Sint_number sint)
{
	uint8_t buf[3];
	uint16_t cmd;
	//print_k("PLL write reg\r\n");
	cmd = ADF4371_WRITE | ADF4371_ADDR(reg);
	buf[0] = cmd >> 8;
	buf[1] = cmd & 0xFF;
	buf[2] = val;
	if(sint==Sint1)
	{
		HAL_GPIO_WritePin(CS1_GPIO_Port,CS1_Pin,GPIO_PIN_RESET);
	}
	else if(sint==Sint2)
	{
		HAL_GPIO_WritePin(CS2_GPIO_Port,CS2_Pin,GPIO_PIN_RESET);
	}
	else if(sint==Sint3)
	{
		HAL_GPIO_WritePin(CS3_GPIO_Port,CS3_Pin,GPIO_PIN_RESET);
	}
	else
	{
		print_k("Error : wrong sint number\r\n");
	}
	HAL_SPI_Transmit(&hspi2, buf, NO_OS_ARRAY_SIZE(buf), 10);
	HAL_GPIO_WritePin(CS1_GPIO_Port,CS1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS2_GPIO_Port,CS2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS3_GPIO_Port,CS3_Pin,GPIO_PIN_SET);
	return 0;
}

/**
 * SPI register read from device.
 * @param dev - The device structure.
 * @param reg - The register address.
 * @param val - The register data.
 * @return 0 in case of success, negative error code otherwise.
 */
int32_t adf4371_read(uint16_t reg,uint8_t *val,enum Sint_number sint)
{
	uint8_t buf[3];
	uint16_t cmd;

	cmd = ADF4371_READ | ADF4371_ADDR(reg);
	buf[0] = cmd >> 8;
	buf[1] = cmd & 0xFF;
	buf[2] = 0;

	if(sint==Sint1)
	{
		HAL_GPIO_WritePin(CS1_GPIO_Port,CS1_Pin,GPIO_PIN_RESET);
	}
	else if(sint==Sint2)
	{
		HAL_GPIO_WritePin(CS2_GPIO_Port,CS2_Pin,GPIO_PIN_RESET);
	}
	else if(sint==Sint3)
	{
		HAL_GPIO_WritePin(CS3_GPIO_Port,CS3_Pin,GPIO_PIN_RESET);
	}
	else
	{
		print_k("Error : wrong sint number\r\n");
	}
	HAL_SPI_Receive(&hspi2, buf, NO_OS_ARRAY_SIZE(buf),10);
	HAL_GPIO_WritePin(CS1_GPIO_Port,CS1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS2_GPIO_Port,CS2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS3_GPIO_Port,CS3_Pin,GPIO_PIN_SET);
	*val = buf[2];

	return 0;
}

/**
 * SPI register update.
 * @param dev - The device structure.
 * @param reg - The register address.
 * @param reg - The register address.
 * @param val - The register data.
 * @return 0 in case of success, negative error code otherwise.
 */
int32_t adf4371_update(uint16_t reg,uint8_t mask,uint8_t val,enum Sint_number sint)
{
	uint8_t read_val;
	int32_t ret = 0;

	ret = adf4371_read(reg, &read_val,sint);
	read_val &= ~mask;
	read_val |= val;
	ret |= adf4371_write(reg, read_val,sint);
	return ret;
}

/**
 * SPI register write bulk to device.
 * @param dev - The device structure.
 * @param reg - The register address.
 * @param val - The buffer of data to be written.
 * @param size - The buffer size.
 * @return 0 in case of success, negative error code otherwise.
 */
static int32_t adf4371_write_bulk(uint16_t reg,uint8_t *val,uint8_t size,enum Sint_number sint)
{
	uint8_t buf[12];
	uint16_t cmd;
	uint8_t i;

	cmd = ADF4371_WRITE | ADF4371_ADDR(reg);
	buf[0] = cmd >> 8;
	buf[1] = cmd & 0xFF;
	for (i = 0; i < size; i++)
		buf[2 + i] = val[i];
	if(sint==Sint1)
		{
			HAL_GPIO_WritePin(CS1_GPIO_Port,CS1_Pin,GPIO_PIN_RESET);
		}
		else if(sint==Sint2)
		{
			HAL_GPIO_WritePin(CS2_GPIO_Port,CS2_Pin,GPIO_PIN_RESET);
		}
		else if(sint==Sint3)
		{
			HAL_GPIO_WritePin(CS3_GPIO_Port,CS3_Pin,GPIO_PIN_RESET);
		}
		else
		{
			print_k("Error : wrong sint number\r\n");
		}
	HAL_SPI_Transmit(&hspi2, buf, size+2, 10);
	HAL_GPIO_WritePin(CS1_GPIO_Port,CS1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS2_GPIO_Port,CS2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS3_GPIO_Port,CS3_Pin,GPIO_PIN_SET);
	return 0;
}

/**
 * @brief init PLL
 *
 */
void adf4371_pll_init(enum Sint_number sint)
{
//print_k("PLL init\r\n");
adf4371_write(ADF4371_REG(0x00),0x18,sint);
adf4371_write(ADF4371_REG(0x01),0x00,sint);

}

int32_t adf4371_pll_set_freq(struct adf4371_dev *dev,uint64_t frequenza,enum Sint_number sint)
{
	uint8_t buf[10];
	uint32_t cp_bleed;
switch (dev->channel)
	{
		case ADF4371_CH_RF16:
			if(frequenza < ADF4371_MIN_OUT_RF16_FREQ || frequenza > ADF4371_MAX_OUT_RF16_FREQ)
				return -1;
			frequenza >>= 1;  //divide la frequenza
		break;
		case ADF4371_CH_RF8:
		case ADF4371_CH_RFAUX8:
			if(frequenza < ADF4371_MIN_OUT_RF8_FREQ || frequenza > ADF4371_MAX_OUT_RF8_FREQ)
				return -1;
		break;
	}

adf4371_pll_fract_n_compute(frequenza,dev->fpfd,&dev->integer,&dev->fract1,&dev->fract2,&dev->mod2);
//LOG_DBG("fpfd %lu  integer %lu  fract1  %lu  fract2  %lu   mod2  %lu\n\r",dev->fpfd,dev->integer,dev->fract1,dev->fract2,dev->mod2);

	buf[0] = dev->integer >> 8;
	buf[1] = 0x40; /* REG12 default */
	buf[2] = 0x00;
	buf[3] = dev->fract1 & 0xFF;
	buf[4] = dev->fract1 >> 8;
	buf[5] = dev->fract1 >> 16;
	buf[6] = ADF4371_FRAC2WORD_L(dev->fract2 & 0x7F) | ADF4371_FRAC1WORD(dev->fract1 >> 24);
	buf[7] = ADF4371_FRAC2WORD_H(dev->fract2 >> 7);
	buf[8] = dev->mod2 & 0xFF;
	buf[9] = ADF4371_MOD2WORD(dev->mod2 >> 8);
	adf4371_write_bulk(ADF4371_REG(0x11), buf, 10,sint);
	adf4371_write(ADF4371_REG(0x1F), dev->ref_div_factor,sint);
	adf4371_write(ADF4371_REG(0x24), 0x80,sint);

	cp_bleed = NO_OS_DIV_ROUND_UP(400 * dev->cp_current, dev->integer * 375);
	cp_bleed = no_os_clamp(cp_bleed, 1U, 255U);
	adf4371_write(ADF4371_REG(0x26), cp_bleed,sint);
		/*
	 * Set to 1 when in INT mode (when FRAC1 = FRAC2 = 0),
	 * and set to 0 when in FRAC mode.
	 */
	if (dev->fract1 == 0 && dev->fract2 == 0)  //verifica se intero allora toglie il fracmode
		{
			adf4371_write(ADF4371_REG(0x2B), 0x01,sint);
		}
		else
		{
			adf4371_write(ADF4371_REG(0x2B), 0x00,sint);
		}
	adf4371_write(ADF4371_REG(0x10), dev->integer & 0xFF,sint);
//	print_k("buffer %x %x %x %x %x %x\n\r",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
	return 0;
}
int32_t adf4371_setup(struct adf4371_dev *dev,enum Sint_number sint)
{
	for (int i = 0; i < NO_OS_ARRAY_SIZE(adf4371_reg_defaults); i++)
	{
		adf4371_write(adf4371_reg_defaults[i].reg,adf4371_reg_defaults[i].val,sint);
	}
	/* Set address in ascending order, so the write_bulk() will work */
	adf4371_update(ADF4371_REG(0x0),
			     ADF4371_ADDR_ASC_MSK | ADF4371_ADDR_ASC_R_MSK,
			     ADF4371_ADDR_ASC(1) | ADF4371_ADDR_ASC_R(1),sint);
	return 0;
}



