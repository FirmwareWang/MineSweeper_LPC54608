/*
 * touch_sreen.h
 *
 *  Created on: Jul 7, 2018
 *      Author: wangjingli
 */

#ifndef TOUCH_SCREEN_INC_TOUCH_SCREEN_H_
#define TOUCH_SCREEN_INC_TOUCH_SCREEN_H_

#include <stdbool.h>

typedef struct _TouchScreenPos {
  int pos_x;
  int pos_y;
} TouchScreenPos;

void TS_Init(void);

bool TS_GetSingleTouch(TouchScreenPos *pos);

#endif /* TOUCH_SCREEN_INC_TOUCH_SCREEN_H_ */
