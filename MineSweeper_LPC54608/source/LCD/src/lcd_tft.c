/*
 * lcd_tft.c
 *
 *  Created on: Jul 7, 2018
 *      Author: wangjingli
 */

#include <stdio.h>
#include <string.h>
#include "simple_engine.h"
#include "board.h"
#include "fsl_lcdc.h"
#include "fsl_sctimer.h"
#include "pin_mux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_LCD LCD
#define LCD_PANEL_CLK 9000000
#define LCD_PPL 480
#define LCD_HSW 2
#define LCD_HFP 8
#define LCD_HBP 43
#define LCD_LPP 272
#define LCD_VSW 10
#define LCD_VFP 4
#define LCD_VBP 12
#define LCD_POL_FLAGS kLCDC_InvertVsyncPolarity | kLCDC_InvertHsyncPolarity
#define LCD_INPUT_CLK_FREQ CLOCK_GetFreq(kCLOCK_LCD)
#define APP_LCD_IRQHandler LCD_IRQHandler
#define APP_LCD_IRQn LCD_IRQn

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if (defined(__CC_ARM) || defined(__GNUC__))
__attribute__((aligned(8)))
#elif defined(__ICCARM__)
#pragma data_alignment = 8
#else
#error Toolchain not support.
#endif
static uint8_t s_frameBuf0[IMG_HEIGHT][IMG_WIDTH / APP_PIXEL_PER_BYTE];

#if (defined(__CC_ARM) || defined(__GNUC__))
__attribute__((aligned(8)))
#elif defined(__ICCARM__)
#pragma data_alignment = 8
#else
#error Toolchain not support.
#endif
static uint8_t s_frameBuf1[IMG_HEIGHT][IMG_WIDTH / APP_PIXEL_PER_BYTE];

static const uint32_t s_frameBufAddr[] = {(uint32_t)s_frameBuf0,
                                          (uint32_t)s_frameBuf1};

static const uint32_t palette[] = {0x001F0000U, 0x7C0003E0U};

/* The index of the inactive buffer. */
static volatile uint8_t s_inactiveBufsIdx;

/* The new frame address already loaded to the LCD controller. */
static volatile bool s_frameAddrUpdated = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_InitPWM(void) {
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

void APP_LCD_IRQHandler(void) {
  uint32_t intStatus = LCDC_GetEnabledInterruptsPendingStatus(APP_LCD);

  LCDC_ClearInterruptsStatus(APP_LCD, intStatus);

  if (intStatus & kLCDC_BaseAddrUpdateInterrupt) {
    s_frameAddrUpdated = true;
  }
  __DSB();
  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate
    overlapping exception return operation might vector to incorrect interrupt
  */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
  __DSB();
#endif
}

void LCD_Setup(void) {
  lcdc_config_t lcdConfig;

  /* Route Main clock to LCD. */
  CLOCK_AttachClk(kMCLK_to_LCD_CLK);
  CLOCK_SetClkDiv(kCLOCK_DivLcdClk, 1, true);

  /* Set the back light PWM. */
  BOARD_InitPWM();

  s_frameAddrUpdated = false;
  s_inactiveBufsIdx = 1;

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
  lcdConfig.upperPanelAddr = (uint32_t)s_frameBufAddr[0];
  lcdConfig.bpp = kLCDC_2BPP;
  lcdConfig.display = kLCDC_DisplayTFT;
  lcdConfig.swapRedBlue = false;

  LCDC_Init(APP_LCD, &lcdConfig, LCD_INPUT_CLK_FREQ);

  LCDC_SetPalette(APP_LCD, palette, ARRAY_SIZE(palette));

  LCDC_EnableInterrupts(APP_LCD, kLCDC_BaseAddrUpdateInterrupt);
  NVIC_EnableIRQ(APP_LCD_IRQn);

  LCDC_Start(APP_LCD);
  LCDC_PowerUp(APP_LCD);
}

void LCD_RunExample(void) {
  APP_DrawPoint((void *)s_frameBufAddr[s_inactiveBufsIdx], 16, 16);

  while (!s_frameAddrUpdated) {
  }

  /*
   * The buffer address has been loaded to the LCD controller, now
   * set the inactive buffer to active buffer.
   */
  LCDC_SetPanelAddr(APP_LCD, kLCDC_UpperPanel,
                    (uint32_t)(s_frameBufAddr[s_inactiveBufsIdx]));

  s_frameAddrUpdated = false;
  s_inactiveBufsIdx ^= 1U;
}
