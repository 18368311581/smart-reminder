#ifndef __PHOTO_H_
#define __PHOTO_H_
#include "basic.h"
#include "camera.h"
#include "sdcard.h"
#include "lcd.h"
#include "ff.h"
#include "malloc_sram.h"
void Gpioc_set_camera_mode(void);
void Gpioc_set_sd_card_mode(void);
void Camera_draw_lcd_start(uint16_t x,uint16_t y,uint16_t dx,uint16_t dy);
void Camera_draw_lcd_stop(void);
void Camera_photo_save_jpeg(uint32_t pic_name);
#endif
