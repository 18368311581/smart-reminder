#ifndef __LCD_H_
#define __LCD_H_
#include "basic.h"
#define Lcd_draw_director_L2R_U2D 0x03
#define Lcd_draw_director_R2L_U2D 0x02
#define Lcd_draw_director_U2D_R2L 0x00
#define Lcd_draw_director_D2U_R2L 0x01
#define Lcd_color_white           0xFFFF
#define Lcd_color_black           0x0000
#define Lcd_color_red             0xF800
#define Lcd_color_green           0x07E0
#define Lcd_color_blue            0x001F
#define Lcd_color_magenta         0xF81F
#define Lcd_color_yellow          0xFFE0
#define Lcd_color_cyan            0x07FF
#define Lcd_color_brown           0xBC40
#define Lcd_color_brownred        0xFC07
#define Lcd_color_gray            0x8430
#define Lcd_color_darkblue        0x01CF
#define Lcd_color_lightblue       0x7D7C
#define Lcd_color_grayblue        0x5458
#define Lcd_color_lightgreen      0x841F
#define Lcd_color_lightgray       0xC618
#define Lcd_color_lightgrayblue   0xA651
#define Lcd_color_lightbrownblue  0x2B12
#define Lcd_CS_GPIO_Port          GPIOG
#define Lcd_CS_GPIO_Pin           GPIO_PIN_12
#define Lcd_WR_GPIO_Port          GPIOD
#define Lcd_WR_GPIO_Pin           GPIO_PIN_5
#define Lcd_RD_GPIO_Port          GPIOD
#define Lcd_RD_GPIO_Pin           GPIO_PIN_4
#define Lcd_RS_GPIO_Port          GPIOF
#define Lcd_RS_GPIO_Pin           GPIO_PIN_12
#define Lcd_D0_GPIO_Port          GPIOD
#define Lcd_D0_GPIO_Pin           GPIO_PIN_14
#define Lcd_D1_GPIO_Port          GPIOD
#define Lcd_D1_GPIO_Pin           GPIO_PIN_15
#define Lcd_D2_GPIO_Port          GPIOD
#define Lcd_D2_GPIO_Pin           GPIO_PIN_0
#define Lcd_D3_GPIO_Port          GPIOD
#define Lcd_D3_GPIO_Pin           GPIO_PIN_1
#define Lcd_D4_GPIO_Port          GPIOE
#define Lcd_D4_GPIO_Pin           GPIO_PIN_7
#define Lcd_D5_GPIO_Port          GPIOE
#define Lcd_D5_GPIO_Pin           GPIO_PIN_8
#define Lcd_D6_GPIO_Port          GPIOE
#define Lcd_D6_GPIO_Pin           GPIO_PIN_9
#define Lcd_D7_GPIO_Port          GPIOE
#define Lcd_D7_GPIO_Pin           GPIO_PIN_10
#define Lcd_D8_GPIO_Port          GPIOE
#define Lcd_D8_GPIO_Pin           GPIO_PIN_11
#define Lcd_D9_GPIO_Port          GPIOE
#define Lcd_D9_GPIO_Pin           GPIO_PIN_12
#define Lcd_D10_GPIO_Port         GPIOE
#define Lcd_D10_GPIO_Pin          GPIO_PIN_13
#define Lcd_D11_GPIO_Port         GPIOE
#define Lcd_D11_GPIO_Pin          GPIO_PIN_14
#define Lcd_D12_GPIO_Port         GPIOE
#define Lcd_D12_GPIO_Pin          GPIO_PIN_15
#define Lcd_D13_GPIO_Port         GPIOD
#define Lcd_D13_GPIO_Pin          GPIO_PIN_8
#define Lcd_D14_GPIO_Port         GPIOD
#define Lcd_D14_GPIO_Pin          GPIO_PIN_9
#define Lcd_D15_GPIO_Port         GPIOD
#define Lcd_D15_GPIO_Pin          GPIO_PIN_10
#define LCD_REG (*((volatile uint16_t*)(0x6C000000)))
#define LCD_RAM (*((volatile uint16_t*)(0x6C000080)))
void Lcd_init(void);
void Lcd_set_back_light(uint8_t light_percent);
void Lcd_write_start(void);
void Lcd_write(uint16_t x);
void Lcd_set_cursor(uint16_t x,uint16_t y);
void Lcd_turn_on(void);
void Lcd_turn_off(void);
void Lcd_clear(uint16_t color);
void Lcd_set_draw_dir(uint8_t dir);
void Lcd_set_window(uint16_t x,uint16_t y,uint16_t dx,uint16_t dy);
uint16_t Lcd_get_point(uint16_t x,uint16_t y);
void Lcd_set_point(uint16_t x,uint16_t y,uint16_t color);
void Lcd_draw_line(uint16_t x,uint16_t y,int16_t dx,int16_t dy,uint16_t color);//均为左闭右开性
void Lcd_draw_line_width(uint16_t x,uint16_t y,int16_t dx,int16_t dy,uint16_t color,uint8_t width);
void Lcd_draw_rect(uint16_t x,uint16_t y,uint16_t dx,uint16_t dy,uint16_t color,uint8_t is_fill);
void Lcd_draw_circle(uint16_t x,uint16_t y,uint16_t r,uint16_t color,uint8_t is_fill);
void Lcd_draw_icon_highbit(uint16_t x,uint16_t y,uint16_t dx,uint16_t dy,uint8_t* list,uint16_t color,uint16_t bgcolor);
void Lcd_draw_icon_lowebit(uint16_t x,uint16_t y,uint16_t dx,uint16_t dy,uint8_t* list,uint16_t color,uint16_t bgcolor);
void Lcd_draw_chr(uint16_t x,uint16_t y,uint8_t chr,uint8_t size,uint16_t color,uint8_t is_float,uint16_t bgcolor);
void Lcd_draw_str(uint16_t x,uint16_t y,char*   str,uint8_t size,uint16_t color,uint8_t is_float,uint16_t bgcolor);
void Lcd_draw_chr_magnify(uint16_t x,uint16_t y,uint8_t chr,uint8_t size,uint8_t rate,uint16_t color,uint8_t is_float,uint16_t bgcolor);
void Lcd_draw_str_magnify(uint16_t x,uint16_t y,char*   str,uint8_t size,uint8_t rate,uint16_t color,uint8_t is_float,uint16_t bgcolor);
void Lcd_set_point_list(uint16_t x,uint16_t y,uint16_t dx,uint16_t dy,uint16_t *list);
#endif
