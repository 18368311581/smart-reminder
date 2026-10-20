#ifndef __CHINESE_IN_NORFLASH_H_
#define __CHINESE_IN_NORFLASH_H_
#include "basic.h"
#include "lcd.h"
#include "norflash.h"
#include "malloc_sram.h"
#include "ff.h"
__packed struct font_mbr_struct{
    uint8_t is_font_ok;
    uint32_t unigbk_address;
    uint32_t unigbk_size;
    uint32_t gbk_12_address;
    uint32_t gbk_12_size;
    uint32_t gbk_16_address;
    uint32_t gbk_16_size;
    uint32_t gbk_24_address;
    uint32_t gbk_24_size;
};
extern struct font_mbr_struct Font_mbr;
void chinese_fonts_rewrite(void);
uint8_t chinese_fonts_init(void);
#endif
