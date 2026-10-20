#ifndef __SRAM_H_
#define __SRAM_H_
#include "basic.h"
#define Sram_WR_GPIO_Port               GPIOD
#define Sram_WR_GPIO_Pin                GPIO_PIN_5
#define Sram_RD_GPIO_Port               GPIOD
#define Sram_RD_GPIO_Pin                GPIO_PIN_4
#define Sram_CS_GPIO_Port               GPIOG
#define Sram_CS_GPIO_Pin                GPIO_PIN_10
#define Sram_AD_GPIO_Pin_with_PortD     GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
#define Sram_AD_GPIO_Pin_with_PortE     GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
#define Sram_AD_GPIO_Pin_with_PortF     GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
#define Sram_AD_GPIO_Pin_with_PortG     GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
void Sram_init(void);
void Sram_read(uint8_t *pbuf,uint32_t addr,uint32_t datalen);
void Sram_write(uint8_t *pbuf,uint32_t addr,uint32_t datalen);
#endif
