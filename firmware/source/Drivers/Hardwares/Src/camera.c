#include "camera.h"
static const uint8_t camera_uxga_init_reg_table[][2]={
    {0xff,0x00},{0x2c,0xff},{0x2e,0xdf},{0xff,0x01},
    {0x3c,0x32},{0x11,0x00},{0x09,0x02},{0x04,0xA8},
    {0x13,0xe5},{0x14,0x48},{0x2c,0x0c},{0x33,0x78},
    {0x3a,0x33},{0x3b,0xfB},{0x3e,0x00},{0x43,0x11},
    {0x16,0x10},{0x39,0x92},{0x35,0xda},{0x22,0x1a},
    {0x37,0xc3},{0x23,0x00},{0x34,0xc0},{0x36,0x1a},
    {0x06,0x88},{0x07,0xc0},{0x0d,0x87},{0x0e,0x41},
    {0x4c,0x00},{0x48,0x00},{0x5B,0x00},{0x42,0x03},
    {0x4a,0x81},{0x21,0x99},{0x24,0x40},{0x25,0x38},
    {0x26,0x82},{0x5c,0x00},{0x63,0x00},{0x46,0x00},
    {0x0c,0x3c},{0x61,0x70},{0x62,0x80},{0x7c,0x05},
    {0x20,0x80},{0x28,0x30},{0x6c,0x00},{0x6d,0x80},
    {0x6e,0x00},{0x70,0x02},{0x71,0x94},{0x73,0xc1}, 
    {0x3d,0x34},{0x5a,0x57},{0x12,0x00},{0x17,0x11},
    {0x18,0x75},{0x19,0x01},{0x1a,0x97},{0x32,0x36},
    {0x03,0x0f},{0x37,0x40},{0x4f,0xca},{0x50,0xa8},
    {0x5a,0x23},{0x6d,0x00},{0x6d,0x38},{0xff,0x00},
    {0xe5,0x7f},{0xf9,0xc0},{0x41,0x24},{0xe0,0x14},
    {0x76,0xff},{0x33,0xa0},{0x42,0x20},{0x43,0x18},
    {0x4c,0x00},{0x87,0xd5},{0x88,0x3f},{0xd7,0x03},
    {0xd9,0x10},{0xd3,0x82},{0xc8,0x08},{0xc9,0x80},
    {0x7c,0x00},{0x7d,0x00},{0x7c,0x03},{0x7d,0x48},
    {0x7d,0x48},{0x7c,0x08},{0x7d,0x20},{0x7d,0x10},
    {0x7d,0x0e},{0x90,0x00},{0x91,0x0e},{0x91,0x1a},
    {0x91,0x31},{0x91,0x5a},{0x91,0x69},{0x91,0x75},
    {0x91,0x7e},{0x91,0x88},{0x91,0x8f},{0x91,0x96},
    {0x91,0xa3},{0x91,0xaf},{0x91,0xc4},{0x91,0xd7},
    {0x91,0xe8},{0x91,0x20},{0x92,0x00},{0x93,0x06},
    {0x93,0xe3},{0x93,0x05},{0x93,0x05},{0x93,0x00},
    {0x93,0x04},{0x93,0x00},{0x93,0x00},{0x93,0x00},
    {0x93,0x00},{0x93,0x00},{0x93,0x00},{0x93,0x00},
    {0x96,0x00},{0x97,0x08},{0x97,0x19},{0x97,0x02},
    {0x97,0x0c},{0x97,0x24},{0x97,0x30},{0x97,0x28},
    {0x97,0x26},{0x97,0x02},{0x97,0x98},{0x97,0x80},
    {0x97,0x00},{0x97,0x00},{0xc3,0xef},{0xa4,0x00},
    {0xa8,0x00},{0xc5,0x11},{0xc6,0x51},{0xbf,0x80},
    {0xc7,0x10},{0xb6,0x66},{0xb8,0xA5},{0xb7,0x64},
    {0xb9,0x7C},{0xb3,0xaf},{0xb4,0x97},{0xb5,0xFF},
    {0xb0,0xC5},{0xb1,0x94},{0xb2,0x0f},{0xc4,0x5c},
    {0xc0,0xc8},{0xc1,0x96},{0x8c,0x00},{0x86,0x3d},
    {0x50,0x00},{0x51,0x90},{0x52,0x2c},{0x53,0x00},
    {0x54,0x00},{0x55,0x88},{0x5a,0x90},{0x5b,0x2C},
    {0x5c,0x05},{0xd3,0x02},{0xc3,0xed},{0x7f,0x00},
    {0xda,0x09},{0xe5,0x1f},{0xe1,0x67},{0xe0,0x00},
    {0xdd,0x7f},{0x05,0x00}
};
static const uint8_t camera_set_yuv_reg_table[][2]={
    {0xFF,0x00},{0xDA,0x10},{0xD7,0x03},{0xDF,0x00},
    {0x33,0x80},{0x3C,0x40},{0xE1,0x77},{0x00,0x00}
};
static const uint8_t camera_set_jpeg_reg_table[][2]={
    {0xFF,0x01},{0xE0,0x14},{0xE1,0x77},{0xE5,0x1F},
    {0xD7,0x03},{0xDA,0x10},{0xE0,0x00}
};
static const uint8_t camera_set_rgb_reg_table[][2]={
    {0xFF,0x00},{0xDA,0x09},{0xD7,0x03},{0xDF,0x02},
    {0x33,0xA0},{0x3C,0x00},{0xE1,0x67}
};
static const uint8_t camera_light_mode_data[4][3]={
    {0x5E,0x41,0x54},
    {0x65,0x41,0x4F},
    {0x52,0x41,0x66},
    {0x42,0x3F,0x71},
};
static const uint8_t camera_auto_exposure_level_data[5][3]={
    {0x20,0x18,0x60},
    {0x34,0x1C,0x00},
    {0x3E,0x38,0x81},
    {0x48,0x40,0x81},
    {0x58,0x50,0x92},
};
static const uint8_t camera_contrast_data[5][2]={
    {0x18,0x34},
    {0x1C,0x2A},
    {0x20,0x20},
    {0x24,0x16},
    {0x28,0x0C},
};
static const uint8_t camera_special_effects_data[7][3]={
    {0x00,0x80,0x80},
    {0x40,0x80,0x80},
    {0x18,0x80,0x80},
    {0x18,0x40,0xC0},
    {0x18,0x40,0x40},
    {0x18,0xA0,0x40},
    {0x18,0x40,0xA6},
};
static inline void camera_sccb_scl_on(void){
    HAL_GPIO_WritePin(Camera_SCL_GPIO_Port,Camera_SCL_GPIO_Pin,GPIO_PIN_SET);
}
static inline void camera_sccb_scl_off(void){
    HAL_GPIO_WritePin(Camera_SCL_GPIO_Port,Camera_SCL_GPIO_Pin,GPIO_PIN_RESET);
}
static inline void camera_sccb_sda_on(void){
    HAL_GPIO_WritePin(Camera_SDA_GPIO_Port,Camera_SDA_GPIO_Pin,GPIO_PIN_SET);
}
static inline void camera_sccb_sda_off(void){
    HAL_GPIO_WritePin(Camera_SDA_GPIO_Port,Camera_SDA_GPIO_Pin,GPIO_PIN_RESET);
}
static inline uint8_t camera_sccb_sda_get(void){
    return (uint8_t)HAL_GPIO_ReadPin(Camera_SDA_GPIO_Port,Camera_SDA_GPIO_Pin);
}
static inline void camera_sccb_sda_set(uint8_t x){
    HAL_GPIO_WritePin(Camera_SDA_GPIO_Port,Camera_SDA_GPIO_Pin,(GPIO_PinState)x);
}
static inline void camera_sccb_delay(void){
    delay_us(50);
}
static void camera_sccb_start(void){
    camera_sccb_sda_on();
    camera_sccb_scl_on();
    camera_sccb_delay();
    camera_sccb_sda_off();
    camera_sccb_delay();
    camera_sccb_scl_off();
}
static void camera_sccb_stop(void){
    camera_sccb_sda_off();
    camera_sccb_delay();
    camera_sccb_scl_on();
    camera_sccb_delay();
    camera_sccb_sda_on();
    camera_sccb_delay();
}
static void camera_sccb_nack(void){
    camera_sccb_delay();
    camera_sccb_sda_on();
    camera_sccb_scl_on();
    camera_sccb_delay();
    camera_sccb_scl_off();
    camera_sccb_delay();
    camera_sccb_sda_off();
    camera_sccb_delay();
}
static void camera_sccb_send_byte(uint8_t data){
    for(int8_t t=0;t<8;t++){
        camera_sccb_sda_set((data&0x80)>>7);
        camera_sccb_delay();
        camera_sccb_scl_on();
        camera_sccb_delay();
        camera_sccb_scl_off();
        data<<=1;
    }
    camera_sccb_sda_on();
    camera_sccb_delay();
    camera_sccb_scl_on();
    camera_sccb_delay();
    camera_sccb_scl_off();
}
static void camera_sccb_get_byte(uint8_t *dat){
    for(int8_t i=7;i>=0;i--){
        camera_sccb_delay();
        camera_sccb_scl_on();
        *dat|=(camera_sccb_sda_get()<<i);
        camera_sccb_delay();
        camera_sccb_scl_off();
    }
}
static uint8_t camera_sccb_read_reg(uint8_t reg){
    uint8_t data=0;
    camera_sccb_start();
    camera_sccb_send_byte(0x60);
    camera_sccb_delay();
    camera_sccb_send_byte(reg);
    camera_sccb_delay();
    camera_sccb_stop();
    camera_sccb_delay();
    camera_sccb_start();
    camera_sccb_send_byte(0x61);
    camera_sccb_delay();
    camera_sccb_get_byte(&data);
    camera_sccb_nack();
    camera_sccb_stop();
    return data;
}
static void camera_sccb_write_reg(uint8_t reg,uint8_t data){
    camera_sccb_start();
    camera_sccb_delay();
    camera_sccb_send_byte(0x60);
    camera_sccb_delay();
    camera_sccb_send_byte(reg);
    camera_sccb_delay();
    camera_sccb_send_byte(data);
    camera_sccb_delay();
    camera_sccb_stop();
}
static void camera_sccb_init(void){
    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin   = Camera_SCL_GPIO_Pin;
    gpio_initure.Mode  = GPIO_MODE_OUTPUT_PP;      
    gpio_initure.Pull  = GPIO_PULLUP;          
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(Camera_SCL_GPIO_Port,&gpio_initure);
    gpio_initure.Pin   = Camera_SDA_GPIO_Pin;
    gpio_initure.Mode  = GPIO_MODE_OUTPUT_OD;
    gpio_initure.Pull  = GPIO_PULLUP;          
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(Camera_SDA_GPIO_Port,&gpio_initure);
    camera_sccb_stop();
}
inline void Camera_pwdn_on(void){
    HAL_GPIO_WritePin(Camera_PWDN_GPIO_Port,Camera_PWDN_GPIO_Pin,GPIO_PIN_SET);
}
inline void Camera_pwdn_off(void){
    HAL_GPIO_WritePin(Camera_PWDN_GPIO_Port,Camera_PWDN_GPIO_Pin,GPIO_PIN_RESET);
}
static inline void camera_rst_on(void){
    HAL_GPIO_WritePin(Camera_RESET_GPIO_Port,Camera_RESET_GPIO_Pin,GPIO_PIN_SET);
}
static inline void camera_rst_off(void){
    HAL_GPIO_WritePin(Camera_RESET_GPIO_Port,Camera_RESET_GPIO_Pin,GPIO_PIN_RESET);
}
DCMI_HandleTypeDef camera_dcmi_handle;
DMA_HandleTypeDef camera_dma_dcmi_handle;
void Camera_init(void){
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin   = Camera_PWDN_GPIO_Pin;
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull  = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(Camera_PWDN_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin   = Camera_RESET_GPIO_Pin;
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull  = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(Camera_RESET_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin   = Camera_FLASH_GPIO_Pin;
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull  = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(Camera_FLASH_GPIO_Port,&gpio_init_struct);
    HAL_GPIO_WritePin(Camera_FLASH_GPIO_Port,Camera_FLASH_GPIO_Pin,GPIO_PIN_RESET);
    Camera_pwdn_off();delay_ms(10);
    camera_rst_off();delay_ms(20);
    camera_rst_on();delay_ms(20);
    camera_sccb_init();delay_ms(5);
    camera_sccb_write_reg(0xFF,0x01);
    camera_sccb_write_reg(0x12,0x80);
    delay_ms(50);
    for(uint16_t i=0;i<sizeof(camera_uxga_init_reg_table)/2;i++){
        camera_sccb_write_reg(camera_uxga_init_reg_table[i][0],camera_uxga_init_reg_table[i][1]);
    }
    gpio_init_struct.Pin       = Camera_VSYNC_GPIO_Pin;
    gpio_init_struct.Mode      = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull      = GPIO_PULLUP;
    gpio_init_struct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init_struct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(Camera_VSYNC_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = Camera_HREF_GPIO_Pin;
    HAL_GPIO_Init(Camera_HREF_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = Camera_PCLK_GPIO_Pin;
    HAL_GPIO_Init(Camera_PCLK_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = Camera_D0_GPIO_Pin;
    HAL_GPIO_Init(Camera_D0_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = Camera_D1_GPIO_Pin;
    HAL_GPIO_Init(Camera_D1_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = Camera_D2_GPIO_Pin;
    HAL_GPIO_Init(Camera_D2_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = Camera_D3_GPIO_Pin;
    HAL_GPIO_Init(Camera_D3_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = Camera_D4_GPIO_Pin;
    HAL_GPIO_Init(Camera_D4_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = Camera_D5_GPIO_Pin;
    HAL_GPIO_Init(Camera_D5_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = Camera_D6_GPIO_Pin;
    HAL_GPIO_Init(Camera_D6_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = Camera_D7_GPIO_Pin;
    HAL_GPIO_Init(Camera_D7_GPIO_Port,&gpio_init_struct);
    camera_dcmi_handle.Instance              = DCMI;
    camera_dcmi_handle.Init.SynchroMode      = DCMI_SYNCHRO_HARDWARE;
    camera_dcmi_handle.Init.PCKPolarity      = DCMI_PCKPOLARITY_RISING;
    camera_dcmi_handle.Init.VSPolarity       = DCMI_VSPOLARITY_LOW;
    camera_dcmi_handle.Init.HSPolarity       = DCMI_HSPOLARITY_LOW;
    camera_dcmi_handle.Init.CaptureRate      = DCMI_CR_ALL_FRAME;
    camera_dcmi_handle.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
    HAL_DCMI_Init(&camera_dcmi_handle);
}
void Camera_set_flash_on(void){
    HAL_GPIO_WritePin(Camera_FLASH_GPIO_Port,Camera_FLASH_GPIO_Pin,GPIO_PIN_SET);
}
void Camera_set_flash_off(void){
    HAL_GPIO_WritePin(Camera_FLASH_GPIO_Port,Camera_FLASH_GPIO_Pin,GPIO_PIN_RESET);
}
void Camera_set_jpeg_mode(void){
    for(uint16_t i=0;i<(sizeof(camera_set_yuv_reg_table)/2);i++){
        camera_sccb_write_reg(camera_set_yuv_reg_table[i][0],camera_set_yuv_reg_table[i][1]);
    }
    for(uint16_t i=0;i<(sizeof(camera_set_jpeg_reg_table)/2);i++){
        camera_sccb_write_reg(camera_set_jpeg_reg_table[i][0],camera_set_jpeg_reg_table[i][1]);
    }
}
void Camera_set_rgb_mode(void){
    for(uint16_t i=0;i<(sizeof(camera_set_rgb_reg_table)/2);i++){
        camera_sccb_write_reg(camera_set_rgb_reg_table[i][0],camera_set_rgb_reg_table[i][1]);
    }
}
void Camera_set_auto_exposure_level(uint8_t level){
    camera_sccb_write_reg(0xFF,0x01);
    camera_sccb_write_reg(0x24,camera_auto_exposure_level_data[level][0]);
    camera_sccb_write_reg(0x25,camera_auto_exposure_level_data[level][1]);
    camera_sccb_write_reg(0x26,camera_auto_exposure_level_data[level][2]);
}
void Camera_set_light_mode(uint8_t mode){
    camera_sccb_write_reg(0xFF,0x00);
    if(mode==0){
        camera_sccb_write_reg(0xC7,0x00);
        return;
    }
    camera_sccb_write_reg(0xC7,0x40);
    camera_sccb_write_reg(0xCC,camera_light_mode_data[mode-1][0]);
    camera_sccb_write_reg(0xCD,camera_light_mode_data[mode-1][1]);
    camera_sccb_write_reg(0xCE,camera_light_mode_data[mode-1][2]);
}
void Camera_set_color_saturation(uint8_t saturation){
    uint8_t temp=((saturation+2)<<4)|0x08;
    camera_sccb_write_reg(0xFF,0x00);
    camera_sccb_write_reg(0x7C,0x00);
    camera_sccb_write_reg(0x7D,0x02);
    camera_sccb_write_reg(0x7C,0x03);
    camera_sccb_write_reg(0x7D,temp);
    camera_sccb_write_reg(0x7D,temp);
}
void Camera_set_brightness(uint8_t brightness){
    camera_sccb_write_reg(0xFF,0x00);
    camera_sccb_write_reg(0x7C,0x00);
    camera_sccb_write_reg(0x7D,0x04);
    camera_sccb_write_reg(0x7C,0x09);
    camera_sccb_write_reg(0x7D,brightness<<4); 
    camera_sccb_write_reg(0x7D,0x00); 
}
void Camera_set_contrast(uint8_t contrast){
    camera_sccb_write_reg(0xFF,0x00);
    camera_sccb_write_reg(0x7C,0x00);
    camera_sccb_write_reg(0x7D,0x04);
    camera_sccb_write_reg(0x7C,0x07);
    camera_sccb_write_reg(0x7D,0x20);
    camera_sccb_write_reg(0x7D,camera_contrast_data[contrast][0]);
    camera_sccb_write_reg(0x7D,camera_contrast_data[contrast][1]);
    camera_sccb_write_reg(0x7D,0x06);
}
void Camera_set_special_effects(uint8_t special_effects){
    camera_sccb_write_reg(0xFF,0x00);
    camera_sccb_write_reg(0x7C,0x00);
    camera_sccb_write_reg(0x7D,camera_special_effects_data[special_effects][0]);
    camera_sccb_write_reg(0x7C,0x05);
    camera_sccb_write_reg(0x7D,camera_special_effects_data[special_effects][1]);
    camera_sccb_write_reg(0x7D,camera_special_effects_data[special_effects][2]); 
}
void Camera_set_window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height){
    uint8_t temp;
    uint16_t endx=sx+width/2;
    uint16_t endy=sy+height/2;
    camera_sccb_write_reg(0xFF,0x01);
    temp=camera_sccb_read_reg(0x03)&0xF0;
    temp|=((endy&0x03)<<2)|(sy&0x03);
    camera_sccb_write_reg(0x03,temp);
    camera_sccb_write_reg(0x19,sy>>2);
    camera_sccb_write_reg(0x1A,endy>>2);
    temp=camera_sccb_read_reg(0x32)&0xC0;
    temp|=((endx&0x07)<<3)|(sx&0x07);
    camera_sccb_write_reg(0x32,temp);
    camera_sccb_write_reg(0x17,sx>>3);
    camera_sccb_write_reg(0x18,endx>>3);
}
void Camera_set_output_size(uint16_t width,uint16_t height){
    uint8_t temp;
    uint16_t outw=width/4;
    uint16_t outh=height/4;
    temp=(outw>>8)&0x03;
    temp|=(outh>>6)&0x04;
    camera_sccb_write_reg(0xFF,0x00);    
    camera_sccb_write_reg(0xE0,0x04);
    camera_sccb_write_reg(0x5A,outw&0xFF);
    camera_sccb_write_reg(0x5B,outh&0xFF);
    camera_sccb_write_reg(0x5C,temp);
    camera_sccb_write_reg(0xE0,0x00);
}
void Camera_set_image_window(uint16_t offx,uint16_t offy,uint16_t width,uint16_t height){
    uint8_t temp;
    uint16_t hsize=width/4;
    uint16_t vsize=height/4;
    temp=(vsize>>1)&0x80;
    temp|=(offy>>4)&0x70;
    temp|=(hsize>>5)&0x08;
    temp|=(offx>>8)&0x07; 
    camera_sccb_write_reg(0xFF,0x00);
    camera_sccb_write_reg(0xE0,0x04);
    camera_sccb_write_reg(0x51,hsize&0xFF);
    camera_sccb_write_reg(0x52,vsize&0xFF);
    camera_sccb_write_reg(0x53,offx&0xFF);
    camera_sccb_write_reg(0x54,offy&0xFF);
    camera_sccb_write_reg(0x55,temp);
    camera_sccb_write_reg(0x57,(hsize>>2)&0x80);
    camera_sccb_write_reg(0xE0,0x00);
}
void Camera_set_image_size(uint16_t width, uint16_t height){ 
    uint8_t temp;
    temp=(width&0x07)<<3;
    temp|=height&0x07;
    temp|=(width>>4)&0x80;
    camera_sccb_write_reg(0xFF,0x00);
    camera_sccb_write_reg(0xE0,0x04);
    camera_sccb_write_reg(0xC0,(width)>>3&0xFF);
    camera_sccb_write_reg(0xC1,(height)>>3&0xFF);
    camera_sccb_write_reg(0x8C,temp);
    camera_sccb_write_reg(0xE0,0x00);
}
