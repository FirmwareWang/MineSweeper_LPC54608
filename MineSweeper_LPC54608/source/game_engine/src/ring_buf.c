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
#define RB_INDC(rb) ((rb)->cursor & ((rb)->count - 1))
#define CURSOR_HEAD(rb) ((rb)->head - 1)

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
  RingBuff->head = RingBuff->tail = RingBuff->cursor = 0;
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
  RingBuff->cursor++;

  return 1;
}

void RingBuffer_Seek(RINGBUFF_T *RingBuff, RingBufWhence whence) {
  switch (whence) {
    case RING_BUF_HEAD:
      RingBuff->cursor = CURSOR_HEAD(RingBuff);
      break;
    case RING_BUF_TAIL:
      RingBuff->cursor = RingBuff->tail;
      break;
    default:
      break;
  }
}

int RingBuffer_GetItem(RINGBUFF_T *RingBuff, void *data){
  uint8_t *ptr = RingBuff->data;

  if (RingBuffer_IsEmpty(RingBuff)) {
    return 0;
  }
  if ((RingBuff->cursor < RingBuff->tail) ||
      (RingBuff->cursor > CURSOR_HEAD(RingBuff))){
    return 0;
  }

  ptr += RB_INDC(RingBuff) * RingBuff->itemSz;
  RingBuff->copy(data, ptr, RingBuff->itemSz);
  // TODO: need to impove
  RingBuff->cursor = (RingBuff->cursor == CURSOR_HEAD(RingBuff)) ? 
                      CURSOR_HEAD(RingBuff) : RingBuff->cursor + 1;

  return 1;
}

// TODO: add the unit test for ring buffer API
// TODO: this function will change the status of cursor
int RingBuffer_GetHead(RINGBUFF_T *RingBuff, void *data) {
  RingBuffer_Seek(RingBuff, RING_BUF_HEAD);

  return RingBuffer_GetItem(RingBuff, data);
}
