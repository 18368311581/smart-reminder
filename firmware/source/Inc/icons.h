#ifndef __ICONS_H_
#define __ICONS_H_
#include "app.h"
extern uint8_t Icon_index_now;
extern uint8_t Icon_can_press;
extern uint32_t sys_count_icon;
void circle_width3(uint16_t x,uint16_t y,uint16_t r,uint16_t color);
void Icons_init_set(uint8_t x);
void Icon_show_main_stp(void);
void Icons_draw(void);
void page_lcd_clear(uint16_t color);
void Camera_icon_draw(uint16_t x,uint16_t y,uint16_t color);
uint8_t Icons_touch_get(uint16_t x,uint16_t y);
#endif
