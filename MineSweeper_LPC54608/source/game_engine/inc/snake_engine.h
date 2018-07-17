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

typedef void *SnakeCtr;

SnakeCtr Snake_Init(void);

void Snake_TransPosToDirect(SnakeCtr sc, 
                            uint16_t touch_pos_x, 
                            uint16_t touch_pos_y);

void Snake_Draw(SnakeCtr sc);

void Snake_Move(SnakeCtr sc);

#endif /* GAME_ENGINE_INC_SNAKE_ENGINE_H_ */
