/*
 * simple_engine.c
 *
 *  Created on: Jul 8, 2018
 *      Author: wangjingli
 */

#include "simple_engine.h"
#include <stdint.h>

static void APP_Draw2BPPLine(uint8_t *line, uint16_t start, uint16_t end,
                             uint8_t color) {
  uint8_t i;
  uint16_t startByte;
  uint16_t endByte;

  startByte = start / APP_PIXEL_PER_BYTE;
  endByte = end / APP_PIXEL_PER_BYTE;

  if (startByte == endByte) {
    for (i = (start & 0x03U); i < (end & 0x03U); i++) {
      line[startByte] =
          (line[startByte] & ~(0x03U << (i * 2U))) | (color << (i * 2U));
    }
  } else {
    for (i = (start & 0x03U); i < APP_PIXEL_PER_BYTE; i++) {
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

void APP_DrawPoint(void *buffer, uint16_t pos_x, uint16_t pos_y) {
  /* Background color. */
  static uint8_t bgColor = 0U;
  /* Foreground color. */
  static uint8_t fgColor = 1U;
  uint8_t colorToSet = 0U;

  uint32_t i, j;
  uint8_t(*buf)[IMG_WIDTH / APP_PIXEL_PER_BYTE] = buffer;

  /* Fill the frame buffer. */
  /* Fill area 1. */
  colorToSet = bgColor * 0x55U;
  for (i = 0; i < pos_y; i++) {
    for (j = 0; j < IMG_WIDTH / APP_PIXEL_PER_BYTE; j++) {
      /* Background color. */
      buf[i][j] = colorToSet;
    }
  }

  APP_Draw2BPPLine((uint8_t *)buf[i], 0, pos_x, bgColor);
  APP_Draw2BPPLine((uint8_t *)buf[i], pos_x, pos_x + 3, fgColor);
  APP_Draw2BPPLine((uint8_t *)buf[i], pos_x + 3, IMG_WIDTH, bgColor);
  
  for (i++; i <= pos_y + 3; i++) {
    for (j = 0; j < (IMG_WIDTH / APP_PIXEL_PER_BYTE); j++) {
      buf[i][j] = buf[pos_y][j];
    }
  }

  /* Fill area 4. */
  colorToSet = bgColor * 0x55U;
  for (; i < IMG_HEIGHT; i++) {
    for (j = 0; j < IMG_WIDTH / APP_PIXEL_PER_BYTE; j++) {
      /* Background color. */
      buf[i][j] = colorToSet;
    }
  }
}
