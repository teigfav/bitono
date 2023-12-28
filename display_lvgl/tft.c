/*********************
 *      INCLUDES
 *********************/
#include "lv_conf.h"
#include "lvgl/lvgl.h"
#include <string.h>
#include <stdlib.h>
#include "tft.h"
#include "stm32h7xx.h"
#include "stm32h735g_discovery_conf.h"
#include "cmsis_os.h"


extern osMutexId mutex_lvgl_id;
extern LTDC_HandleTypeDef hltdc;
extern DMA2D_HandleTypeDef hdma2d;
//uint8_t* my_fb_sdram=(uint8_t*)0x70000000;
#if LV_COLOR_DEPTH == 16
typedef uint16_t uintpixel_t;
#elif LV_COLOR_DEPTH == 24 || LV_COLOR_DEPTH == 32
typedef uint32_t uintpixel_t;
#endif


//static __IO uintpixel_t * my_fb = (__IO uintpixel_t*) (LCD_LAYER_0_ADDRESS);
//lv_disp_draw_buf_t* my_fb_sdram;

static lv_disp_drv_t disp_drv;
static lv_disp_draw_buf_t disp_buf_1;
static lv_disp_t *our_disp = NULL;

static void ex_disp_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t * color_p);
static void disp_flush_complete (DMA2D_HandleTypeDef*);


void tft_init(void)
{
osMutexWait(mutex_lvgl_id, osWaitForever);
lv_init();
osMutexRelease(mutex_lvgl_id);
static lv_color_t buf1_1[TFT_HOR_RES * 10];
osMutexWait(mutex_lvgl_id, osWaitForever);
lv_disp_draw_buf_init(&disp_buf_1, buf1_1, NULL, TFT_HOR_RES * 10);   /*Initialize the display buffer*/
osMutexRelease(mutex_lvgl_id);

osMutexWait(mutex_lvgl_id, osWaitForever);
lv_disp_drv_init(&disp_drv);
osMutexRelease(mutex_lvgl_id);
disp_drv.hor_res = TFT_HOR_RES;
disp_drv.ver_res = TFT_VER_RES;
disp_drv.flush_cb = ex_disp_flush;
disp_drv.draw_buf = &disp_buf_1;
disp_drv.full_refresh = 0;
hdma2d.XferCpltCallback = disp_flush_complete;
osMutexWait(mutex_lvgl_id, osWaitForever);
our_disp = lv_disp_drv_register(&disp_drv);
osMutexRelease(mutex_lvgl_id);
}

static void ex_disp_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t * color_p)
{
	osMutexWait(mutex_lvgl_id, osWaitForever);
  lv_coord_t width = lv_area_get_width(area);
  lv_coord_t height = lv_area_get_height(area);

  DMA2D->CR = 0x0U << DMA2D_CR_MODE_Pos;
  DMA2D->FGPFCCR = DMA2D_INPUT_RGB565;
  DMA2D->FGMAR = (uint32_t)color_p;
  DMA2D->FGOR = 0;
  DMA2D->OPFCCR = DMA2D_INPUT_RGB565;
  DMA2D->OMAR = hltdc.LayerCfg[0].FBStartAdress + 2 * \
                (area->y1 * TFT_HOR_RES + area->x1);
  DMA2D->OOR = TFT_HOR_RES - width;
  DMA2D->NLR = (width << DMA2D_NLR_PL_Pos) | (height << DMA2D_NLR_NL_Pos);
  DMA2D->IFCR = 0x3FU;
  DMA2D->CR |= DMA2D_CR_TCIE;
  DMA2D->CR |= DMA2D_CR_START;
  osMutexRelease(mutex_lvgl_id);
}

static void disp_flush_complete (DMA2D_HandleTypeDef *hdma2d)
{
osMutexWait(mutex_lvgl_id, osWaitForever);
  lv_disp_flush_ready(&disp_drv);
  osMutexRelease(mutex_lvgl_id);
}
