/*
 * draw_util.h
 *
 *  Created on: Jul 11, 2018
 *      Author: wangjingli
 */

#ifndef IMAGE_BUF_INC_DRAW_UTIL_H_
#define IMAGE_BUF_INC_DRAW_UTIL_H_

#include <stdint.h>

void DrawUtil_DrawFrameDone(void);

uint32_t DrawUtil_InactFrameAddr(void);

/**
 * @Note
 * (x,y)
 *   |
 *   ***
 *   ***
 *   ***
 *  The top-left pixel locate at (pos_x, pos_y).
 * 
 *  @Parameter point_size The width and height of point in pixel.
 */
void DrawUtil_DrawPoint(uint16_t pos_x, uint16_t pos_y, uint16_t point_size);

void DrawUtil_FillBackGroundColor(void);

#endif /* IMAGE_BUF_INC_DRAW_UTIL_H_ */
