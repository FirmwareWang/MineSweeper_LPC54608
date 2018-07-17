/*
 * game_controller.c
 *
 *  Created on: Jul 17, 2018
 *      Author: wangjingli
 */

#include "game_controller.h"
#include "snake_engine.h"
#include "draw_util.h"
#include "lcd_tft.h"
#include <stdint.h>

/*******************************************************************************
 * Pravite 
 ******************************************************************************/

// TODO: In fact, drawing is not the task of engine module
static void GameCtr_FetchPointToDraw(uint16_t x, uint16_t y){
  DrawUtil_DrawPoint(y, x);
}

static void GameCtr_UpdateDisplay(SnakeCtr sc) {
  DrawUtil_FillBackGroundColor();
  Snake_Draw(sc);
  LCD_Update(DrawUtil_InactFrameAddr());

  DrawUtil_DrawFrameDone();
}

/*******************************************************************************
 * Public
 ******************************************************************************/

GameCtrl GameCtr_Init(void) {
  SnakeCtr sc = Snake_Init(GameCtr_FetchPointToDraw);

  DrawUtil_FillBackGroundColor();
  Snake_Draw(sc);

  LCD_Setup(DrawUtil_InactFrameAddr());

  DrawUtil_DrawFrameDone();

  return (GameCtrl)sc;
}

void GameCtr_Run(GameCtrl gc,
                 uint16_t touch_pos_x, 
                 uint16_t touch_pos_y,
                 bool touch_trigger) {
  SnakeCtr sc = (SnakeCtr) gc;
  if (touch_trigger) {
    Snake_TransPosToDirect(sc, touch_pos_x, touch_pos_y);
  }
  Snake_Move(sc);
  GameCtr_UpdateDisplay(sc);
}