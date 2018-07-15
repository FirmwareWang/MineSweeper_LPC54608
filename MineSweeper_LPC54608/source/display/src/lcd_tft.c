/*
 * lcd_tft.c
 *
 *  Created on: Jul 7, 2018
 *      Author: wangjingli
 */

#include "display_config.h"
#include "fsl_lcdc.h"
#include "fsl_sctimer.h"
#include "pin_mux.h"
#include "board.h"
#include <stdio.h>
#include <string.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LCD_PANEL_CLK 9000000
#define LCD_PPL IMG_WIDTH
#define LCD_HSW 2
#define LCD_HFP 8
#define LCD_HBP 43
#define LCD_LPP IMG_HEIGHT
#define LCD_VSW 10
#define LCD_VFP 4
#define LCD_VBP 12
#define LCD_POL_FLAGS kLCDC_InvertVsyncPolarity | kLCDC_InvertHsyncPolarity
#define LCD_INPUT_CLK_FREQ CLOCK_GetFreq(kCLOCK_LCD)

/*******************************************************************************
 * Variables
 ******************************************************************************/

static const uint32_t palette[] = {0x001F0000U, 0x7C0003E0U};

/* The new frame address already loaded to the LCD controller. */
static volatile bool s_frameAddrUpdated = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void LCD_InitBacklightPwm(void) {
  sctimer_config_t config;
  sctimer_pwm_signal_param_t pwmParam;
  uint32_t event;

  CLOCK_AttachClk(kMCLK_to_SCT_CLK);

  CLOCK_SetClkDiv(kCLOCK_DivSctClk, 2, true);

  SCTIMER_GetDefaultConfig(&config);

  SCTIMER_Init(SCT0, &config);

  pwmParam.output = kSCTIMER_Out_5;
  pwmParam.level = kSCTIMER_HighTrue;
  pwmParam.dutyCyclePercent = 5;

  SCTIMER_SetupPwm(SCT0, &pwmParam, kSCTIMER_CenterAlignedPwm, 1000U,
                   CLOCK_GetFreq(kCLOCK_Sct), &event);
}

void LCD_IRQHandler(void) {
  uint32_t intStatus = LCDC_GetEnabledInterruptsPendingStatus(LCD);

  LCDC_ClearInterruptsStatus(LCD, intStatus);

  if (intStatus & kLCDC_BaseAddrUpdateInterrupt) {
    s_frameAddrUpdated = true;
  }
  __DSB();
}

void LCD_Setup(uint32_t panel_addr) {
  lcdc_config_t lcdConfig;

  /* Route Main clock to LCD. */
  CLOCK_AttachClk(kMCLK_to_LCD_CLK);
  CLOCK_SetClkDiv(kCLOCK_DivLcdClk, 1, true);

  /* Set the back light PWM. */
  LCD_InitBacklightPwm();

  s_frameAddrUpdated = false;

  LCDC_GetDefaultConfig(&lcdConfig);

  lcdConfig.panelClock_Hz = LCD_PANEL_CLK;
  lcdConfig.ppl = LCD_PPL;
  lcdConfig.hsw = LCD_HSW;
  lcdConfig.hfp = LCD_HFP;
  lcdConfig.hbp = LCD_HBP;
  lcdConfig.lpp = LCD_LPP;
  lcdConfig.vsw = LCD_VSW;
  lcdConfig.vfp = LCD_VFP;
  lcdConfig.vbp = LCD_VBP;
  lcdConfig.polarityFlags = LCD_POL_FLAGS;
  lcdConfig.upperPanelAddr = panel_addr;
  lcdConfig.bpp = kLCDC_2BPP;
  lcdConfig.display = kLCDC_DisplayTFT;
  lcdConfig.swapRedBlue = false;

  LCDC_Init(LCD, &lcdConfig, LCD_INPUT_CLK_FREQ);

  LCDC_SetPalette(LCD, palette, ARRAY_SIZE(palette));

  LCDC_EnableInterrupts(LCD, kLCDC_BaseAddrUpdateInterrupt);
  NVIC_EnableIRQ(LCD_IRQn);

  LCDC_Start(LCD);
  LCDC_PowerUp(LCD);
}

void LCD_Update(uint32_t buf_addr) {
  while (!s_frameAddrUpdated) {
  }

  /*
   * The buffer address has been loaded to the LCD controller, now
   * set the inactive buffer to active buffer.
   */
  LCDC_SetPanelAddr(LCD, kLCDC_UpperPanel, buf_addr);

  s_frameAddrUpdated = false;
}
