#ifndef __PAGE_SETTING_H_
#define __PAGE_SETTING_H_
#include "icons.h"
#include "data_base.h"
#include "time_clocks.h"
#define setting_password 0x6D
__packed typedef struct{
    uint8_t password;
    uint8_t lcd_light;
    uint8_t auto_exposure_level;
    uint8_t light_mode;
    uint8_t color_saturation;
    uint8_t brightness;
    uint8_t contrast;
    uint8_t crc;
}setting_pack;
extern uint8_t is_setting_show;
void SET_PRO_func(void);
void Setting_save(void);
void Setting_load(void);
void Setting_save_checkly(void);
void Lcd_reset(void);
void Page_setting_draw(void);
void Page_setting_process(uint16_t x,uint16_t y);
#endif
