/*
 * snake_engine.c
 *
 *  Created on: Jul 15, 2018
 *      Author: wangjingli
 */

#include "snake_engine.h"
#include "ring_buf.h"
#include "display_config.h"
#include "fsl_rng.h"
#include "FreeRTOS.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define INIT_SNAKE_POS_X 0
#define INIT_SNAKE_POS_Y 0
#define INIT_SNAKE_LEN   3

#define SNAKE_POINTS_PITCH_PIXEL 1
#define SNAKE_POINTS_STEP_PIXEL \
  (SNAKE_POINTS_PITCH_PIXEL + POINT_SIZE_PIXEL)
#define ROUND_TO_MULTI_PTS_STEP(val) \
  (val - (val % SNAKE_POINTS_STEP_PIXEL))
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
} MoveDirection;

typedef struct _DrawPos {
  uint16_t x;
  uint16_t y;
} SnakePixelPos;

typedef struct _SnakeHandle {
  RINGBUFF_T snake_container;
  SnakePixelPos food_pos;
  MoveDirection move_direct;
  FetchPointFunc FetchPoint;
} SnakeHandle;

/*******************************************************************************
 * Globals
 ******************************************************************************/

static SnakePixelPos snake_map_buf[SNAKE_LEN_MAX];

/*******************************************************************************
 * Pravite 
 ******************************************************************************/

static bool Snake_TwoPosIsSame(const SnakePixelPos *a,
                               const SnakePixelPos *b) {
  return (a->x == b->x) && (a->y == b->y);
}

// If the point goes out of image range, rolling it to another side
static void Snake_GetNewHeadPos(const SnakePixelPos *cur_head, 
                                MoveDirection dir,
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

static void Snake_GenInitMap(SnakeHandle *handle, uint16_t init_len) {
  // generate first point
  SnakePixelPos head = {INIT_SNAKE_POS_X, INIT_SNAKE_POS_Y};
  RingBuffer_Insert(&(handle->snake_container), &head);
  init_len--;

  SnakePixelPos new_head;
  for (int i = 0; i < init_len; i++) {
    Snake_GetNewHeadPos(&head, handle->move_direct, &new_head);
    RingBuffer_Insert(&(handle->snake_container), &new_head);
    head = new_head;
  }
}

static void Snake_GenRandomFood(SnakePixelPos *food) {
  food->x = (uint16_t)
    ROUND_TO_MULTI_PTS_STEP(RNG_GetRandomData() % APP_IMG_WIDTH);
  food->y = (uint16_t)
    ROUND_TO_MULTI_PTS_STEP(RNG_GetRandomData() % APP_IMG_HEIGHT);
}

static bool Snake_Suicide(RINGBUFF_T *snake_map, const SnakePixelPos *head) {
  RingBuffer_Seek(snake_map, RING_BUF_TAIL);
  for (int i = 0; i < RingBuffer_GetCount(snake_map); i++) {
    SnakePixelPos point;
    RingBuffer_GetItem(snake_map, (void *)&point);
    if (Snake_TwoPosIsSame(&point, head)) {
      return true;
    }
  }
  return false;
}

static bool Snake_PermittedTurning(MoveDirection pre, 
                                   MoveDirection cur) {
  if (pre == cur) {
    return false;
  }
  
  bool ret = true;
  switch (pre) {
    case UP:
      if (cur == DOWN) ret = false;
      break;
    case DOWN:
      if (cur == UP) ret = false;
      break;
    case LEFT:
      if (cur == RIGHT) ret = false;
      break;
    case RIGHT:
      if (cur == LEFT) ret = false;
      break;
    default:
      break;
  }
  return ret;
}

/*******************************************************************************
 * Public
 ******************************************************************************/
void Snake_Draw(SnakeCtr sc){
  SnakeHandle *handle = (SnakeHandle *)sc;
  RINGBUFF_T *snake_map = &(handle->snake_container);
  RingBuffer_Seek(snake_map, RING_BUF_TAIL);
  for (int i = 0; i < RingBuffer_GetCount(snake_map); i++) {
    SnakePixelPos point;
    RingBuffer_GetItem(snake_map, (void *)&point);
    handle->FetchPoint(point.x, point.y);
  }

  // Fetch the food position
  handle->FetchPoint(handle->food_pos.x, handle->food_pos.y);
}

bool Snake_TakeAction(SnakeCtr sc) {
  SnakeHandle *handle = (SnakeHandle *)sc;
  RINGBUFF_T *ring_buf = &(handle->snake_container);

  SnakePixelPos cur_head, new_head;
  RingBuffer_GetHead(ring_buf, &cur_head);
  Snake_GetNewHeadPos(&cur_head, handle->move_direct, &new_head);
  if(Snake_Suicide(ring_buf, &new_head)) {
    return false;
  }

  RingBuffer_Insert(ring_buf, (void *)&new_head);
  // Pop the tail position if succeeding to eat
  if (Snake_TwoPosIsSame(&new_head, &handle->food_pos)) {
    Snake_GenRandomFood(&handle->food_pos);
  } else {
    SnakePixelPos temp;
    RingBuffer_Pop(ring_buf, &temp);
  }

  return true;
}

SnakeCtr Snake_Init(FetchPointFunc FetchPoint) {
  // TODO: FreeRTOS Heap may be a better choise.
  SnakeHandle *handle = 
    (SnakeHandle *)pvPortMalloc(sizeof(SnakeHandle) * 1);
  RingBuffer_Init(&(handle->snake_container),
                  (void *) snake_map_buf,
                  sizeof(snake_map_buf[0]),
                  SNAKE_LEN_MAX,
                  NULL);
  handle->move_direct = RIGHT;
  handle->FetchPoint = FetchPoint;

  Snake_GenRandomFood(&handle->food_pos);

  Snake_GenInitMap(handle, INIT_SNAKE_LEN);

  return (SnakeCtr)handle;
}

void Snake_Restart(SnakeCtr sc) {
  SnakeHandle *handle = (SnakeHandle *)sc;
  RingBuffer_Flush(&handle->snake_container);
  handle->move_direct = RIGHT;
  Snake_GenInitMap(handle, INIT_SNAKE_LEN);
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

  MoveDirection cur_direct =  
    (touch_pos_x < IMG_WIDTH / 4)     ? LEFT : 
    (touch_pos_x > IMG_WIDTH * 3 / 4) ? RIGHT : 
    (touch_pos_y < IMG_HEIGHT / 2)    ? UP : 
    (touch_pos_y > IMG_HEIGHT / 2)    ? DOWN : RANGE_OUT;

  if (Snake_PermittedTurning(handle->move_direct, cur_direct)) {
    handle->move_direct = cur_direct;
  }
}

