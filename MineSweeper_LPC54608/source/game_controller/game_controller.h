/*
 * game_controller.h
 *
 *  Created on: Jul 17, 2018
 *      Author: wangjingli
 */

#ifndef GAME_CONTROLLER_GAME_CONTROLLER_H_
#define GAME_CONTROLLER_GAME_CONTROLLER_H_

#include "snake_engine.h"
#include <stdint.h>

SnakeCtr GameCtr_Init(void);

void GameCtr_TransPosToDirect(SnakeCtr sc, 
                              uint16_t touch_pos_x, 
                              uint16_t touch_pos_y);

void GameCtr_Run(SnakeCtr sc);

#endif /* GAME_CONTROLLER_GAME_CONTROLLER_H_ */
