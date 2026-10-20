#include "ths.h"
static inline void ths_iic_scl_on(void){
    HAL_GPIO_WritePin(Ths_scl_GPIO_Port,Ths_scl_GPIO_Pin,GPIO_PIN_SET);
}
static inline void ths_iic_scl_off(void){
    HAL_GPIO_WritePin(Ths_scl_GPIO_Port,Ths_scl_GPIO_Pin,GPIO_PIN_RESET);
}
static inline void ths_iic_sda_on(void){
    HAL_GPIO_WritePin(Ths_sda_GPIO_Port,Ths_sda_GPIO_Pin,GPIO_PIN_SET);
}
static inline void ths_iic_sda_off(void){
    HAL_GPIO_WritePin(Ths_sda_GPIO_Port,Ths_sda_GPIO_Pin,GPIO_PIN_RESET);
}
static inline uint8_t ths_iic_sda_get(void){
    return (uint8_t)HAL_GPIO_ReadPin(Ths_sda_GPIO_Port,Ths_sda_GPIO_Pin);
}
static inline void ths_iic_sda_set(uint8_t x){
    HAL_GPIO_WritePin(Ths_sda_GPIO_Port,Ths_sda_GPIO_Pin,(GPIO_PinState)x);
}
static inline void ths_iic_delay(void){
    delay_us(2);
}
static void ths_iic_start(void){
    ths_iic_sda_on();
    ths_iic_scl_on();
    ths_iic_delay();
    ths_iic_sda_off();
    ths_iic_delay();
    ths_iic_scl_off(); 
    ths_iic_delay();
}
static void ths_iic_stop(void){
    ths_iic_scl_off();
    ths_iic_sda_off();
    ths_iic_delay();
    ths_iic_scl_on();
    ths_iic_sda_on();
    ths_iic_delay();
}
static uint8_t ths_iic_wait_ack(void){
    uint8_t waittime=0,rack=0;
    ths_iic_sda_on();
    ths_iic_delay();
    ths_iic_scl_on();
    ths_iic_delay();
    while(ths_iic_sda_get()){
        waittime++;
        if(waittime>250){
            ths_iic_stop();
            rack=1;
            break;
        }
        ths_iic_delay();
    }
    ths_iic_scl_off();
    return rack;
}
static void ths_iic_ack(void){
    ths_iic_scl_off();
    ths_iic_sda_off();
    ths_iic_delay();
    ths_iic_scl_on();
    ths_iic_delay();
    ths_iic_scl_off();
    ths_iic_sda_on();
}
static void ths_iic_nack(void){
    ths_iic_scl_off();
    ths_iic_sda_on();
    ths_iic_delay();
    ths_iic_scl_on();
    ths_iic_delay();
    ths_iic_scl_off();
}
static void ths_iic_send_byte(uint8_t data){
    ths_iic_scl_off();
    for(uint8_t t=0;t<8;t++){
        ths_iic_sda_set((data&0x80)>>7); 
        ths_iic_delay();
        ths_iic_scl_on();
        ths_iic_delay();
        ths_iic_scl_off();
        data<<=1;
    }
    ths_iic_sda_on();
}
static uint8_t ths_iic_read_byte(){
    uint8_t receive=0;
    ths_iic_sda_on();
    for(uint8_t i=0;i<8;i++){
        ths_iic_scl_off();
        ths_iic_delay();
        ths_iic_scl_on();
        ths_iic_delay();
        receive<<=1;
        if(ths_iic_sda_get()){receive|=1;}
    }
    ths_iic_scl_off();
    ths_iic_delay();
    return receive;
}
static void ths_wr_reg(uint8_t reg,uint8_t data){
    ths_iic_start();
    ths_iic_send_byte(0xEC);
    ths_iic_wait_ack();
    ths_iic_send_byte(reg);
    ths_iic_wait_ack();
    ths_iic_send_byte(data);
    ths_iic_wait_ack();
    ths_iic_stop();
}
static uint8_t ths_rd_reg(uint8_t reg){
    ths_iic_start();
    ths_iic_send_byte(0xEC);
    ths_iic_wait_ack();
    ths_iic_send_byte(reg);
    ths_iic_wait_ack();
    ths_iic_start();
    ths_iic_send_byte(0xED);
    ths_iic_wait_ack();
    uint8_t data=ths_iic_read_byte();
    ths_iic_nack();
    ths_iic_stop();
    return data;
}
static void ths_rd_reg_list(uint8_t reg,uint8_t* data,uint8_t len){
    if(len==0){return;}len-=1;
    ths_iic_start();
    ths_iic_send_byte(0xEC);
    ths_iic_wait_ack();
    ths_iic_send_byte(reg);
    ths_iic_wait_ack();
    ths_iic_start();
    ths_iic_send_byte(0xED);
    ths_iic_wait_ack();
    for(uint8_t i=0;i<len;i++){
        data[i]=ths_iic_read_byte();
        ths_iic_ack();
    }
    data[len]=ths_iic_read_byte();
    ths_iic_nack();
    ths_iic_stop();
}
static struct{
    uint16_t T1;int16_t  T2;int16_t  T3;
    uint16_t P1;int16_t  P2;int16_t  P3;
    int16_t  P4;int16_t  P5;int16_t  P6;
    int16_t  P7;int16_t  P8;int16_t  P9;
    uint8_t  H1;int16_t  H2;uint8_t  H3;
    int16_t  H4;int16_t  H5;uint8_t  H6;
    int32_t  tc;}consts;
