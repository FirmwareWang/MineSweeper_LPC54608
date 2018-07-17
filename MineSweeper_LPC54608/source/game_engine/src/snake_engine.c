/*
 * snake_engine.c
 *
 *  Created on: Jul 15, 2018
 *      Author: wangjingli
 */

#include "snake_engine.h"
#include "lcd_tft.h"
#include "draw_util.h"
#include "ring_buf.h"
#include "display_config.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define INIT_SNAKE_POS_X 0
#define INIT_SNAKE_POS_Y 0

#define SNAKE_POINTS_PITCH_PIXCEL 1
#define SNAKE_POINTS_STEP_PIXCEL \
  (SNAKE_POINTS_PITCH_PIXCEL + POINT_SIZE_PIXEL)
// the buffer length should be the 2^n
#define SNAKE_LEN_MAX 1024

#define APP_IMG_HEIGHT (IMG_HEIGHT - SNAKE_POINTS_STEP_PIXCEL)
#define APP_IMG_WIDTH  (IMG_WIDTH - SNAKE_POINTS_STEP_PIXCEL)

typedef enum {
  UP = 0,
  DOWN,
  LEFT,
  RIGHT,
  RANGE_OUT,
} TouchDirection;

typedef struct _SnakeHandle {
  RINGBUFF_T snake_container;
  TouchDirection move_direct;
} SnakeHandle;

/*******************************************************************************
 * Globals
 ******************************************************************************/

static DrawPos snake_map_buf[SNAKE_LEN_MAX];

/*******************************************************************************
 * Pravite 
 ******************************************************************************/

// If the point goes out of image range, rolling it to another side
static void Snake_GetNewHeadPos(const DrawPos *cur_head, 
                                TouchDirection dir,
                                DrawPos *new_head) {
  *new_head = *cur_head;
  switch(dir){
    case UP:
      new_head->y = 
        (cur_head->y < SNAKE_POINTS_STEP_PIXCEL) ?
        APP_IMG_HEIGHT : cur_head->y - SNAKE_POINTS_STEP_PIXCEL;
      break;
    case DOWN:
      new_head->y = 
        (APP_IMG_HEIGHT - cur_head->y < SNAKE_POINTS_STEP_PIXCEL) ?
        0 : cur_head->y + SNAKE_POINTS_STEP_PIXCEL;
      break;
    case LEFT:
      new_head->x = 
        (cur_head->x < SNAKE_POINTS_STEP_PIXCEL) ?
        APP_IMG_WIDTH : cur_head->x - SNAKE_POINTS_STEP_PIXCEL;
      break;
    case RIGHT:
      new_head->x = 
        (APP_IMG_WIDTH - cur_head->x < SNAKE_POINTS_STEP_PIXCEL) ?
        0 : cur_head->x + SNAKE_POINTS_STEP_PIXCEL;
      break;
    default:
      break;
  }
}

static void Snake_Lengthen(SnakeHandle *handle) {
  RINGBUFF_T *ring_buf = &(handle->snake_container);
  DrawPos cur_head, new_head;
  RingBuffer_GetHead(ring_buf, &cur_head);
  Snake_GetNewHeadPos(&cur_head, handle->move_direct, &new_head);
  RingBuffer_Insert(ring_buf, (void *)&new_head);
}

static void Snake_Move(SnakeHandle *handle) {
  DrawPos temp;
  Snake_Lengthen(handle);
  RingBuffer_Pop(&(handle->snake_container), &temp);
}

static void Snake_Draw(RINGBUFF_T *snake_map){
  RingBuffer_Seek(snake_map, RING_BUF_TAIL);
  for (int i = 0; i < RingBuffer_GetCount(snake_map); i++) {
    DrawPos point;
    RingBuffer_GetItem(snake_map, (void *)&point);
    DrawUtil_DrawPoint(&point);
  }
}

static void Snake_UpdateDisplay(RINGBUFF_T *snake_map) {
  DrawUtil_FillBackGroundColor();
  Snake_Draw(snake_map);
  LCD_Update(DrawUtil_InactFrameAddr());

  DrawUtil_DrawFrameDone();
}

/*******************************************************************************
 * Public
 ******************************************************************************/

SnakeCtr Snake_Init(void) {
  SnakeHandle *handle = (SnakeHandle *)malloc(sizeof(SnakeHandle) * 1);
  RingBuffer_Init(&(handle->snake_container),
                  (void *) snake_map_buf,
                  sizeof(snake_map_buf[0]),
                  SNAKE_LEN_MAX,
                  NULL);
  handle->move_direct = RIGHT;

  DrawPos init_pos = {INIT_SNAKE_POS_X, INIT_SNAKE_POS_Y};
  RingBuffer_Insert(&(handle->snake_container), &init_pos);
  Snake_Lengthen(handle);
  Snake_Lengthen(handle);

  DrawUtil_FillBackGroundColor();
  Snake_Draw(&(handle->snake_container));

  LCD_Setup(DrawUtil_InactFrameAddr());

  DrawUtil_DrawFrameDone();

  return (SnakeCtr)handle;
}

void Snake_TransPosToDirect(SnakeCtr sc, 
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
  SnakeHandle *handle = (SnakeHandle *)sc;

  handle->move_direct =  
    (touch_pos_x < IMG_WIDTH / 4)     ? LEFT : 
    (touch_pos_x > IMG_WIDTH * 3 / 4) ? RIGHT : 
    (touch_pos_y < IMG_HEIGHT / 2)    ? UP : 
    (touch_pos_y > IMG_HEIGHT / 2)    ? DOWN : RANGE_OUT;
}

void Snake_ControlPoint(SnakeCtr sc) {
  SnakeHandle *handle = (SnakeHandle *)sc;
  Snake_Move(handle);
  Snake_UpdateDisplay(&(handle->snake_container));
}

