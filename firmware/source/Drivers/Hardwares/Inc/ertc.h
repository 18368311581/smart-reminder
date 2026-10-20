#ifndef __ERTC_H_
#define __ERTC_H_
#include "basic.h"
#define Ertc_scl_GPIO_Port        GPIOA
#define Ertc_scl_GPIO_Pin         GPIO_PIN_9
#define Ertc_sda_GPIO_Port        GPIOA
#define Ertc_sda_GPIO_Pin         GPIO_PIN_10
extern const char Week_names_chinese[7][3];
extern const char Week_names[7][10];
extern const char Week_short_names[7][4];
void Ertc_init(void);
void Ertc_set_year(uint16_t year);
void Ertc_set_month(uint8_t month);
void Ertc_set_day(uint8_t day);
void Ertc_set_week(uint8_t week);
void Ertc_set_hour(uint8_t hour);
void Ertc_set_minute(uint8_t minute);
void Ertc_set_second(uint8_t second);
uint16_t Ertc_get_year(void);
uint8_t Ertc_get_month(void);
uint8_t Ertc_get_day(void);
uint8_t Ertc_get_week(void);
uint8_t Ertc_get_hour(void);
uint8_t Ertc_get_minute(void);
uint8_t Ertc_get_second(void);
void Ertc_get_format_time(char *data);
void Ertc_get_format_date(char *data);
void Ertc_get_format_week_short(char *data);
void Ertc_get_format_week_chinese(char *data);
uint8_t Ertc_week_calc_from_date(uint16_t year,uint8_t month,uint8_t day);
uint32_t get_time_fat_format(void);
#endif
