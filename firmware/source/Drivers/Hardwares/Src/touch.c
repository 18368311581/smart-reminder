#include "touch.h"
uint16_t Touch_get_x[5]={0};
uint16_t Touch_get_y[5]={0};
uint16_t Touch_get_size[5]={0};
uint16_t Touch_state=0;
static uint8_t t=0;
static inline void touch_iic_scl_on(void){
    HAL_GPIO_WritePin(Touch_scl_GPIO_Port,Touch_scl_GPIO_Pin,GPIO_PIN_SET);
}
static inline void touch_iic_scl_off(void){
    HAL_GPIO_WritePin(Touch_scl_GPIO_Port,Touch_scl_GPIO_Pin,GPIO_PIN_RESET);
}
static inline void touch_iic_sda_on(void){
    HAL_GPIO_WritePin(Touch_sda_GPIO_Port,Touch_sda_GPIO_Pin,GPIO_PIN_SET);
}
static inline void touch_iic_sda_off(void){
    HAL_GPIO_WritePin(Touch_sda_GPIO_Port,Touch_sda_GPIO_Pin,GPIO_PIN_RESET);
}
static inline uint8_t touch_iic_sda_get(void){
    return (uint8_t)HAL_GPIO_ReadPin(Touch_sda_GPIO_Port,Touch_sda_GPIO_Pin);
}
static inline void touch_iic_sda_set(uint8_t x){
    HAL_GPIO_WritePin(Touch_sda_GPIO_Port,Touch_sda_GPIO_Pin,(GPIO_PinState)x);
}
static inline void touch_iic_delay(void){
    delay_us(2);
}
static void touch_iic_start(void){
    touch_iic_sda_on();
    touch_iic_scl_on();
    touch_iic_delay();
    touch_iic_sda_off();
    touch_iic_delay();
    touch_iic_scl_off(); 
    touch_iic_delay();
}
static void touch_iic_stop(void){
    touch_iic_sda_off();
    touch_iic_delay();
    touch_iic_scl_on();
    touch_iic_delay();
    touch_iic_sda_on();
    touch_iic_delay();
}
static uint8_t touch_iic_wait_ack(void){
    uint8_t waittime=0,rack=0;
    touch_iic_sda_on();
    touch_iic_delay();
    touch_iic_scl_on();
    touch_iic_delay();
    while(touch_iic_sda_get()){
        waittime++;
        if(waittime>250){
            touch_iic_stop();
            rack=1;break;
        }
    }
    touch_iic_scl_off();
    touch_iic_delay();
    return rack;
}
static void touch_iic_ack(void){
    touch_iic_sda_off();
    touch_iic_delay();
    touch_iic_scl_on();
    touch_iic_delay();
    touch_iic_scl_off();
    touch_iic_delay();
    touch_iic_sda_on();
    touch_iic_delay();
}
static void touch_iic_nack(void){
    touch_iic_sda_on();
    touch_iic_delay();
    touch_iic_scl_on();
    touch_iic_delay();
    touch_iic_scl_off();
    touch_iic_delay();
}
static void touch_iic_send_byte(uint8_t data){
    for(uint8_t i=0;i<8;i++){
        touch_iic_sda_set((data&0x80)>>7); 
        touch_iic_delay();
        touch_iic_scl_on();
        touch_iic_delay();
        touch_iic_scl_off();
        data<<=1;
    }
    touch_iic_sda_on();
}
static uint8_t touch_iic_read_byte(){
    uint8_t receive=0;
    for(uint8_t i=0;i<8;i++){
        receive<<=1;
        touch_iic_scl_on();
        touch_iic_delay();
        receive+=touch_iic_sda_get();
        touch_iic_scl_off();
        touch_iic_delay();
    }
    return receive;
}
static uint8_t touch_wr_reg(uint16_t reg,uint8_t *buf,uint8_t len){
    uint8_t ret=0;
    touch_iic_start();
    touch_iic_send_byte(0x28);
    touch_iic_wait_ack();
    touch_iic_send_byte(reg>>8);
    touch_iic_wait_ack();
    touch_iic_send_byte(reg&0xFF);
    touch_iic_wait_ack();
    for(uint8_t i=0;i<len;i++){
        touch_iic_send_byte(buf[i]);
        ret=touch_iic_wait_ack();
        if(ret){break;}
    }
    touch_iic_stop();
    return ret;
}
static void touch_rd_reg(uint16_t reg,uint8_t *buf,uint8_t len){
    touch_iic_start();
    touch_iic_send_byte(0x28);
    touch_iic_wait_ack();
    touch_iic_send_byte(reg>>8);
    touch_iic_wait_ack();
    touch_iic_send_byte(reg&0xFF);
    touch_iic_wait_ack();
    touch_iic_start();
    touch_iic_send_byte(0x29);
    touch_iic_wait_ack();
    for (uint8_t i=0;i<len-1;i++){
        buf[i]=touch_iic_read_byte();
        touch_iic_ack();
    }
    buf[len-1]=touch_iic_read_byte();
    touch_iic_nack();
    touch_iic_stop();
}

