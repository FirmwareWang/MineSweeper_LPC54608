/*
 * snake_engine.c
 *
 *  Created on: Jul 15, 2018
 *      Author: wangjingli
 */

#include "lcd_tft.h"
#include "draw_util.h"
#include "display_config.h"
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define INIT_SNAKE_POS_X 0
#define INIT_SNAKE_POS_Y 0

typedef enum {
  UP = 0,
  DOWN,
  LEFT,
  RIGHT,
  RANGE_OUT,
} TouchDrection;

/*******************************************************************************
 * Globals
 ******************************************************************************/

static uint16_t cur_pos_x = INIT_SNAKE_POS_X;
static uint16_t cur_pos_y = INIT_SNAKE_POS_Y;
static TouchDrection cur_direction = RIGHT;

/*******************************************************************************
 * Pravite 
 ******************************************************************************/

static TouchDrection Snake_TransPosToDirect(uint16_t pos_x, uint16_t pos_y) {
 /*
     +-------------------------------------------------------------------------+
     |                 |                                      |                |
     |                 |                                      |                |
     |                 |                                      |                |
     |                 |        UP                            |                |
     |                 |                                      |                |
     |                 |                                      |                |
     |                 |                                      |                |
     |                 |                                      |                |
     |                 |                                      |                |
     |    LEFT         |                                      |  RIGHT         |
     |                 |--------------------------------------|                |
     |                 |                                      |                |
     |                 |                                      |                |
     |                 |                                      |                |
     |                 |                                      |                |
     |                 |                                      |                |
     |                 |                                      |                |
     |                 |                                      |                |
     |                 |       DOWN                           |                |
     |                 |                                      |                |
     |                 |                                      |                |
     +-------------------------------------------------------------------------+
  */ 

  return (pos_x < IMG_WIDTH / 4)     ? LEFT : 
         (pos_x > IMG_WIDTH * 3 / 4) ? RIGHT :
         (pos_y < IMG_HEIGHT / 2)    ? UP :
         (pos_y > IMG_HEIGHT / 2)    ? DOWN : RANGE_OUT;
}

static void Snake_UpdateCurPos(TouchDrection dir) {
  switch(dir){
    case UP:
      cur_pos_y--;
      break;
    case DOWN:
      cur_pos_y++;
      break;
    case LEFT:
      cur_pos_x--;
      break;
    case RIGHT:
      cur_pos_x++;
      break;
    default:
      break;
  }
}

static void Snake_UpdateDisplay(void) {
  DrawUtil_FillBackGroundColor();
  DrawUtil_DrawPoint(cur_pos_x, cur_pos_y);
  LCD_Update(DrawUtil_InactFrameAddr());

  DrawUtil_DrawFrameDone();
}

/*******************************************************************************
 * Public
 ******************************************************************************/

void Snake_Init(void) {
  DrawUtil_FillBackGroundColor();
  DrawUtil_DrawPoint(INIT_SNAKE_POS_X, INIT_SNAKE_POS_Y);

  LCD_Setup(DrawUtil_InactFrameAddr());

  DrawUtil_DrawFrameDone();
}

void Snake_ControlPoint(uint16_t pos_x, uint16_t pos_y, bool pos_updated) {
  if (pos_updated) {
    cur_direction = Snake_TransPosToDirect(pos_x, pos_y);
  }

  Snake_UpdateCurPos(cur_direction);
  Snake_UpdateDisplay();
}

