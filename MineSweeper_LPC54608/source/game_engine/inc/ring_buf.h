/*
 * ring_buf.h
 *
 *  Created on: Jul 16, 2018
 *      Author: wangjingli
 */

#ifndef GAME_ENGINE_INC_RING_BUF_H_
#define GAME_ENGINE_INC_RING_BUF_H_

#include <stdint.h>

/**
 * @brief Ring buffer structure
 */
typedef struct {
  void *data;
  int count;
  int itemSz;
  uint32_t head;
  uint32_t tail;
  uint32_t cursor; /* track tail by default */
  void *(*copy)(void *dst, const void *src, uint32_t len);
} RINGBUFF_T;

typedef enum {
  RING_BUF_TAIL = 0,
  RING_BUF_HEAD,
} RingBufWhence;

/**
 * @def		RB_VHEAD(rb)
 * volatile typecasted head index
 */
#define RB_VHEAD(rb) (*(volatile uint32_t *)&(rb)->head)

/**
 * @def		RB_VTAIL(rb)
 * volatile typecasted tail index
 */
#define RB_VTAIL(rb) (*(volatile uint32_t *)&(rb)->tail)

/**
 * @brief	Initialize ring buffer
 * @param	RingBuff	: Pointer to ring buffer to initialize
 * @param	buffer		: Pointer to buffer to associate with RingBuff
 * @param	itemSize	: Size of each buffer item size
 * @param	count		: Size of ring buffer
 * @param	cpyFunc		: Call-back function that copies data (if NULL
 * library @a memcpy will be used)
 * @note	Memory pointed by @a buffer must have correct alignment of
 *          @a itemSize, and @a count must be a power of 2 and must at
 *          least be 2 or greater. @a len of the @a cpyFunc is in bytes.
 * @return	Nothing
 */
int RingBuffer_Init(RINGBUFF_T *RingBuff, void *buffer, int itemSize, 
  int count, void *(*cpyFunc)(void *dst, const void *src, uint32_t len));

/**
 * @brief	Resets the ring buffer to empty
 * @param	RingBuff	: Pointer to ring buffer
 * @return	Nothing
 */
static inline void RingBuffer_Flush(RINGBUFF_T *RingBuff) {
  RingBuff->head = RingBuff->tail = 0;
}

/**
 * @brief	Return size the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	Size of the ring buffer in bytes
 */
static inline int RingBuffer_GetSize(RINGBUFF_T *RingBuff) {
  return RingBuff->count;
}

/**
 * @brief	Return number of items in the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	Number of items in the ring buffer
 */
static inline int RingBuffer_GetCount(RINGBUFF_T *RingBuff) {
  return RB_VHEAD(RingBuff) - RB_VTAIL(RingBuff);
}

/**
 * @brief	Return number of free items in the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	Number of free items in the ring buffer
 */
static inline int RingBuffer_GetFree(RINGBUFF_T *RingBuff) {
  return RingBuff->count - RingBuffer_GetCount(RingBuff);
}

/**
 * @brief	Return number of items in the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	1 if the ring buffer is full, otherwise 0
 */
static inline int RingBuffer_IsFull(RINGBUFF_T *RingBuff) {
  return RingBuffer_GetCount(RingBuff) >= RingBuff->count;
}

/**
 * @brief	Return empty status of ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @return	1 if the ring buffer is empty, otherwise 0
 */
static inline int RingBuffer_IsEmpty(RINGBUFF_T *RingBuff) {
  return RB_VHEAD(RingBuff) == RB_VTAIL(RingBuff);
}

/**
 * @brief	Insert a single item into ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	data		: pointer to item
 * @return	1 when successfully inserted,
 *			0 on error (Buffer not initialized using
 *			RingBuffer_Init() or attempted to insert
 *			when buffer is full)
 */
int RingBuffer_Insert(RINGBUFF_T *RingBuff, const void *data);

/**
 * @brief	Pop an item from the ring buffer
 * @param	RingBuff	: Pointer to ring buffer
 * @param	data		: Pointer to memory where popped item be stored
 * @return	1 when item popped successfuly onto @a data,
 *          0 When error (Buffer not initialized using
 *          RingBuffer_Init() or attempted to pop item when
 *          the buffer is empty)
 */
int RingBuffer_Pop(RINGBUFF_T *RingBuff, void *data);

int RingBuffer_GetHead(RINGBUFF_T *RingBuff, void *data);

void RingBuffer_Seek(RINGBUFF_T *RingBuff, RingBufWhence whence);

int RingBuffer_GetItem(RINGBUFF_T *RingBuff, void *data);

#endif /* GAME_ENGINE_INC_RING_BUF_H_ */
