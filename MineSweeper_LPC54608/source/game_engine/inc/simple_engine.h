/*
 * simple_engine.h
 *
 *  Created on: Jul 8, 2018
 *      Author: wangjingli
 */

#ifndef GAME_ENGINE_INC_SIMPLE_ENGINE_H_
#define GAME_ENGINE_INC_SIMPLE_ENGINE_H_

#include <stdint.h>

void Engine_Init(void);

void Engine_TrackPoint(uint16_t pos_x, uint16_t pos_y);

#endif /* GAME_ENGINE_INC_SIMPLE_ENGINE_H_ */
