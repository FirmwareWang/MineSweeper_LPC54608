/*
 * ring_buf.c
 *
 *  Created on: Jul 16, 2018
 *      Author: wangjingli
 */

#include "ring_buf.h"
#include <string.h>

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define RB_INDH(rb) ((rb)->head & ((rb)->count - 1))
#define RB_INDT(rb) ((rb)->tail & ((rb)->count - 1))

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initialize ring buffer */
int RingBuffer_Init(RINGBUFF_T *RingBuff, 
                    void *buffer, 
                    int itemSize, int count,
                 	  void *(*cpyFunc)(void *dst, const void *src, uint32_t len)) {
  RingBuff->data = buffer;
  RingBuff->count = count;
  RingBuff->itemSz = itemSize;
  RingBuff->head = RingBuff->tail = 0;
  if (!cpyFunc) {
    cpyFunc = memcpy;
  }
  RingBuff->copy = cpyFunc;

  return 1;
}

/* Insert a single item into Ring Buffer */
int RingBuffer_Insert(RINGBUFF_T *RingBuff, const void *data) {
  uint8_t *ptr = RingBuff->data;

  /* We cannot insert when queue is full */
  if (RingBuffer_IsFull(RingBuff)) {
    return 0;
  }

  ptr += RB_INDH(RingBuff) * RingBuff->itemSz;
  RingBuff->copy(ptr, data, RingBuff->itemSz);
  RingBuff->head++;

  return 1;
}

/* Pop single item from Ring Buffer */
int RingBuffer_Pop(RINGBUFF_T *RingBuff, void *data) {
  uint8_t *ptr = RingBuff->data;

  /* We cannot pop when queue is empty */
  if (RingBuffer_IsEmpty(RingBuff)) {
    return 0;
  }

  ptr += RB_INDT(RingBuff) * RingBuff->itemSz;
  RingBuff->copy(data, ptr, RingBuff->itemSz);
  RingBuff->tail++;

  return 1;
}
