/*
 * snake_engine.h
 *
 *  Created on: Jul 15, 2018
 *      Author: wangjingli
 */

#ifndef GAME_ENGINE_INC_SNAKE_ENGINE_H_
#define GAME_ENGINE_INC_SNAKE_ENGINE_H_

#include <stdint.h>
#include <stdbool.h>

void Snake_Init(void);

void Snake_ControlPoint(uint16_t pos_x, uint16_t pos_y, bool pos_updated);

#endif /* GAME_ENGINE_INC_SNAKE_ENGINE_H_ */
