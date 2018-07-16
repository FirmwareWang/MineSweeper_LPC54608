/*
 * rtos_app.c
 *
 *  Created on: Jul 8, 2018
 *      Author: wangjingli
 */

#include "snake_engine.h"
#include "touch_screen.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "task.h"
#include "LPC54608.h"
#include <stdio.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define POS_READY (1 << 0)

#define RTOS_ACT 1

/*******************************************************************************
 * Globals
 ******************************************************************************/
static volatile TouchScreenPos pos;

static xTaskHandle lcd_task_handle;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void GameController(void *pvParameters) {
  SnakeHandle sh = Snake_Init();

  uint32_t ulNotifiedValue;
  while(1) {
    xTaskNotifyWait(0x00,
                    0xFFFFFFFFUL,
                    &ulNotifiedValue,
                    configTICK_RATE_HZ);
    if ((ulNotifiedValue & POS_READY) != 0) {
      // The coordinate in touch screen is inverse to that in LCD
      Snake_TransPosToDirect(sh, pos.pos_y, pos.pos_x);
    }
    Snake_ControlPoint(sh);
    vTaskDelay(1);
  }
}

#if 0
static void LcdTask(void *pvParameters) {
  LCD_Setup();

  uint32_t ulNotifiedValue;
  while(1) {
    xTaskNotifyWait(0x00,
                    0xFFFFFFFFUL,
                    &ulNotifiedValue,
                    portMAX_DELAY );
    if( ( ulNotifiedValue & POS_READY ) != 0 ) {
      LCD_Update(pos.pos_y, pos.pos_x);
    }
    vTaskDelay(1);
  }
}
#endif

static void GetTouchPointTask(void *pvParameters) {
  TS_Init();
  while (1) {
    if(TS_GetSingleTouch((TouchScreenPos *)&pos)) {
      xTaskNotify(lcd_task_handle, POS_READY, eSetBits);
    }
    vTaskDelay(configTICK_RATE_HZ / 2);
  }
}

#if 0
static void PrintTask(void *pvParameters) {
  while (1) {
    printf("test\n");
    vTaskDelay(configTICK_RATE_HZ / 2);
  }
}
#endif

void RTOS_RunApp(void) {
#if (RTOS_ACT == 1)
  xTaskCreate(GameController, "GameController", configMINIMAL_STACK_SIZE, NULL,
              (tskIDLE_PRIORITY + 1UL), &lcd_task_handle);

  xTaskCreate(GetTouchPointTask, "GetTouchPointTask", 
              configMINIMAL_STACK_SIZE, NULL,
              (tskIDLE_PRIORITY + 1UL), (xTaskHandle *)NULL);

  /* Start the scheduler */
  vTaskStartScheduler();
#else
  GetTouchPointTask(NULL);
#endif
}
