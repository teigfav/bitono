/*
 * tft.h
 *
 *  Created on: 26 nov 2023
 *      Author: giuli
 */

#ifndef TFT_H_
#define TFT_H_

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include "lvgl/src/misc/lv_color.h"
#include "lvgl/src/misc/lv_area.h"

/*********************
 *      DEFINES
 *********************/
#define TFT_HOR_RES 480
#define TFT_VER_RES 272

void tft_init(void);


#endif /* TFT_H_ */
