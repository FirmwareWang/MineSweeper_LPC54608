/*
 * snake_engine.c
 *
 *  Created on: Jul 15, 2018
 *      Author: wangjingli
 */

#include "snake_engine.h"
#include "lcd_tft.h"
#include "draw_util.h"
#include "display_config.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define INIT_SNAKE_POS_X 0
#define INIT_SNAKE_POS_Y 0

#define APP_IMG_HEIGHT (IMG_HEIGHT - POINT_SIZE_PIXEL)
#define APP_IMG_WIDTH  (IMG_WIDTH - POINT_SIZE_PIXEL)

typedef enum {
  UP = 0,
  DOWN,
  LEFT,
  RIGHT,
  RANGE_OUT,
} TouchDirection;

typedef struct _SnakeStatus {
  DrawPos pos;
  TouchDirection move_direct;
} SnakeStatus;

/*******************************************************************************
 * Globals
 ******************************************************************************/

/*******************************************************************************
 * Pravite 
 ******************************************************************************/


// If the point goes out of image range, rolling it to another side
static void Snake_UpdateCurPos(SnakeStatus *stat) {
  switch(stat->move_direct){
    case UP:
      stat->pos.y = (stat->pos.y < POINT_SIZE_PIXEL) ? 
                    APP_IMG_HEIGHT : stat->pos.y - POINT_SIZE_PIXEL;
      break;
    case DOWN:
      stat->pos.y = (APP_IMG_HEIGHT - stat->pos.y < POINT_SIZE_PIXEL) ? 
                    0 : stat->pos.y + POINT_SIZE_PIXEL;
      break;
    case LEFT:
      stat->pos.x = (stat->pos.x < POINT_SIZE_PIXEL) ? 
                    APP_IMG_WIDTH : stat->pos.x - POINT_SIZE_PIXEL;
      break;
    case RIGHT:
      stat->pos.x = (APP_IMG_WIDTH - stat->pos.x < POINT_SIZE_PIXEL) ? 
                    0 : stat->pos.x + POINT_SIZE_PIXEL;
      break;
    default:
      break;
  }
}

static void Snake_UpdateDisplay(const DrawPos *pos) {
  DrawUtil_FillBackGroundColor();
  DrawUtil_DrawPoint(pos);
  LCD_Update(DrawUtil_InactFrameAddr());

  DrawUtil_DrawFrameDone();
}

/*******************************************************************************
 * Public
 ******************************************************************************/

SnakeHandle Snake_Init(void) {
  SnakeStatus *handle = (SnakeStatus *)malloc(sizeof(SnakeStatus) * 1);
  handle->pos.x = INIT_SNAKE_POS_X;
  handle->pos.y = INIT_SNAKE_POS_Y;
  handle->move_direct = RIGHT;

  DrawUtil_FillBackGroundColor();
  DrawUtil_DrawPoint(&handle->pos);

  LCD_Setup(DrawUtil_InactFrameAddr());

  DrawUtil_DrawFrameDone();

  return (SnakeHandle)handle;
}

void Snake_TransPosToDirect(SnakeHandle sh, 
                            uint16_t touch_pos_x, 
                            uint16_t touch_pos_y) {
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
  SnakeStatus *stat = (SnakeStatus *)sh;

  stat->move_direct =  
    (touch_pos_x < IMG_WIDTH / 4)     ? LEFT : 
    (touch_pos_x > IMG_WIDTH * 3 / 4) ? RIGHT : 
    (touch_pos_y < IMG_HEIGHT / 2)    ? UP : 
    (touch_pos_y > IMG_HEIGHT / 2)    ? DOWN : RANGE_OUT;
}

void Snake_ControlPoint(SnakeHandle sh) {
  SnakeStatus *stat = (SnakeStatus *)sh;
  Snake_UpdateCurPos(stat);
  Snake_UpdateDisplay(&stat->pos);
}

