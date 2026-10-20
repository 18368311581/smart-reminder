#include "sram.h"
static SRAM_HandleTypeDef sram_handler;
void Sram_init(void){
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin       = Sram_CS_GPIO_Pin;
    gpio_init_struct.Mode      = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull      = GPIO_PULLUP;
    gpio_init_struct.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(Sram_CS_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = Sram_WR_GPIO_Pin;
    HAL_GPIO_Init(Sram_WR_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = Sram_RD_GPIO_Pin;
    HAL_GPIO_Init(Sram_RD_GPIO_Port,&gpio_init_struct);
    gpio_init_struct.Pin       = Sram_AD_GPIO_Pin_with_PortD;
    HAL_GPIO_Init(GPIOD,&gpio_init_struct);
    gpio_init_struct.Pin       = Sram_AD_GPIO_Pin_with_PortE;
    HAL_GPIO_Init(GPIOE,&gpio_init_struct);
    gpio_init_struct.Pin       = Sram_AD_GPIO_Pin_with_PortF;
    HAL_GPIO_Init(GPIOF,&gpio_init_struct);
    gpio_init_struct.Pin       = Sram_AD_GPIO_Pin_with_PortG;
    HAL_GPIO_Init(GPIOG,&gpio_init_struct);
    sram_handler.Instance                = FSMC_NORSRAM_DEVICE;
    sram_handler.Extended                = FSMC_NORSRAM_EXTENDED_DEVICE;
    sram_handler.Init.NSBank             = FSMC_NORSRAM_BANK3;
    sram_handler.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
    sram_handler.Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;
    sram_handler.Init.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    sram_handler.Init.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE;
    sram_handler.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    sram_handler.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
    sram_handler.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
    sram_handler.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
    sram_handler.Init.ExtendedMode       = FSMC_EXTENDED_MODE_DISABLE;
    sram_handler.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    sram_handler.Init.WriteBurst         = FSMC_WRITE_BURST_DISABLE;
    FSMC_NORSRAM_TimingTypeDef fsmc_readwritetim;
    fsmc_readwritetim.AddressSetupTime      = 0x02;
    fsmc_readwritetim.AddressHoldTime       = 0x00;
    fsmc_readwritetim.DataSetupTime         = 0x08;
    fsmc_readwritetim.BusTurnAroundDuration = 0x00;
    fsmc_readwritetim.AccessMode            = FSMC_ACCESS_MODE_A;
    HAL_SRAM_Init(&sram_handler,&fsmc_readwritetim,&fsmc_readwritetim);
}
void Sram_write(uint8_t *pbuf,uint32_t addr,uint32_t datalen){
    for(;datalen!=0;datalen--){
        *(volatile uint8_t*)(0X68000000+addr)=*pbuf;
        addr++;
        pbuf++;
    }
}
void Sram_read(uint8_t *pbuf,uint32_t addr,uint32_t datalen){
    for(;datalen!=0;datalen--){
        *pbuf++=*(volatile uint8_t*)(0X68000000+addr);
        addr++;
    }
}
