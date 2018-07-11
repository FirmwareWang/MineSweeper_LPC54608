/*
 * draw_util.h
 *
 *  Created on: Jul 11, 2018
 *      Author: wangjingli
 */

#ifndef IMAGE_BUF_INC_DRAW_UTIL_H_
#define IMAGE_BUF_INC_DRAW_UTIL_H_

#include <stdint.h>

#define IMG_HEIGHT 272
#define IMG_WIDTH 480

void DrawUtil_Init(void);
void DrawUtil_Point(uint16_t pos_x, uint16_t pos_y);

void DrawUtil_Draw2BPPLine(uint8_t *line, uint16_t start, uint16_t end,
                           uint8_t color);
void APP_DrawPoint(void *buffer, uint16_t pos_x, uint16_t pos_y);

#endif /* IMAGE_BUF_INC_DRAW_UTIL_H_ */
