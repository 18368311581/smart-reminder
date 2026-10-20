#ifndef __INPUT_UI_H_
#define __INPUT_UI_H_
#include "basic.h"
#include "lcd.h"
#include "touch.h"
#include "malloc_sram.h"
#include "chinese_on_lcd.h"
struct Ui_button{
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint8_t border_width;
    uint16_t color_light;
    uint16_t color_dark;
    uint16_t color_ground;
    uint16_t color_text;
    uint16_t text_size;
    uint8_t is_pressed;
    char* text;
};
void Ui_draw_special_icon(uint16_t x,uint16_t y,uint8_t icon_type);
void Ui_draw_button_normal(struct Ui_button* button);
void Ui_draw_button_pressed(struct Ui_button* button);
void Ui_draw_button_bg(struct Ui_button* button);
void Ui_draw_button_text(struct Ui_button* button);
void Ui_draw_button_init_draw(struct Ui_button* button);
uint8_t Ui_draw_button_get(struct Ui_button* button,uint16_t x,uint16_t y);
uint8_t Ui_draw_button_get_axis_x(struct Ui_button* button,uint16_t x);
uint8_t Ui_draw_button_get_with_draw(struct Ui_button* button,uint16_t x,uint16_t y);
uint8_t Ui_message_box(char* str,uint8_t type);
void Ui_show_message(char* str,uint32_t show_ms);
#endif
