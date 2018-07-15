/*
 * lcd_tft.h
 *
 *  Created on: Jul 7, 2018
 *      Author: wangjingli
 */

#ifndef LCD_INC_LCD_TFT_H_
#define LCD_INC_LCD_TFT_H_

#include <stdint.h>

#define IMG_HEIGHT 272
#define IMG_WIDTH 480

void LCD_Setup(uint32_t panel_addr);

void LCD_Update(uint32_t buf_addr);

#endif /* LCD_INC_LCD_TFT_H_ */
