#ifndef __PAGE_OTHER_H_
#define __PAGE_OTHER_H_
#include "icons.h"
#include "time_clocks.h"
uint8_t sys_close_button_get(uint16_t x,uint16_t y);
void CLK_SHW_func(void);
void Page_other_draw(void);
void Page_other_process(uint16_t x,uint16_t y);
void Random_ui_task(void);
void Ths_ui_task(void);
#endif
