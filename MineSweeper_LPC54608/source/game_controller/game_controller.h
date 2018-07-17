/*
 * game_controller.h
 *
 *  Created on: Jul 17, 2018
 *      Author: wangjingli
 */

#ifndef GAME_CONTROLLER_GAME_CONTROLLER_H_
#define GAME_CONTROLLER_GAME_CONTROLLER_H_

#include <stdint.h>
#include <stdbool.h>

typedef void *GameCtrl;

GameCtrl GameCtr_Init(void);

void GameCtr_Restart(GameCtrl gc);

bool GameCtr_Run(GameCtrl gc,
                 uint16_t touch_pos_x, 
                 uint16_t touch_pos_y,
                 bool touch_trigger);

#endif /* GAME_CONTROLLER_GAME_CONTROLLER_H_ */