static inline void touch_tcs_on(void){
    HAL_GPIO_WritePin(Touch_tcs_GPIO_Port,Touch_tcs_GPIO_Pin,GPIO_PIN_SET);
}
static inline void touch_tcs_off(void){
    HAL_GPIO_WritePin(Touch_tcs_GPIO_Port,Touch_tcs_GPIO_Pin,GPIO_PIN_RESET);
}
static inline void touch_pen_on(void){
    HAL_GPIO_WritePin(Touch_pen_GPIO_Port,Touch_pen_GPIO_Pin,GPIO_PIN_SET);
}
static inline void touch_pen_off(void){
    HAL_GPIO_WritePin(Touch_pen_GPIO_Port,Touch_pen_GPIO_Pin,GPIO_PIN_RESET);
}
void Touch_init(void){
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin   = Touch_tcs_GPIO_Pin;
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull  = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(Touch_tcs_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin   = Touch_pen_GPIO_Pin;
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull  = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(Touch_pen_GPIO_Port, &gpio_init_struct);
    touch_tcs_off();
    touch_pen_off();
    touch_pen_on();
    delay_ms(1);
    touch_tcs_on();
    delay_ms(10);
    gpio_init_struct.Pin   = Touch_pen_GPIO_Pin;
    gpio_init_struct.Mode  = GPIO_MODE_INPUT;
    gpio_init_struct.Pull  = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(Touch_pen_GPIO_Port, &gpio_init_struct);
    gpio_init_struct.Pin   = Touch_scl_GPIO_Pin;
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_OD;
    gpio_init_struct.Pull  = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(Touch_scl_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin   = Touch_sda_GPIO_Pin;
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_OD;
    gpio_init_struct.Pull  = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(Touch_sda_GPIO_Port,&gpio_init_struct);
    touch_iic_stop();
    uint8_t dat=0x02;
    touch_wr_reg(0X8040,&dat,1);
    delay_ms(10);dat=0x00;
    touch_wr_reg(0X8040,&dat,1);
}
uint8_t Touch_get(void){
    t++;
    uint8_t mode=0;
    uint8_t tp_cnt=0;
    uint8_t i;
    uint8_t res=0;
    uint16_t temp;
    uint16_t tempsta;
    uint8_t buf[6];
    if((t%10)==0||t<10){
        touch_rd_reg(0x814E,&mode,1);
        if(mode&0x80){
            tp_cnt=mode&0x0F;
            if(tp_cnt<=5){
                mode=0;
                touch_wr_reg(0x814E,&mode,1);
                if(tp_cnt){
                    temp=0xFFFF<<tp_cnt;
                    tempsta=Touch_state;
                    Touch_state=(~temp)|0x8000|0x4000;
                    Touch_get_x[4]=Touch_get_x[0];
                    Touch_get_y[4]=Touch_get_y[0];
                    for(i=0;i<tp_cnt;i++){
                        touch_rd_reg(0x8150+8*i,buf,6);
                        Touch_get_x[i]=480-((((uint16_t)buf[3])<<8)|buf[2]);
                        Touch_get_y[i]=((((uint16_t)buf[1])<<8)|buf[0]);
                        Touch_get_size[i]=((((uint16_t)buf[5])<<8)|buf[4]);
                    }
                    if(Touch_get_x[0]>480 || Touch_get_y[0]>800||Touch_get_size[0]>400){
                        if(tp_cnt>1){
                            Touch_get_x[0]=Touch_get_x[1];
                            Touch_get_y[0]=Touch_get_y[1];
                            Touch_get_size[0]=Touch_get_size[1];
                            t=0;
                        }
                        else{
                            Touch_get_x[0]=Touch_get_x[4];
                            Touch_get_y[0]=Touch_get_y[4];
                            Touch_get_size[0]=Touch_get_size[4];
                            Touch_state=tempsta;
                        }
                    }
                    else{
                        t=0;
                    }
                    res=1;
                }
            }
        }
    }
    if(tp_cnt==0){
        if(Touch_state&0x8000){
            Touch_state&=~0x8000;
        }
        else{
            Touch_get_x[0]=0xFFFF;
            Touch_get_y[0]=0xFFFF;
            Touch_get_size[0]=0xFFFF;
            Touch_state&=0xE000;
        }
    }
    if(t>240){
        t=10;
    }
    return res;
}
void Touch_clear(void){
    Touch_state=0;t=0;
    for(uint8_t i=0;i<5;i++){
        Touch_get_x[i]=0;
        Touch_get_y[i]=0;
        Touch_get_size[i]=0;
    }
}
uint8_t Touch_try_now(void){
    uint8_t mode=0,i=0;touch_rd_reg(0X814E,&mode,1);
    if((mode&0X80)&&((mode&0XF)<=5)){i=0;touch_wr_reg(0X814E,&i,1);}
    if((mode&0XF)&&((mode&0XF)<=5)){return 1;}return 0;
}
