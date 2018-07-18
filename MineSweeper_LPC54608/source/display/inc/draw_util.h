/*
 * draw_util.h
 *
 *  Created on: Jul 11, 2018
 *      Author: wangjingli
 */

#ifndef IMAGE_BUF_INC_DRAW_UTIL_H_
#define IMAGE_BUF_INC_DRAW_UTIL_H_

#include <stdint.h>

uint32_t DrawUtil_DrawFrameDone(void);

/**
 * @Note The (base_row, base_column) locate at the top-left pixel of point.
 */
void DrawUtil_DrawPoint(uint16_t base_row, uint16_t base_column);

void DrawUtil_FillBackGroundColor(void);

#endif /* IMAGE_BUF_INC_DRAW_UTIL_H_ */
