#include "chinese_in_norflash.h"
static const uint16_t bg_color=0x36DB;
static const uint16_t ft_color=0x0000;
static const uint16_t Font_sector_size=791;
static const uint32_t Font_mbr_address=12*1024*1024;
struct font_mbr_struct Font_mbr;
void chinese_fonts_rewrite(void){
    Lcd_clear(bg_color);
    Lcd_draw_str(0,0,"Start chinese fonts rewrite",24,ft_color,1,bg_color);
    Font_mbr.is_font_ok=0xFF;char temp_str[20];
    uint32_t* buf=Malloc_sram_alloc(4096);uint32_t sector_number;
    Lcd_draw_str(0,26,"Start clear norflash",16,ft_color,1,bg_color);
    Lcd_draw_rect(0,44,400,3,Lcd_color_gray,1);
    Lcd_draw_rect(0,47,3,19,Lcd_color_gray,1);
    Lcd_draw_rect(397,47,3,19,Lcd_color_gray,1);
    Lcd_draw_rect(3,63,394,3,Lcd_color_gray,1);
    for(uint16_t i=0;i<Font_sector_size;i++){
        sector_number=Font_mbr_address/4096+i;
        Norflash_read((uint8_t*)buf,sector_number*4096,4096);
        for(uint16_t j=0;j<1024;j++){
            if(buf[j]!=0xFFFFFFFF){
                Norflash_clear_sector(sector_number);break;
            }
        }
        Lcd_draw_rect(i*390/Font_sector_size+5,49,1,12,ft_color,1);
        sprintf(temp_str,"%3d%%",i*100/Font_sector_size);
        Lcd_draw_str(410,47,temp_str,16,ft_color,0,bg_color);
    }
    Malloc_sram_free(buf);
    Lcd_draw_str(200,26,"fineshed",16,Lcd_color_red,1,bg_color);
    Lcd_draw_str(0,68,"Open file and load Font_mbr",16,ft_color,1,bg_color);
    FIL* file_unigbk=(FIL*)Malloc_sram_alloc(sizeof(FIL));
    f_open(file_unigbk,"0:/smart_remd_stat/UNIGBK.BIN",FA_READ);
    FIL* file_gbk12=(FIL*)Malloc_sram_alloc(sizeof(FIL));
    f_open(file_gbk12,"0:/smart_remd_stat/GBK12.FON",FA_READ);
    FIL* file_gbk16=(FIL*)Malloc_sram_alloc(sizeof(FIL));
    f_open(file_gbk16,"0:/smart_remd_stat/GBK16.FON",FA_READ);
    FIL* file_gbk24=(FIL*)Malloc_sram_alloc(sizeof(FIL));
    f_open(file_gbk24,"0:/smart_remd_stat/GBK24.FON",FA_READ);
    Font_mbr.unigbk_address = Font_mbr_address+sizeof(Font_mbr);
    Font_mbr.unigbk_size    = file_unigbk->obj.objsize;
    Font_mbr.gbk_12_address = Font_mbr.unigbk_address+Font_mbr.unigbk_size;
    Font_mbr.gbk_12_size    = file_gbk12->obj.objsize;
    Font_mbr.gbk_16_address = Font_mbr.gbk_12_address+Font_mbr.gbk_12_size;
    Font_mbr.gbk_16_size    = file_gbk16->obj.objsize;
    Font_mbr.gbk_24_address = Font_mbr.gbk_16_address+Font_mbr.gbk_16_size;
    Font_mbr.gbk_24_size    = file_gbk24->obj.objsize;
    Lcd_draw_str(300,68,"fineshed",16,Lcd_color_red,1,bg_color);
    Lcd_draw_str(0,86,"Start write file to norflash",16,ft_color,1,bg_color);
    uint8_t *tempbuf=Malloc_sram_alloc(4096);uint16_t bread;
    Lcd_draw_str(0,104,"File UNIGBK.BIN",16,ft_color,1,bg_color);
    for(uint32_t offx=Font_mbr.unigbk_address;offx;offx+=4096){
        f_read(file_unigbk,tempbuf,4096,(UINT*)&bread);
        Norflash_write(tempbuf,offx,bread);
        sprintf(temp_str,"%3d%%",(offx-Font_mbr.unigbk_address)*100/Font_mbr.unigbk_size);
        Lcd_draw_str(150,104,temp_str,16,ft_color,0,bg_color);
        if(bread!=4096){break;}
    }
    f_close(file_unigbk);Malloc_sram_free(file_unigbk);
    Lcd_draw_str(200,104,"fineshed",16,Lcd_color_red,1,bg_color);
    Lcd_draw_str(0,122,"File GBK12.BIN",16,ft_color,1,bg_color);
    for(uint32_t offx=Font_mbr.gbk_12_address;offx;offx+=4096){
        f_read(file_gbk12,tempbuf,4096,(UINT*)&bread);
        Norflash_write(tempbuf,offx,bread);
        sprintf(temp_str,"%3d%%",(offx-Font_mbr.gbk_12_address)*100/Font_mbr.gbk_12_size);
        Lcd_draw_str(150,122,temp_str,16,ft_color,0,bg_color);
        if(bread!=4096){break;}
    }
    f_close(file_gbk12);Malloc_sram_free(file_gbk12);
    Lcd_draw_str(200,122,"fineshed",16,Lcd_color_red,1,bg_color);
    Lcd_draw_str(0,140,"File GBK16.BIN",16,ft_color,1,bg_color);
    for(uint32_t offx=Font_mbr.gbk_16_address;offx;offx+=4096){
        f_read(file_gbk16,tempbuf,4096,(UINT*)&bread);
        Norflash_write(tempbuf,offx,bread);
        sprintf(temp_str,"%3d%%",(offx-Font_mbr.gbk_16_address)*100/Font_mbr.gbk_16_size);
        Lcd_draw_str(150,140,temp_str,16,ft_color,0,bg_color);
        if(bread!=4096){break;}
    }
    f_close(file_gbk16);Malloc_sram_free(file_gbk16);
    Lcd_draw_str(200,140,"fineshed",16,Lcd_color_red,1,bg_color);
    Lcd_draw_str(0,158,"File GBK24.BIN",16,ft_color,1,bg_color);
    for(uint32_t offx=Font_mbr.gbk_24_address;offx;offx+=4096){
        f_read(file_gbk24,tempbuf,4096,(UINT*)&bread);
        Norflash_write(tempbuf,offx,bread);
        sprintf(temp_str,"%3d%%",(offx-Font_mbr.gbk_24_address)*100/Font_mbr.gbk_24_size);
        Lcd_draw_str(150,158,temp_str,16,ft_color,0,bg_color);
        if(bread!=4096){break;}
    }
    f_close(file_gbk24);
    Malloc_sram_free(file_gbk24);
    Lcd_draw_str(200,158,"fineshed",16,Lcd_color_red,1,bg_color);
    Malloc_sram_free(tempbuf);
    Font_mbr.is_font_ok=0xAA;
    Norflash_write((uint8_t*)&Font_mbr,Font_mbr_address,sizeof(Font_mbr));
    Lcd_draw_str(200,186,"Update Font_mbr",16,Lcd_color_red,1,bg_color);
    Lcd_draw_str(200,204,"Update All fineshed",16,Lcd_color_red,1,bg_color);
    Lcd_draw_str(250,86,"fineshed",16,Lcd_color_red,1,bg_color);
}
uint8_t chinese_fonts_init(void){
    uint8_t t=0;while(t<10){
        t++;Norflash_read((uint8_t *)&Font_mbr,Font_mbr_address,sizeof(Font_mbr));
        if(Font_mbr.is_font_ok==0xAA){break;}
        delay_ms(20);
    }
    if(Font_mbr.is_font_ok!=0xAA){return 1;}
    return 0;
}
