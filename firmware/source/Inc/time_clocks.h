#ifndef __TIME_CLOCKS_H_
#define __TIME_CLOCKS_H_
#include "app.h"
#include "data_base.h"
#define data_update_cycle 90
extern volatile uint8_t second_signal;
uint8_t Time_is_normal_check(void);
void Time_draw(void);
void Time_update(void);
void TIM2_Init(void);
void TIM2_Start(void);
void TIM2_Stop(void);
void TIM3_Init(void);
void sleep_delay_ms(uint32_t nms);
#endif
