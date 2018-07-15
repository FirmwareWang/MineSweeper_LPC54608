/*
 * simple_engine.c
 *
 *  Created on: Jul 8, 2018
 *      Author: wangjingli
 */

#include "draw_util.h"
#include "lcd_tft.h"
#include <stdint.h>
#include <stdio.h>

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Public
 ******************************************************************************/

#if 0 /* stop maintaining this module temporarily */
void Engine_Init(void) {
  LCD_Setup(DrawUtil_InactFrameAddr());
}

void Engine_TrackPoint(uint16_t pos_x, uint16_t pos_y) {
  DrawUtil_FillBackGroundColor();
  DrawUtil_DrawPoint(pos_x, pos_y);

  LCD_Update(DrawUtil_InactFrameAddr());

  DrawUtil_DrawFrameDone();
}
#endif 
