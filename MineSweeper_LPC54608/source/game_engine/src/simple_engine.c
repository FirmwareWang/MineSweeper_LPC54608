/*
 * simple_engine.c
 *
 *  Created on: Jul 8, 2018
 *      Author: wangjingli
 */

#include "simple_engine.h"
#include "draw_util.h"
#include "lcd_tft.h"
#include <stdint.h>
#include <stdio.h>

#define APP_PIXEL_PER_BYTE 4

/*******************************************************************************
 * Variables
 ******************************************************************************/
__attribute__((aligned(8)))
static uint8_t s_frameBuf0[IMG_HEIGHT][IMG_WIDTH / APP_PIXEL_PER_BYTE];

__attribute__((aligned(8)))
static uint8_t s_frameBuf1[IMG_HEIGHT][IMG_WIDTH / APP_PIXEL_PER_BYTE];

/*******************************************************************************
 * Public
 ******************************************************************************/

void Engine_Init(void) {
  DrawUtilConfig config = 
    {IMG_WIDTH, IMG_HEIGHT, APP_PIXEL_PER_BYTE, 
     {(uint32_t)s_frameBuf0, (uint32_t)s_frameBuf1}
    };
  DrawUtil_Init(&config);

  LCD_Setup(DrawUtil_InactFrameAddr());
}

void Engine_TrackPoint(uint16_t pos_x, uint16_t pos_y) {
  APP_DrawPoint(pos_x, pos_y);

  LCD_Update(DrawUtil_InactFrameAddr());

  DrawUtil_DrawFrameDone();
}
