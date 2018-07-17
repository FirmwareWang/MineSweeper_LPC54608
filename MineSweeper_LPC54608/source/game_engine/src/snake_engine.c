/*
 * snake_engine.c
 *
 *  Created on: Jul 15, 2018
 *      Author: wangjingli
 */

#include "snake_engine.h"
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

#define SNAKE_POINTS_PITCH_PIXEL 1
#define SNAKE_POINTS_STEP_PIXEL \
  (SNAKE_POINTS_PITCH_PIXEL + POINT_SIZE_PIXEL)
// the buffer length should be the 2^n
#define SNAKE_LEN_MAX 1024

#define APP_IMG_HEIGHT (IMG_HEIGHT - SNAKE_POINTS_STEP_PIXEL)
#define APP_IMG_WIDTH  (IMG_WIDTH - SNAKE_POINTS_STEP_PIXEL)

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

typedef struct _DrawPos {
  uint16_t x;
  uint16_t y;
} SnakePixelPos;

/*******************************************************************************
 * Globals
 ******************************************************************************/

static SnakePixelPos snake_map_buf[SNAKE_LEN_MAX];

/*******************************************************************************
 * Pravite 
 ******************************************************************************/

// If the point goes out of image range, rolling it to another side
static void Snake_GetNewHeadPos(const SnakePixelPos *cur_head, 
                                TouchDirection dir,
                                SnakePixelPos *new_head) {
  *new_head = *cur_head;
  switch(dir){
    case UP:
      new_head->y = 
        (cur_head->y < SNAKE_POINTS_STEP_PIXEL) ?
        APP_IMG_HEIGHT : cur_head->y - SNAKE_POINTS_STEP_PIXEL;
      break;
    case DOWN:
      new_head->y = 
        (APP_IMG_HEIGHT - cur_head->y < SNAKE_POINTS_STEP_PIXEL) ?
        0 : cur_head->y + SNAKE_POINTS_STEP_PIXEL;
      break;
    case LEFT:
      new_head->x = 
        (cur_head->x < SNAKE_POINTS_STEP_PIXEL) ?
        APP_IMG_WIDTH : cur_head->x - SNAKE_POINTS_STEP_PIXEL;
      break;
    case RIGHT:
      new_head->x = 
        (APP_IMG_WIDTH - cur_head->x < SNAKE_POINTS_STEP_PIXEL) ?
        0 : cur_head->x + SNAKE_POINTS_STEP_PIXEL;
      break;
    default:
      break;
  }
}

static void Snake_Lengthen(SnakeHandle *handle) {
  RINGBUFF_T *ring_buf = &(handle->snake_container);
  SnakePixelPos cur_head, new_head;
  RingBuffer_GetHead(ring_buf, &cur_head);
  Snake_GetNewHeadPos(&cur_head, handle->move_direct, &new_head);
  RingBuffer_Insert(ring_buf, (void *)&new_head);
}


/*******************************************************************************
 * Public
 ******************************************************************************/
void Snake_Draw(SnakeCtr sc){
  RINGBUFF_T *snake_map = &((SnakeHandle *)sc)->snake_container;
  RingBuffer_Seek(snake_map, RING_BUF_TAIL);
  for (int i = 0; i < RingBuffer_GetCount(snake_map); i++) {
    SnakePixelPos point;
    RingBuffer_GetItem(snake_map, (void *)&point);
    DrawUtil_DrawPoint(point.y, point.x);
  }
}

void Snake_Move(SnakeCtr sc) {
  SnakeHandle *handle = (SnakeHandle *)sc;
  SnakePixelPos temp;
  Snake_Lengthen(handle);
  RingBuffer_Pop(&(handle->snake_container), &temp);
}

SnakeCtr Snake_Init(void) {
  SnakeHandle *handle = (SnakeHandle *)malloc(sizeof(SnakeHandle) * 1);
  RingBuffer_Init(&(handle->snake_container),
                  (void *) snake_map_buf,
                  sizeof(snake_map_buf[0]),
                  SNAKE_LEN_MAX,
                  NULL);
  handle->move_direct = RIGHT;

  SnakePixelPos init_pos = {INIT_SNAKE_POS_X, INIT_SNAKE_POS_Y};
  RingBuffer_Insert(&(handle->snake_container), &init_pos);
  Snake_Lengthen(handle);
  Snake_Lengthen(handle);

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

