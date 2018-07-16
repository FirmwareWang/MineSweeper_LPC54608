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

#define POINT_SIZE_PIXEL 3
#define APP_IMG_HEIGHT (IMG_HEIGHT - POINT_SIZE_PIXEL)
#define APP_IMG_WIDTH  (IMG_WIDTH - POINT_SIZE_PIXEL)

typedef enum {
  UP = 0,
  DOWN,
  LEFT,
  RIGHT,
  RANGE_OUT,
} TouchDirection;

/*******************************************************************************
 * Globals
 ******************************************************************************/

static DrawPos cur_pos = {INIT_SNAKE_POS_X, INIT_SNAKE_POS_Y};
static TouchDirection cur_direction = RIGHT;

/*******************************************************************************
 * Pravite 
 ******************************************************************************/

static TouchDirection Snake_TransPosToDirect(uint16_t pos_x, uint16_t pos_y) {
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

// If the point goes out of image range, rolling it to another side
static void Snake_UpdateCurPos(TouchDirection dir) {
  switch(dir){
    case UP:
      cur_pos.y = (cur_pos.y < POINT_SIZE_PIXEL) ? 
                  APP_IMG_HEIGHT : cur_pos.y - POINT_SIZE_PIXEL;
      break;
    case DOWN:
      cur_pos.y = (APP_IMG_HEIGHT - cur_pos.y < POINT_SIZE_PIXEL) ? 
                  0 : cur_pos.y + POINT_SIZE_PIXEL;
      break;
    case LEFT:
      cur_pos.x = (cur_pos.x < POINT_SIZE_PIXEL) ? 
                  APP_IMG_WIDTH : cur_pos.x - POINT_SIZE_PIXEL;
      break;
    case RIGHT:
      cur_pos.x = (APP_IMG_WIDTH - cur_pos.x < POINT_SIZE_PIXEL) ? 
                  0 : cur_pos.x + POINT_SIZE_PIXEL;
      break;
    default:
      break;
  }
}

static void Snake_UpdateDisplay(void) {
  DrawUtil_FillBackGroundColor();
  DrawUtil_DrawPoint(&cur_pos, POINT_SIZE_PIXEL);
  LCD_Update(DrawUtil_InactFrameAddr());

  DrawUtil_DrawFrameDone();
}

/*******************************************************************************
 * Public
 ******************************************************************************/

void Snake_Init(void) {
  DrawUtil_FillBackGroundColor();
  DrawUtil_DrawPoint(&cur_pos, POINT_SIZE_PIXEL);

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

