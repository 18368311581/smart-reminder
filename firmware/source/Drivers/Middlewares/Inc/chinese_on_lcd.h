#ifndef __CHINESE_ON_LCD_H_
#define __CHINESE_ON_LCD_H_
#include "basic.h"
#include "chinese_in_norflash.h"
#include "lcd.h"
#include "norflash.h"
#include "malloc_sram.h"
void Lcd_draw_chr_chinese(uint16_t x,uint16_t y,uint16_t chr,uint8_t size,uint16_t color,uint8_t is_float,uint16_t bgcolor);
void Lcd_draw_str_chinese(uint16_t x,uint16_t y,char* str,uint8_t size,uint16_t color,uint8_t is_float,uint16_t bgcolor);
void Lcd_draw_str_invertly(uint16_t x,uint16_t y,char* str,uint8_t size);
#endif