float Ths_temperature_get,Ths_humidity_get,Ths_pressure_get,Ths_altitude_get;
void Ths_init(void){
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin   = Ths_scl_GPIO_Pin;
    HAL_GPIO_Init(Ths_scl_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin   = Ths_sda_GPIO_Pin;
    HAL_GPIO_Init(Ths_sda_GPIO_Port,&GPIO_InitStruct);
    ths_iic_scl_on();
    ths_iic_sda_on();
    delay_ms(10);
    ths_wr_reg(0xF2,0x05);
    ths_wr_reg(0xF4,0xB3);
    ths_wr_reg(0xF5,0x14);
    uint8_t data_buf[24];
    ths_rd_reg_list(0x88,data_buf,24);
    consts.T1 = (data_buf[1 ]<<8)|data_buf[0 ];
    consts.T2 = (data_buf[3 ]<<8)|data_buf[2 ];
    consts.T3 = (data_buf[5 ]<<8)|data_buf[4 ];
    consts.P1 = (data_buf[7 ]<<8)|data_buf[6 ];
    consts.P2 = (data_buf[9 ]<<8)|data_buf[8 ];
    consts.P3 = (data_buf[11]<<8)|data_buf[10];
    consts.P4 = (data_buf[13]<<8)|data_buf[12];
    consts.P5 = (data_buf[15]<<8)|data_buf[14];
    consts.P6 = (data_buf[17]<<8)|data_buf[16];
    consts.P7 = (data_buf[19]<<8)|data_buf[18];
    consts.P8 = (data_buf[21]<<8)|data_buf[20];
    consts.P9 = (data_buf[23]<<8)|data_buf[22];
    consts.H1 = ths_rd_reg(0xA1);
    ths_rd_reg_list(0xE1,data_buf,7);
    consts.H2 = (data_buf[1 ]<<8)|data_buf[0 ];
    consts.H3 =  data_buf[2 ];
    consts.H4 = (data_buf[3 ]<<4)|(0x0F&data_buf[4]);
    consts.H5 = (data_buf[5 ]<<4)|((data_buf[4]>>4)&0x0F);
    consts.H6 =  data_buf[6 ];
    Ths_temperature_get=0;Ths_humidity_get=0;
    Ths_pressure_get=0;Ths_altitude_get=0;
}
void Ths_get(void){
    int64_t px,py,pz;
    float t_calc,p_calc,h_calc,a_calc;
    int32_t adcT,adcP,adcH,kh;
    uint8_t data_buf[8];
    ths_rd_reg_list(0xF7,data_buf,8);
    adcP=(int32_t)((((uint32_t)(data_buf[0]))<<12)|(((uint32_t)(data_buf[1]))<<4)|(((uint32_t)(data_buf[2]))>>4));
    adcT=(int32_t)((((uint32_t)(data_buf[3]))<<12)|(((uint32_t)(data_buf[4]))<<4)|(((uint32_t)(data_buf[5]))>>4));
    adcH=(int32_t)((((uint32_t)(data_buf[6]))<< 8)|((uint32_t)(data_buf[7])));
    consts.tc=((((adcT>>3)-((int32_t)consts.T1<<1)))*((int32_t)consts.T2))>>11;
    consts.tc+=(((((adcT>>4)-((int32_t)consts.T1))*((adcT>>4)-((int32_t)consts.T1)))>>12)*((int32_t)consts.T3))>>14;
    t_calc=((float)((int32_t)(consts.tc*5+128)>>8))/100.0f;
    px=((int64_t)consts.tc)-128000;
    py=px*px*((int64_t)consts.P6)+((px*(int64_t)consts.P5)<<17)+(((int64_t)consts.P4)<<35);
    px=((px*px*(int64_t)consts.P3)>>8)+((px*(int64_t)consts.P2)<<12);
    px=(((((int64_t)1)<<47)+px))*((int64_t)consts.P1)>>33;
    if(px==0){p_calc=0;}
    else{
        pz=(((((int64_t)(1048576-adcP))<<31)-py)*3125)/px;
        px=(((int64_t)consts.P9)*(pz>>13)*(pz>>13))>>25;
        py=(((int64_t)consts.P8)*pz)>>19;
        pz=((pz+px+py)>>8)+(((int64_t)consts.P7)<<4);
        p_calc=((float)((uint32_t)pz))/256.0f;
    }
    kh=(consts.tc-((int32_t)76800));
    kh=(((((adcH<<14)-(((int32_t)consts.H4)<<20)-(((int32_t)consts.H5)*kh))+((int32_t)16384))>>15)*(((((((kh*((int32_t)consts.H6))>>10)
        *(((kh*((int32_t)consts.H3))>>11)+((int32_t)32768)))>>10)+((int32_t)2097152))*((int32_t)consts.H2)+8192)>>14));
    kh=kh-(((((kh>>15)*(kh>>15))>>7)*((int32_t)consts.H1))>>4);
    if(kh<0){kh=0;}if(kh>419430400){kh=419430400;}
    h_calc=((float)((uint32_t)(kh>>12)))/1024.0f;
    a_calc=p_calc/101325.0f-1.0f;
    a_calc*=0.0770f*a_calc-0.1903f;
    a_calc*=(t_calc+273.15f)/0.0065f;
    Ths_temperature_get = t_calc;
    Ths_pressure_get    = p_calc;
    Ths_humidity_get    = h_calc;
    Ths_altitude_get    = a_calc;
}
