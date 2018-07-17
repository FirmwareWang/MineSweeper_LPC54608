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
typedef void (*FetchPointFunc)(uint16_t pos_x, uint16_t pos_y);

SnakeCtr Snake_Init(FetchPointFunc FetchPoint);

void Snake_Restart(SnakeCtr sc);

bool Snake_TakeAction(SnakeCtr sc);

/**
 * The same or reverse direction will move forward quickly.
 */
void Snake_TransPosToDirect(SnakeCtr sc, 
                            uint16_t touch_pos_x, 
                            uint16_t touch_pos_y);

void Snake_Draw(SnakeCtr sc);

#endif /* GAME_ENGINE_INC_SNAKE_ENGINE_H_ */
