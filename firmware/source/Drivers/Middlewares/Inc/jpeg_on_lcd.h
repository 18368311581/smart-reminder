#ifndef __JPEG_H_
#define __JPEG_H_
#include "basic.h"
#include "lcd.h"
#include "sdcard.h"
#include "ff.h"
void Lcd_draw_jpeg_pic(char* file_path,uint16_t x,uint16_t y,uint16_t width,uint16_t height);
#endif
