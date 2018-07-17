/*
 * game_controller.c
 *
 *  Created on: Jul 17, 2018
 *      Author: wangjingli
 */

#include "game_controller.h"
#include "draw_util.h"
#include "lcd_tft.h"
#include <stdint.h>

/*******************************************************************************
 * Pravite 
 ******************************************************************************/

static void GameCtr_UpdateDisplay(SnakeCtr sc) {
  DrawUtil_FillBackGroundColor();
  Snake_Draw(sc);
  LCD_Update(DrawUtil_InactFrameAddr());

  DrawUtil_DrawFrameDone();
}

/*******************************************************************************
 * Public
 ******************************************************************************/

SnakeCtr GameCtr_Init(void) {
  SnakeCtr sc = Snake_Init();

  DrawUtil_FillBackGroundColor();
  Snake_Draw(sc);

  LCD_Setup(DrawUtil_InactFrameAddr());

  DrawUtil_DrawFrameDone();

  return sc;
}

void GameCtr_TransPosToDirect(SnakeCtr sc, 
                              uint16_t touch_pos_x, 
                              uint16_t touch_pos_y) {
  Snake_TransPosToDirect(sc, touch_pos_x, touch_pos_y);
}

void GameCtr_Run(SnakeCtr sc) {
  Snake_Move(sc);
  GameCtr_UpdateDisplay(sc);
}