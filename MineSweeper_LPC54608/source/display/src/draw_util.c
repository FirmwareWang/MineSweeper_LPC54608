/*
 * draw_util.c
 *
 *  Created on: Jul 11, 2018
 *      Author: wangjingli
 */

#include "draw_util.h"
#include "display_config.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BYTES_PER_LINE (IMG_WIDTH / PIXEL_PER_BYTE)

typedef uint8_t(*LineBuf)[BYTES_PER_LINE];

/*******************************************************************************
 * Variables
 ******************************************************************************/

__attribute__((aligned(8)))
static uint8_t s_frameBuf0[IMG_HEIGHT][BYTES_PER_LINE];

__attribute__((aligned(8)))
static uint8_t s_frameBuf1[IMG_HEIGHT][BYTES_PER_LINE];

const static uint32_t frame_buf_addr[] = {(uint32_t)s_frameBuf0,
                                          (uint32_t)s_frameBuf1};

static uint8_t inactive_buf_idx = 1;

/*******************************************************************************
 * Private
 ******************************************************************************/

/*******************************************************************************
 * Public
 ******************************************************************************/
void DrawUtil_DrawFrameDone(void) {
  inactive_buf_idx ^= 1U;
}

uint32_t DrawUtil_InactFrameAddr(void) {
  return frame_buf_addr[inactive_buf_idx];
}

static void DrawUtil_Draw2BPPLine(uint8_t *line, uint16_t start, 
                                  uint16_t end, uint8_t color) {
  uint8_t i;
  uint16_t startByte;
  uint16_t endByte;

  startByte = start / PIXEL_PER_BYTE;
  endByte = end / PIXEL_PER_BYTE;

  if (startByte == endByte) {
    for (i = (start & 0x03U); i < (end & 0x03U); i++) {
      line[startByte] =
          (line[startByte] & ~(0x03U << (i * 2U))) | (color << (i * 2U));
    }
  } else {
    for (i = (start & 0x03U); i < PIXEL_PER_BYTE; i++) {
      line[startByte] =
          (line[startByte] & ~(0x03U << (i * 2U))) | (color << (i * 2U));
    }

    for (i = (startByte + 1U); i < endByte; i++) {
      line[i] = color * 0x55U;
    }

    for (i = 0U; i < (end & 0x03U); i++) {
      line[endByte] =
          (line[endByte] & ~(0x03U << (i * 2U))) | (color << (i * 2));
    }
  }
}


void DrawUtil_DrawPoint(const DrawPos *pos) {
  /* Foreground color. */
  static uint8_t fgColor = 1U;

  LineBuf buf = (LineBuf)frame_buf_addr[inactive_buf_idx];

  for (int i = pos->y; i < pos->y + POINT_SIZE_PIXEL; i++) {
    DrawUtil_Draw2BPPLine((uint8_t *)buf[i], pos->x, 
                          pos->x + POINT_SIZE_PIXEL, fgColor);
  }
}

void DrawUtil_FillBackGroundColor(void) {
  uint8_t bg_color = 0;

  LineBuf buf = (LineBuf)frame_buf_addr[inactive_buf_idx];

  for(int j = 0; j < IMG_HEIGHT; j++) {
    DrawUtil_Draw2BPPLine((uint8_t *)buf[j], 0, IMG_WIDTH, bg_color);
  }
}
