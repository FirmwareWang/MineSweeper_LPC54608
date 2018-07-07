/*
 * rtos_app.c
 *
 *  Created on: Jul 8, 2018
 *      Author: wangjingli
 */

#include "lcd_tft.h"
#include "touch_screen.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "LPC54608.h"
#include <stdio.h>

#define RTOS_ACT 1

static void LcdTask(void *pvParameters) {
  LCD_Setup();

  while(1) {
    LCD_RunExample();
    vTaskDelay(1);
  }
}

static void GetTouchPointTask(void *pvParameters) {
  TouchScreenPos pos;
  TS_Init();
  while (1) {
    if(TS_GetSingleTouch(&pos)) {
      printf("posx: %d, posy: %d\n", pos.pos_x, pos.pos_y);
    }
    vTaskDelay(configTICK_RATE_HZ / 2);
  }
}

static void PrintTask(void *pvParameters) {
  while (1) {
    printf("test\n");
    vTaskDelay(configTICK_RATE_HZ / 2);
  }
}

void RTOS_RunApp(void) {
#if (RTOS_ACT == 1)
  xTaskCreate(LcdTask, "vLcdTask", configMINIMAL_STACK_SIZE, NULL,
              (tskIDLE_PRIORITY + 1UL), (xTaskHandle *)NULL);

  xTaskCreate(GetTouchPointTask, "GetTouchPointTask", 
              configMINIMAL_STACK_SIZE, NULL,
              (tskIDLE_PRIORITY + 1UL), (xTaskHandle *)NULL);

  /* Start the scheduler */
  vTaskStartScheduler();
#else
  GetTouchPointTask(NULL);
#endif
}
