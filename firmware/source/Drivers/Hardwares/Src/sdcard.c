#include "sdcard.h"
SD_HandleTypeDef SD_card_handle;
HAL_SD_CardInfoTypeDef SD_card_info_handle;
const uint32_t SD_card_timeout_limit=100000000;
static uint8_t data_buf[512]={0};
uint32_t SD_card_get_total_size_MB(void){
    return ((uint64_t)(SD_card_info_handle.LogBlockNbr)*(SD_card_info_handle.LogBlockSize))>>20;
}
void SD_card_get_info(void){
    HAL_SD_GetCardInfo(&SD_card_handle,&SD_card_info_handle);
}
static void SD_card_wait_busy(void){
    while(HAL_SD_GetCardState(&SD_card_handle)!=HAL_SD_CARD_TRANSFER){}
}
void SD_card_init(void){
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin       = SD_card_D0_GPIO_Pin;
    gpio_init_struct.Mode      = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull      = GPIO_PULLUP;
    gpio_init_struct.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(SD_card_D0_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = SD_card_D1_GPIO_Pin;
    HAL_GPIO_Init(SD_card_D1_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = SD_card_D2_GPIO_Pin;
    HAL_GPIO_Init(SD_card_D2_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = SD_card_D3_GPIO_Pin;
    HAL_GPIO_Init(SD_card_D3_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = SD_card_CLK_GPIO_Pin;
    HAL_GPIO_Init(SD_card_CLK_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = SD_card_CMD_GPIO_Pin;
    HAL_GPIO_Init(SD_card_CMD_GPIO_Port,&gpio_init_struct);
    SD_card_handle.Instance                 = SDIO;
    SD_card_handle.Init.ClockEdge           = SDIO_CLOCK_EDGE_RISING;
    SD_card_handle.Init.ClockBypass         = SDIO_CLOCK_BYPASS_DISABLE;
    SD_card_handle.Init.ClockPowerSave      = SDIO_CLOCK_POWER_SAVE_DISABLE;
    SD_card_handle.Init.BusWide             = SDIO_BUS_WIDE_1B;
    SD_card_handle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    SD_card_handle.Init.ClockDiv            = 1;
    HAL_SD_Init(&SD_card_handle);
    SD_card_get_info();
    HAL_SD_ConfigWideBusOperation(&SD_card_handle,SDIO_BUS_WIDE_4B);
}
void SD_card_read_disk(uint8_t* data,uint32_t disk_address,uint32_t disk_count){
    __disable_irq();
    HAL_SD_ReadBlocks(&SD_card_handle,data_buf,disk_address,disk_count,SD_card_timeout_limit);
    SD_card_wait_busy();
    __enable_irq();
}
void SD_card_read_disk_with_buf(uint8_t* data,uint32_t disk_address,uint32_t disk_count){
    __disable_irq();
    for(uint32_t i=0;i<disk_count;i++){
        HAL_SD_ReadBlocks(&SD_card_handle,data_buf,disk_address+i,1,SD_card_timeout_limit);
        SD_card_wait_busy();
        for(uint16_t j=0;j<512;j++){
            *data=data_buf[j];
            data++;
        }
    }
    __enable_irq();
}
void SD_card_write_disk(uint8_t* data,uint32_t disk_address,uint32_t disk_count){
    __disable_irq();
    HAL_SD_WriteBlocks(&SD_card_handle,data,disk_address,disk_count,SD_card_timeout_limit);
    SD_card_wait_busy();
    __enable_irq();
}
void SD_card_write_disk_with_buf(uint8_t* data,uint32_t disk_address,uint32_t disk_count){
    __disable_irq();
    for(uint32_t i=0;i<disk_count;i++){
        for(uint16_t j=0;j<512;j++){
            data_buf[j]=*data;
            data++;
        }
        HAL_SD_WriteBlocks(&SD_card_handle,data_buf,disk_address+i,1,SD_card_timeout_limit);
        SD_card_wait_busy();
    }
    __enable_irq();
}
