#ifndef __THS_H_
#define __THS_H_
#include "basic.h"
#include "lcd.h"
#define Ths_scl_GPIO_Port        GPIOA
#define Ths_scl_GPIO_Pin         GPIO_PIN_9
#define Ths_sda_GPIO_Port        GPIOA
#define Ths_sda_GPIO_Pin         GPIO_PIN_10
extern float Ths_temperature_get;
extern float Ths_humidity_get;
extern float Ths_pressure_get;
extern float Ths_altitude_get;
void Ths_init(void);
void Ths_get(void);
#endif
