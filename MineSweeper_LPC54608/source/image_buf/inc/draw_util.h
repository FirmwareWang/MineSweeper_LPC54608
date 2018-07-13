/*
 * draw_util.h
 *
 *  Created on: Jul 11, 2018
 *      Author: wangjingli
 */

#ifndef IMAGE_BUF_INC_DRAW_UTIL_H_
#define IMAGE_BUF_INC_DRAW_UTIL_H_

#include <stdint.h>

#define FRAME_BUF_NUM 2

typedef struct _DrawUtilConfig {
  uint16_t image_width;
  uint16_t image_height;
  uint16_t pixel_per_byte;
  uint32_t frame_buf_addr[FRAME_BUF_NUM];
} DrawUtilConfig;

void DrawUtil_Init(DrawUtilConfig *config);

void DrawUtil_DrawFrameDone(void);

uint32_t DrawUtil_InactFrameAddr(void);

void APP_DrawPoint(uint16_t pos_x, uint16_t pos_y);

#endif /* IMAGE_BUF_INC_DRAW_UTIL_H_ */
