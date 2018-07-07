/*
 * touch_screen.c
 *
 *  Created on: Jul 7, 2018
 *      Author: wangjingli
 */

#include "touch_screen.h"
#include "LPC54608.h"
#include "fsl_ft5406.h"
#include "fsl_gpio.h"
#include "fsl_i2c.h"

#define I2C_MASTER_CLOCK_FREQUENCY (12000000)
#define TOUCH_SCREEN_I2C ((I2C_Type *)I2C2_BASE)
#define TOUCH_SCREEN_I2C_CLK (kFRO12M_to_FLEXCOMM2)
#define I2C_BAUDRATE 100000U

static ft5406_handle_t touch_handle;

static status_t TS_I2cInit(void) {
  i2c_master_config_t masterConfig;

  I2C_MasterGetDefaultConfig(&masterConfig);

  /* Change the default baudrate configuration */
  masterConfig.baudRate_Bps = I2C_BAUDRATE;

  /* Initialize the I2C master peripheral */
  I2C_MasterInit(TOUCH_SCREEN_I2C, &masterConfig, I2C_MASTER_CLOCK_FREQUENCY);

  return kStatus_Success;
}

void TS_Init(void) {
  CLOCK_AttachClk(TOUCH_SCREEN_I2C_CLK);
  status_t status = TS_I2cInit();
  assert(status == kStatus_Success);

  status = FT5406_Init(&touch_handle, TOUCH_SCREEN_I2C);
  assert(status == kStatus_Success);
}

bool TS_GetSingleTouch(TouchScreenPos *pos) {
  touch_event_t touch_event;
  status_t status = FT5406_GetSingleTouch(&touch_handle, &touch_event,
                                          &pos->pos_x, &pos->pos_y);
  assert(status == kStatus_Success);

  return (touch_event == kTouch_Down) || (touch_event == kTouch_Contact) ?
          true : false;
}
