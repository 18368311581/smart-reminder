#include "time_clocks.h"
TIM_HandleTypeDef htim2;
volatile uint8_t second_signal=0;
static uint8_t is_time_drawing;
static volatile uint16_t sec_cnt;
static uint8_t predate=0;
static uint8_t prehour=0;
static const uint16_t time_frt=0x0000;
static const uint16_t time_bg=0x2611;
static const uint8_t time_draw_datas[4][36]={
{0xF0,0x80,0x1F,0x9C,0xC3,0x30,0x0C,0xC3,0x30,0x0C,0xC3,0x30,
 0x0C,0xC3,0x36,0x6C,0xC3,0x36,0x6C,0xC3,0x36,0x6E,0xF7,0xF6,
 0x67,0x7E,0xEF,0xF7,0x7E,0xE6,0x0F,0xEF,0x7F,0xFC,0x03,0x0F},
{0x00,0x00,0x00,0x78,0xC0,0x0C,0x84,0x40,0x08,0xCC,0x80,0x07,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x80,0x00,0x0C,0xC0,0x00,0x0E,0xE0,0x00,0x0F,0xF0,
 0x80,0x0F,0xFC,0xC0,0x0F,0xFC,0xE0,0x0F,0xFE,0xF0,0x0F,0xFF,
 0xF0,0x0F,0xFE,0xE0,0x0F,0xFC,0xC0,0x0F,0xF0,0x00,0x0E,0x00},
{0x00,0x10,0x00,0x03,0x30,0x00,0x07,0x70,0x00,0x0F,0xF0,0x00,
 0x1F,0xF0,0x03,0x3F,0xF0,0x03,0x7F,0xF0,0x07,0xFF,0xF0,0x0F,
 0xFF,0xF0,0x07,0x7F,0xF0,0x03,0x3F,0xF0,0x00,0x07,0x00,0x00}};
