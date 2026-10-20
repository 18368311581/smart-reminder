#ifndef __SDCARD_H_
#define __SDCARD_H_
#include "basic.h"
#define SD_card_D0_GPIO_Port                GPIOC
#define SD_card_D0_GPIO_Pin                 GPIO_PIN_8
#define SD_card_D1_GPIO_Port                GPIOC
#define SD_card_D1_GPIO_Pin                 GPIO_PIN_9
#define SD_card_D2_GPIO_Port                GPIOC
#define SD_card_D2_GPIO_Pin                 GPIO_PIN_10
#define SD_card_D3_GPIO_Port                GPIOC
#define SD_card_D3_GPIO_Pin                 GPIO_PIN_11
#define SD_card_CLK_GPIO_Port               GPIOC
#define SD_card_CLK_GPIO_Pin                GPIO_PIN_12
#define SD_card_CMD_GPIO_Port               GPIOD
#define SD_card_CMD_GPIO_Pin                GPIO_PIN_2
extern SD_HandleTypeDef SD_card_handle;
extern HAL_SD_CardInfoTypeDef SD_card_info_handle;
uint32_t SD_card_get_total_size_MB(void);
void SD_card_init(void);
void SD_card_get_info(void);
void SD_card_read_disk(uint8_t* data,uint32_t disk_address,uint32_t disk_count);
void SD_card_write_disk(uint8_t* data,uint32_t disk_address,uint32_t disk_count);
void SD_card_read_disk_with_buf(uint8_t* data,uint32_t disk_address,uint32_t disk_count);
void SD_card_write_disk_with_buf(uint8_t* data,uint32_t disk_address,uint32_t disk_count);
#endif
