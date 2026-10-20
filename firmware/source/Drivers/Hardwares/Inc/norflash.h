#ifndef __NORFLASH_H_
#define __NORFLASH_H_
#include "basic.h"
#define Norflash_identification   "W25Q128"
#define Norflash_base_id          0xEF17
#define Norflash_max_size         16777216
#define Norflash_CS_GPIO_Port     GPIOB
#define Norflash_CS_GPIO_Pin      GPIO_PIN_14
#define Norflash_SCK_GPIO_Port    GPIOB
#define Norflash_SCK_GPIO_Pin     GPIO_PIN_3
#define Norflash_MISO_GPIO_Port   GPIOB
#define Norflash_MISO_GPIO_Pin    GPIO_PIN_4
#define Norflash_MOSI_GPIO_Port   GPIOB
#define Norflash_MOSI_GPIO_Pin    GPIO_PIN_5
void Norflash_init(void);
void Norflash_read(uint8_t *pbuf,uint32_t addr,uint16_t datalen);
void Norflash_write(uint8_t *pbuf,uint32_t addr,uint16_t datalen);
void Norflash_clear_sector(uint32_t sector_addr);
#endif