static void time_draws(uint16_t x,uint16_t y,uint8_t index){
    Lcd_draw_icon_lowebit(x,y,12,24,(uint8_t*)time_draw_datas[index],time_frt,time_bg);
}
uint8_t Time_is_normal_check(void){
    if(Ertc_get_year()<2020){
        Lcd_clear(0xFFFF);
        Ertc_set_year(2030);
        Ertc_set_month(1);
        Ertc_set_day(1);
        Ertc_set_hour(12);
        Ertc_set_minute(0);
        Ertc_set_second(0);
        Ui_message_box("Ê±¼ä´íÎó£¬ÇëÉèÖÃ",1);
        return 1;
    }
    Ertc_set_week(Ertc_week_calc_from_date(Ertc_get_year(),Ertc_get_month(),Ertc_get_day()));
    return 0;
}
void Time_draw(void){
    char str_temp[20];
    Lcd_draw_rect(0,0,480,30,time_bg,1);
    Lcd_draw_line(0,30,480,0,0x0000);
    Lcd_draw_line(0,31,480,0,0x0000);
    predate=Ertc_get_day();
    sprintf(str_temp,"%4d-%02d-%02d",Ertc_get_year(),Ertc_get_month(),predate);
    Lcd_draw_str(12,3,str_temp,24,time_frt,0,time_bg);
    prehour=Ertc_get_hour();
    sprintf(str_temp,"%02d:%02d",prehour,Ertc_get_minute());
    Lcd_draw_str(144,3,str_temp,24,time_frt,0,time_bg);
    sprintf(str_temp,"%2d%%",(uint8_t)(Malloc_sram_get_usage()*100));
    Lcd_draw_str(216,3,str_temp,24,0x0000,0,time_bg);
    time_draws(264,3,0);
    Ths_get();
    sprintf(str_temp,"%5.1f",Ths_temperature_get);
    Lcd_draw_str(276,3,str_temp,24,0x0000,0,time_bg);
    time_draws(336,4,1);
    Lcd_draw_chr(344,3,'C',24,0x0000,0,time_bg);
    time_draws(372,3,2);
    time_draws(384,3,3);
    sprintf(str_temp,"%5.1f",Ths_humidity_get);
    Lcd_draw_str(396,3,str_temp,24,0x0000,0,time_bg);
    Lcd_draw_chr(456,3,'%',24,0x0000,0,time_bg);
}
void Time_update(void){
    if(is_time_drawing){
        char str_temp[20];
        sprintf(str_temp,"%02d",Ertc_get_minute());
        Lcd_draw_str(180,3,str_temp,24,0x0000,0,time_bg);
        uint8_t hour_tp=Ertc_get_hour();
        if(hour_tp!=prehour){
            prehour=hour_tp;
            sprintf(str_temp,"%02d",hour_tp);
            Lcd_draw_str(144,3,str_temp,24,0x0000,0,time_bg);
            uint8_t day_tp=Ertc_get_day();
            if(day_tp!=predate){
                predate=day_tp;
                sprintf(str_temp,"%4d-%02d-%02d",Ertc_get_year(),Ertc_get_month(),predate);
                Lcd_draw_str(12,3,str_temp,24,0x0000,0,time_bg);
            }
        }
        sprintf(str_temp,"%2d%%",(uint8_t)(Malloc_sram_get_usage()*100));
        Lcd_draw_str(216,3,str_temp,24,0x0000,0,time_bg);
        Ths_get();
        sprintf(str_temp,"%5.1f",Ths_temperature_get);
        Lcd_draw_str(276,3,str_temp,24,0x0000,0,time_bg);
        sprintf(str_temp,"%5.1f",Ths_humidity_get);
        Lcd_draw_str(396,3,str_temp,24,0x0000,0,time_bg);
    }
}
void TIM2_Init(void){
    htim2.Instance               = TIM2;
    htim2.Init.Prescaler         = 8400  - 1;
    htim2.Init.Period            = 10000 - 1;
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim2);
    HAL_NVIC_SetPriority(TIM2_IRQn,1,0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    sec_cnt=0;
}
void TIM2_Start(void){
    HAL_TIM_Base_Start_IT(&htim2);
    is_time_drawing=1;
    Time_draw();
}
void TIM2_Stop(void){
    HAL_TIM_Base_Stop_IT(&htim2);
    is_time_drawing=0;
}
void TIM2_IRQHandler(void){
    HAL_TIM_IRQHandler(&htim2);
}
TIM_HandleTypeDef htim3;
volatile uint8_t sleep_delay_flag=0;
void TIM3_Init(void){
    htim3.Instance           = TIM3;
    htim3.Init.Prescaler     = 8400-1;
    htim3.Init.CounterMode   = TIM_COUNTERMODE_UP;
    htim3.Init.Period        = 1000-1;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&htim3);
    HAL_NVIC_SetPriority(TIM3_IRQn,1,0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
}
void TIM3_IRQHandler(void){
    HAL_TIM_IRQHandler(&htim3);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM2){
        second_signal=1;
        sec_cnt+=1;
        if(sec_cnt>=data_update_cycle){
            Item_data_update();sec_cnt=0;
        }
    }
    if(htim->Instance==TIM3){
        HAL_TIM_Base_Stop_IT(&htim3);
        sleep_delay_flag=1;
    }
}
void sleep_delay_ms(uint32_t nms){
    sleep_delay_flag=0;
    uint32_t arr=nms*10;
    if(arr==0){arr=1;}
    if(arr>65535){arr=65535;}
    __HAL_TIM_SET_AUTORELOAD(&htim3,arr-1);
    __HAL_TIM_SET_COUNTER(&htim3,0);
    HAL_SuspendTick();
    HAL_TIM_Base_Start_IT(&htim3);
    while(!sleep_delay_flag){
        __WFI();
    }
    HAL_ResumeTick();
}
