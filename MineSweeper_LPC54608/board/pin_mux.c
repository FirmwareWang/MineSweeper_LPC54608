/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: LPC54608J512
package_id: LPC54608J512ET180
mcu_data: ksdk2_0
processor_version: 0.0.11
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_iocon.h"

/* FUNCTION
 * ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END
 * ****************************************************************************************************************/
void BOARD_InitBootPins(void) { BOARD_InitPins(); }

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: B13, peripheral: FLEXCOMM0, signal: RXD_SDA_MOSI, pin_signal: PIO0_29/FC0_RXD_SDA_MOSI/CTIMER2_MAT3/SCT0_OUT8/TRACEDATA(2), mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: A2, peripheral: FLEXCOMM0, signal: TXD_SCL_MISO, pin_signal: PIO0_30/FC0_TXD_SCL_MISO/CTIMER0_MAT0/SCT0_OUT9/TRACEDATA(1), mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION
 * ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical
 * features.
 *
 * END
 * ****************************************************************************************************************/
/* Function assigned for the Core #0 (ARM Cortex-M4) */
void BOARD_InitPins(void) {
  /* Enables the clock for the IOCON block. 0 = Disable; 1 = Enable.: 0x01u */
  CLOCK_EnableClock(kCLOCK_Iocon);

  const uint32_t port0_pin29_config =
      (/* Pin is configured as FC0_RXD_SDA_MOSI */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT0 PIN29 (coords: B13) is configured as FC0_RXD_SDA_MOSI */
  IOCON_PinMuxSet(IOCON, 0U, 29U, port0_pin29_config);

  const uint32_t port0_pin30_config =
      (/* Pin is configured as FC0_TXD_SCL_MISO */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT0 PIN30 (coords: A2) is configured as FC0_TXD_SCL_MISO */
  IOCON_PinMuxSet(IOCON, 0U, 30U, port0_pin30_config);

  const uint32_t port2_pin11_config =
      (/* Pin is configured as LCD_PWR */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT2 PIN11 (coords: K3) is configured as LCD_PWR */
  IOCON_PinMuxSet(IOCON, 2U, 11U, port2_pin11_config);

  const uint32_t port2_pin13_config =
      (/* Pin is configured as LCD_DCLK */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT2 PIN13 (coords: P7) is configured as LCD_DCLK */
  IOCON_PinMuxSet(IOCON, 2U, 13U, port2_pin13_config);

  const uint32_t port2_pin14_config =
      (/* Pin is configured as LCD_FP */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT2 PIN14 (coords: L7) is configured as LCD_FP */
  IOCON_PinMuxSet(IOCON, 2U, 14U, port2_pin14_config);

  const uint32_t port2_pin15_config =
      (/* Pin is configured as LCD_AC */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT2 PIN15 (coords: M8) is configured as LCD_AC */
  IOCON_PinMuxSet(IOCON, 2U, 15U, port2_pin15_config);

  const uint32_t port2_pin16_config =
      (/* Pin is configured as LCD_LP */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT2 PIN16 (coords: L8) is configured as LCD_LP */
  IOCON_PinMuxSet(IOCON, 2U, 16U, port2_pin16_config);

  const uint32_t port2_pin21_config =
      (/* Pin is configured as LCD_VD(3) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT2 PIN21 (coords: L10) is configured as LCD_VD(3) */
  IOCON_PinMuxSet(IOCON, 2U, 21U, port2_pin21_config);

  const uint32_t port2_pin22_config =
      (/* Pin is configured as LCD_VD(4) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT2 PIN22 (coords: K10) is configured as LCD_VD(4) */
  IOCON_PinMuxSet(IOCON, 2U, 22U, port2_pin22_config);

  const uint32_t port2_pin23_config =
      (/* Pin is configured as LCD_VD(5) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT2 PIN23 (coords: M14) is configured as LCD_VD(5) */
  IOCON_PinMuxSet(IOCON, 2U, 23U, port2_pin23_config);

  const uint32_t port2_pin24_config =
      (/* Pin is configured as LCD_VD(6) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT2 PIN24 (coords: K14) is configured as LCD_VD(6) */
  IOCON_PinMuxSet(IOCON, 2U, 24U, port2_pin24_config);

  const uint32_t port2_pin25_config =
      (/* Pin is configured as LCD_VD(7) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT2 PIN25 (coords: J11) is configured as LCD_VD(7) */
  IOCON_PinMuxSet(IOCON, 2U, 25U, port2_pin25_config);

  const uint32_t port2_pin28_config =
      (/* Pin is configured as LCD_VD(10) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT2 PIN28 (coords: G13) is configured as LCD_VD(10) */
  IOCON_PinMuxSet(IOCON, 2U, 28U, port2_pin28_config);

  const uint32_t port2_pin29_config =
      (/* Pin is configured as LCD_VD(11) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT2 PIN29 (coords: G11) is configured as LCD_VD(11) */
  IOCON_PinMuxSet(IOCON, 2U, 29U, port2_pin29_config);

  const uint32_t port2_pin30_config =
      (/* Pin is configured as LCD_VD(12) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT2 PIN30 (coords: F12) is configured as LCD_VD(12) */
  IOCON_PinMuxSet(IOCON, 2U, 30U, port2_pin30_config);

  const uint32_t port2_pin31_config =
      (/* Pin is configured as LCD_VD(13) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT2 PIN31 (coords: D14) is configured as LCD_VD(13) */
  IOCON_PinMuxSet(IOCON, 2U, 31U, port2_pin31_config);

  const uint32_t port3_pin0_config =
      (/* Pin is configured as LCD_VD(14) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT3 PIN0 (coords: D12) is configured as LCD_VD(14) */
  IOCON_PinMuxSet(IOCON, 3U, 0U, port3_pin0_config);

  const uint32_t port3_pin1_config =
      (/* Pin is configured as LCD_VD(15) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT3 PIN1 (coords: D11) is configured as LCD_VD(15) */
  IOCON_PinMuxSet(IOCON, 3U, 1U, port3_pin1_config);

  const uint32_t port3_pin5_config =
      (/* Pin is configured as LCD_VD(19) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT3 PIN5 (coords: B10) is configured as LCD_VD(19) */
  IOCON_PinMuxSet(IOCON, 3U, 5U, port3_pin5_config);

  const uint32_t port3_pin6_config =
      (/* Pin is configured as LCD_VD(20) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT3 PIN6 (coords: C9) is configured as LCD_VD(20) */
  IOCON_PinMuxSet(IOCON, 3U, 6U, port3_pin6_config);

  const uint32_t port3_pin7_config =
      (/* Pin is configured as LCD_VD(21) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT3 PIN7 (coords: B8) is configured as LCD_VD(21) */
  IOCON_PinMuxSet(IOCON, 3U, 7U, port3_pin7_config);

  const uint32_t port3_pin8_config =
      (/* Pin is configured as LCD_VD(22) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT3 PIN8 (coords: A7) is configured as LCD_VD(22) */
  IOCON_PinMuxSet(IOCON, 3U, 8U, port3_pin8_config);

  const uint32_t port3_pin9_config =
      (/* Pin is configured as LCD_VD(23) */
       IOCON_PIO_FUNC1 |
       /* No addition pin function */
       IOCON_PIO_MODE_INACT |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* Standard mode, output slew rate control is enabled */
       IOCON_PIO_SLEW_STANDARD |
       /* Open drain is disabled */
       IOCON_PIO_OPENDRAIN_DI);
  /* PORT3 PIN9 (coords: C7) is configured as LCD_VD(23) */
  IOCON_PinMuxSet(IOCON, 3U, 9U, port3_pin9_config);

  const uint32_t port3_pin23_config =
      (/* Pin is configured as FC2_CTS_SDA_SSEL0 */
       IOCON_PIO_FUNC1 |
       /* I2C mode */
       IOCON_PIO_I2CSLEW_I2C |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* High drive: 20 mA */
       IOCON_PIO_I2CDRIVE_HIGH |
       /* I2C 50 ns glitch filter enabled */
       IOCON_PIO_I2CFILTER_EN);
  /* PORT3 PIN23 (coords: C2) is configured as FC2_CTS_SDA_SSEL0 */
  IOCON_PinMuxSet(IOCON, 3U, 23U, port3_pin23_config);

  const uint32_t port3_pin24_config =
      (/* Pin is configured as FC2_RTS_SCL_SSEL1 */
       IOCON_PIO_FUNC1 |
       /* I2C mode */
       IOCON_PIO_I2CSLEW_I2C |
       /* Input function is not inverted */
       IOCON_PIO_INV_DI |
       /* Enables digital function */
       IOCON_PIO_DIGITAL_EN |
       /* Input filter disabled */
       IOCON_PIO_INPFILT_OFF |
       /* High drive: 20 mA */
       IOCON_PIO_I2CDRIVE_HIGH |
       /* I2C 50 ns glitch filter enabled */
       IOCON_PIO_I2CFILTER_EN);
  /* PORT3 PIN24 (coords: E2) is configured as FC2_RTS_SCL_SSEL1 */
  IOCON_PinMuxSet(IOCON, 3U, 24U, port3_pin24_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
