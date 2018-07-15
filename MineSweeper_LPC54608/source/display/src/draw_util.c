/*
 * draw_util.c
 *
 *  Created on: Jul 11, 2018
 *      Author: wangjingli
 */

#include "draw_util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


/*******************************************************************************
 * Variables
 ******************************************************************************/

static volatile struct {
  DrawUtilConfig config;
  uint8_t inactive_buf_idx;
} draw_handle;

/*******************************************************************************
 * Private
 ******************************************************************************/

/*******************************************************************************
 * Public
 ******************************************************************************/
void DrawUtil_DrawFrameDone(void) {
  draw_handle.inactive_buf_idx ^= 1U;
}

uint32_t DrawUtil_InactFrameAddr(void) {
  return draw_handle.config.frame_buf_addr[draw_handle.inactive_buf_idx];
}

void DrawUtil_Init(DrawUtilConfig *config) {
  draw_handle.config = *config;
  draw_handle.inactive_buf_idx = 1;
}

static void DrawUtil_Draw2BPPLine(uint8_t *line, uint16_t start, 
                                  uint16_t end, uint8_t color) {
  uint8_t i;
  uint16_t startByte;
  uint16_t endByte;

  startByte = start / draw_handle.config.pixel_per_byte;
  endByte = end / draw_handle.config.pixel_per_byte;

  if (startByte == endByte) {
    for (i = (start & 0x03U); i < (end & 0x03U); i++) {
      line[startByte] =
          (line[startByte] & ~(0x03U << (i * 2U))) | (color << (i * 2U));
    }
  } else {
    for (i = (start & 0x03U); i < draw_handle.config.pixel_per_byte; i++) {
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


void DrawUtil_DrawPoint(uint16_t pos_x, uint16_t pos_y) {
  /* Foreground color. */
  static uint8_t fgColor = 1U;

  uint16_t width_bytes = 
    draw_handle.config.image_width / draw_handle.config.pixel_per_byte;

  uint8_t(*buf)[width_bytes] = (uint8_t(*)[width_bytes])
    draw_handle.config.frame_buf_addr[draw_handle.inactive_buf_idx];

  for (int i = pos_y - 1; i < pos_y + 2; i++) {
    DrawUtil_Draw2BPPLine((uint8_t *)buf[i], pos_x, pos_x + 3, fgColor);
  }
}

void DrawUtil_FillBackGroundColor(void) {
  uint8_t bg_color = 0;
  uint16_t width_bytes = 
    draw_handle.config.image_width / draw_handle.config.pixel_per_byte;

  uint8_t(*buf)[width_bytes] = (uint8_t(*)[width_bytes])
    draw_handle.config.frame_buf_addr[draw_handle.inactive_buf_idx];

  for(int j = 0; j < draw_handle.config.image_height; j++) {
    DrawUtil_Draw2BPPLine((uint8_t *)buf[j], 0, 
      draw_handle.config.image_width, bg_color);
  }
}
