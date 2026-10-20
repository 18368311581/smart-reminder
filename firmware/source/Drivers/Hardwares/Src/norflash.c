#include "norflash.h"
static SPI_HandleTypeDef norflash_handler;
static uint8_t g_norflash_buf[4096];
static inline void norflash_cs_on(void){
    HAL_GPIO_WritePin(Norflash_CS_GPIO_Port,Norflash_CS_GPIO_Pin,GPIO_PIN_SET);
}
static inline void norflash_cs_off(void){
    HAL_GPIO_WritePin(Norflash_CS_GPIO_Port,Norflash_CS_GPIO_Pin,GPIO_PIN_RESET);
}
static uint8_t norflash_sendget_byte(uint8_t txdata){
    uint8_t rxdata;
    HAL_SPI_TransmitReceive(&norflash_handler,&txdata,&rxdata,1,1000);
    return rxdata;
}
void Norflash_init(void){
    GPIO_InitTypeDef gpio_init_struct={0};
    gpio_init_struct.Mode      = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull      = GPIO_PULLUP;
    gpio_init_struct.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Pin       = Norflash_CS_GPIO_Pin;
    HAL_GPIO_Init(Norflash_CS_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Mode      = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull      = GPIO_PULLUP;
    gpio_init_struct.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Alternate = GPIO_AF5_SPI1;
    gpio_init_struct.Pin       = Norflash_SCK_GPIO_Pin;
    HAL_GPIO_Init(Norflash_SCK_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Mode      = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull      = GPIO_PULLUP;
    gpio_init_struct.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Alternate = GPIO_AF5_SPI1;
    gpio_init_struct.Pin       = Norflash_MISO_GPIO_Pin;
    HAL_GPIO_Init(Norflash_MISO_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Mode      = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull      = GPIO_PULLUP;
    gpio_init_struct.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Alternate = GPIO_AF5_SPI1;
    gpio_init_struct.Pin       = Norflash_MOSI_GPIO_Pin;
    HAL_GPIO_Init(Norflash_MOSI_GPIO_Port,&gpio_init_struct);
    norflash_cs_on();
    norflash_handler.Instance               = SPI1;
    norflash_handler.Init.Mode              = SPI_MODE_MASTER;
    norflash_handler.Init.Direction         = SPI_DIRECTION_2LINES;
    norflash_handler.Init.DataSize          = SPI_DATASIZE_8BIT;
    norflash_handler.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    norflash_handler.Init.CLKPhase          = SPI_PHASE_2EDGE;
    norflash_handler.Init.NSS               = SPI_NSS_SOFT;
    norflash_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    norflash_handler.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    norflash_handler.Init.TIMode            = SPI_TIMODE_DISABLE;
    norflash_handler.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    norflash_handler.Init.CRCPolynomial     = 7;
    HAL_SPI_Init(&norflash_handler);
    norflash_handler.Instance->CR1&=0XFFC7;
    norflash_handler.Instance->CR1|=1<<3;
    __HAL_SPI_ENABLE(&norflash_handler);
}
static void norflash_wait_busy(void){
    uint8_t flag=0;
    while(1){
        norflash_cs_off();
        norflash_sendget_byte(0x05);
        flag=norflash_sendget_byte(0Xff);
        norflash_cs_on();
        if((flag&0x01)!=0x01){
            break;
        }
    }
}
static void norflash_write_enable(void){
    norflash_cs_off();
    norflash_sendget_byte(0x06);
    norflash_cs_on();
}
static void norflash_send_address(uint32_t address){
    norflash_sendget_byte((uint8_t)((address)>>16));
    norflash_sendget_byte((uint8_t)((address)>>8));
    norflash_sendget_byte((uint8_t)address);
}
void Norflash_read(uint8_t *pbuf,uint32_t addr,uint16_t datalen){
    norflash_cs_off();
    norflash_sendget_byte(0x03);
    norflash_send_address(addr);
    for(uint16_t i=0;i<datalen;i++){
        pbuf[i]=norflash_sendget_byte(0XFF);
    }
    norflash_cs_on();
}
void Norflash_clear_sector(uint32_t sector_addr){
    sector_addr*=4096;
    norflash_write_enable();
    norflash_wait_busy();
    norflash_cs_off();
    norflash_sendget_byte(0x20);
    norflash_send_address(sector_addr);
    norflash_cs_on();
    norflash_wait_busy();
}
static void norflash_write_without_check(uint8_t *pbuf,uint32_t addr,uint16_t datalen){
    if(datalen==0){return;}
    uint16_t pageremain=256-addr%256;
    if(datalen<=pageremain){pageremain=datalen;}
    while(1){
        norflash_write_enable();
        norflash_cs_off();
        norflash_sendget_byte(0x02);
        norflash_send_address(addr);
        for(uint16_t i=0;i<pageremain;i++){
            norflash_sendget_byte(pbuf[i]);
        }
        norflash_cs_on();
        norflash_wait_busy();
        if(datalen==pageremain){break;}
        pbuf+=pageremain;
        addr+=pageremain;
        datalen-=pageremain;
        if(datalen>256){pageremain=256;}
        else{pageremain=datalen;}
    }
}
void Norflash_write(uint8_t *pbuf,uint32_t addr,uint16_t datalen){
    if(datalen==0){return;}
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *norflash_buf;
    norflash_buf = g_norflash_buf;
    secpos       = addr / 4096;
    secoff       = addr % 4096;
    secremain    = 4096 - secoff;
    if(datalen<=secremain){secremain=datalen;}
    while(1){
        Norflash_read(norflash_buf,secpos*4096,4096);
        for(i=0;i<secremain;i++){
            if(norflash_buf[secoff+i]!=0XFF){
                break;
            }
        }
        if(i<secremain){
            Norflash_clear_sector(secpos);
            for(i=0;i<secremain;i++){
                norflash_buf[secoff+i]=pbuf[i];
            }
            norflash_write_without_check(norflash_buf,secpos*4096,4096);
        }
        else{
            norflash_write_without_check(pbuf,addr,secremain);
        }
        if(datalen==secremain){
            break;
        }
        else{
            secpos++;
            secoff=0;
            pbuf+=secremain;
            addr+=secremain;
            datalen-=secremain;
            if(datalen>4096){
                secremain=4096;
            }
            else{
                secremain=datalen;
            }
        }
    }
}
