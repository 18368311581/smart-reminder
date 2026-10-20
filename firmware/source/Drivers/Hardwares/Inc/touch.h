#ifndef __TOUCH_H_
#define __TOUCH_H_
#include "basic.h"
#define Touch_identification 911
#define Touch_tcs_GPIO_Port            GPIOC
#define Touch_tcs_GPIO_Pin             GPIO_PIN_13
#define Touch_pen_GPIO_Port            GPIOB
#define Touch_pen_GPIO_Pin             GPIO_PIN_1
#define Touch_scl_GPIO_Port            GPIOB
#define Touch_scl_GPIO_Pin             GPIO_PIN_0
#define Touch_sda_GPIO_Port            GPIOF
#define Touch_sda_GPIO_Pin             GPIO_PIN_11
void Touch_init(void);
uint8_t Touch_get(void);
void Touch_clear(void);
uint8_t Touch_try_now(void);
extern uint16_t Touch_get_x[5];
extern uint16_t Touch_get_y[5];
extern uint16_t Touch_get_size[5];
extern uint16_t Touch_state;
#endif
