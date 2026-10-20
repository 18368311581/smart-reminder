#include "ertc.h"
const char Week_names_chinese[7][3]={"一","二","三","四","五","六","日"};
const char Week_names[7][10]={"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
const char Week_short_names[7][4]={"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
static const uint16_t Month_Offset[]={0,31,59,90,120,151,181,212,243,273,304,334};
static inline void ertc_iic_scl_on(void){
    HAL_GPIO_WritePin(Ertc_scl_GPIO_Port,Ertc_scl_GPIO_Pin,GPIO_PIN_SET);
}
static inline void ertc_iic_scl_off(void){
    HAL_GPIO_WritePin(Ertc_scl_GPIO_Port,Ertc_scl_GPIO_Pin,GPIO_PIN_RESET);
}
static inline void ertc_iic_sda_on(void){
    HAL_GPIO_WritePin(Ertc_sda_GPIO_Port,Ertc_sda_GPIO_Pin,GPIO_PIN_SET);
}
static inline void ertc_iic_sda_off(void){
    HAL_GPIO_WritePin(Ertc_sda_GPIO_Port,Ertc_sda_GPIO_Pin,GPIO_PIN_RESET);
}
static inline uint8_t ertc_iic_sda_get(void){
    return (uint8_t)HAL_GPIO_ReadPin(Ertc_sda_GPIO_Port,Ertc_sda_GPIO_Pin);
}
static inline void ertc_iic_sda_set(uint8_t x){
    HAL_GPIO_WritePin(Ertc_sda_GPIO_Port,Ertc_sda_GPIO_Pin,(GPIO_PinState)x);
}
static inline void ertc_iic_delay(void){
    delay_us(2);
}
static void ertc_iic_start(void){
    ertc_iic_sda_on();
    ertc_iic_scl_on();
    ertc_iic_delay();
    ertc_iic_sda_off();
    ertc_iic_delay();
    ertc_iic_scl_off(); 
    ertc_iic_delay();
}
static void ertc_iic_stop(void){
    ertc_iic_sda_off();
    ertc_iic_delay();
    ertc_iic_scl_on();
    ertc_iic_delay();
    ertc_iic_sda_on();
    ertc_iic_delay();
}
static void ertc_iic_send_byte(uint8_t data){
    for(uint8_t t=0;t<8;t++){
        ertc_iic_sda_set((data&0x80)>>7); 
        ertc_iic_delay();
        ertc_iic_scl_on();
        ertc_iic_delay();
        ertc_iic_scl_off();
        data<<=1;
    }
    ertc_iic_sda_on();
}
static uint8_t ertc_iic_read_byte(){
    uint8_t receive=0;
    for(uint8_t i=0;i<8;i++){
        receive<<=1;
        ertc_iic_scl_on();
        ertc_iic_delay();
        receive+=ertc_iic_sda_get();
        ertc_iic_scl_off();
        ertc_iic_delay();
    }
    return receive;
}
static uint8_t ertc_iic_wait_ack(void){
    uint8_t waittime=0,rack=0;
    ertc_iic_sda_on();
    ertc_iic_delay();
    ertc_iic_scl_on();
    ertc_iic_delay();
    while(ertc_iic_sda_get()){
        waittime++;
        if(waittime>250){
            ertc_iic_stop();
            rack=1;
            break;
        }
        ertc_iic_delay();
    }
    ertc_iic_scl_off();
    ertc_iic_delay();
    return rack;
}
static void ertc_iic_nack(void){
    ertc_iic_sda_on();
    ertc_iic_delay();
    ertc_iic_scl_on();
    ertc_iic_delay();
    ertc_iic_scl_off();
    ertc_iic_delay();
}
static uint8_t ertc_wr_reg(uint8_t reg,uint8_t data){
    ertc_iic_start();
    ertc_iic_send_byte(0xD0);
    ertc_iic_wait_ack();
    ertc_iic_send_byte(reg);
    ertc_iic_wait_ack();
    ertc_iic_send_byte(data);
    ertc_iic_wait_ack();
    ertc_iic_stop();
    return 1;
}
static uint8_t ertc_rd_reg(uint8_t reg){
    ertc_iic_start();
    ertc_iic_send_byte(0xD0);
    ertc_iic_wait_ack();
    ertc_iic_send_byte(reg);
    ertc_iic_wait_ack();
    ertc_iic_start();
    ertc_iic_send_byte(0xD1);
    ertc_iic_wait_ack();
    uint8_t data=ertc_iic_read_byte();
    ertc_iic_nack();
    ertc_iic_stop();
    return data;
} 
void Ertc_init(void){
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin   = Ertc_scl_GPIO_Pin;
    HAL_GPIO_Init(Ertc_scl_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin   = Ertc_sda_GPIO_Pin;
    HAL_GPIO_Init(Ertc_sda_GPIO_Port,&GPIO_InitStruct);
    ertc_iic_scl_on();
    ertc_iic_sda_on();
    ertc_iic_stop();
    ertc_wr_reg(0x02,(ertc_rd_reg(0x02)&0x3F));
    ertc_wr_reg(0x05,(ertc_rd_reg(0x05)&0x1F));
    ertc_wr_reg(0x0E,0x00);
}
void Ertc_set_year(uint16_t year){
    ertc_wr_reg(0x06,bin_to_bcd(year-2000));
}
void Ertc_set_month(uint8_t month){
    ertc_wr_reg(0x05,bin_to_bcd(month));
}
void Ertc_set_day(uint8_t day){
    ertc_wr_reg(0x04,bin_to_bcd(day));
}
void Ertc_set_week(uint8_t week){
    ertc_wr_reg(0x03,bin_to_bcd(week));
}
void Ertc_set_hour(uint8_t hour){
    ertc_wr_reg(0x02,bin_to_bcd(hour));
}
void Ertc_set_minute(uint8_t minute){
    ertc_wr_reg(0x01,bin_to_bcd(minute));
}
void Ertc_set_second(uint8_t second){
    ertc_wr_reg(0x00,bin_to_bcd(second));
}
uint16_t Ertc_get_year(void){
    return bcd_to_bin(ertc_rd_reg(0x06))+2000;
}
uint8_t Ertc_get_month(void){
    return bcd_to_bin(ertc_rd_reg(0x05));
}
uint8_t Ertc_get_day(void){
    return bcd_to_bin(ertc_rd_reg(0x04));
}
uint8_t Ertc_get_week(void){
    return bcd_to_bin(ertc_rd_reg(0x03));
}
uint8_t Ertc_get_hour(void){
    return bcd_to_bin(ertc_rd_reg(0x02));
}
uint8_t Ertc_get_minute(void){
    return bcd_to_bin(ertc_rd_reg(0x01));
}
uint8_t Ertc_get_second(void){
    return bcd_to_bin(ertc_rd_reg(0x00));
}
void Ertc_get_format_time(char *data){
    sprintf(data,"%02d:%02d:%02d",Ertc_get_hour(),Ertc_get_minute(),Ertc_get_second());
}
void Ertc_get_format_date(char *data){
    sprintf(data,"%4d-%02d-%02d",Ertc_get_year(),Ertc_get_month(),Ertc_get_day());
}
void Ertc_get_format_week_short(char *data){
    sprintf(data,"%s",Week_short_names[Ertc_get_week()-1]);
}
void Ertc_get_format_week_chinese(char *data){
    sprintf(data,"星期%s",Week_names_chinese[Ertc_get_week()-1]);
}
uint8_t Ertc_week_calc_from_date(uint16_t year,uint8_t month,uint8_t day){
    int days_in_this_year=Month_Offset[month-1]+day;
    int days_in_past_years_mod_7=year-1;
    int days_of_leap_year_offset=(year-1)/4-(year-1)/100+(year-1)/400;
    int days_from_the_year_of_grace=0;
    if((year%4==0)&&((year%100!=0)||(year%400==0))&&(month>2)){days_in_this_year+=1;}
    days_from_the_year_of_grace=days_in_this_year+days_of_leap_year_offset+days_in_past_years_mod_7-1;
    return (uint8_t)(days_from_the_year_of_grace%7+1);
}
uint32_t get_time_fat_format(void){
    uint32_t return_bit=((Ertc_get_year()-1980)&0x7F)<<25;
    return_bit|=(Ertc_get_month()&0x0F)<<21;
    return_bit|=(Ertc_get_day()&0x1F)<<16;
    return_bit|=(Ertc_get_hour()&0x1F)<<11;
    return_bit|=(Ertc_get_minute()&0x3F)<<5;
    return_bit|=(Ertc_get_second()/2)&0x1F;
    return return_bit;
}
