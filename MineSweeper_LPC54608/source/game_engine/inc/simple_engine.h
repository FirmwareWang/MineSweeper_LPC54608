/*
 * simple_engine.h
 *
 *  Created on: Jul 8, 2018
 *      Author: wangjingli
 */

#ifndef GAME_ENGINE_INC_SIMPLE_ENGINE_H_
#define GAME_ENGINE_INC_SIMPLE_ENGINE_H_

#include <stdint.h>

#define IMG_HEIGHT 272
#define IMG_WIDTH 480

#define APP_BIT_PER_PIXEL 2
#define APP_PIXEL_PER_BYTE 4
#define APP_PIXEL_MAX_VALUE 3
#define APP_PIXEL_MIN_VALUE 3

void APP_DrawPoint(void *buffer, uint16_t pos_x, uint16_t pos_y);

#endif /* GAME_ENGINE_INC_SIMPLE_ENGINE_H_ */
