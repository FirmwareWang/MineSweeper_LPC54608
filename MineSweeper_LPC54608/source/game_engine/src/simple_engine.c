/*
 * simple_engine.c
 *
 *  Created on: Jul 8, 2018
 *      Author: wangjingli
 */

#include "simple_engine.h"
#include "draw_util.h"
#include <stdint.h>

void Engine_Init(void) {
  DrawUtil_Init();
}

void Engine_TrackPoint(uint16_t pos_x, uint16_t pos_y) {
  DrawUtil_Point(pos_x, pos_y);
}
