#include "chinese_on_lcd.h"
static uint8_t pfont_b[256];
void Lcd_draw_chr_chinese(uint16_t x,uint16_t y,uint16_t chr,uint8_t size,uint16_t color,uint8_t is_float,uint16_t bgcolor){
    uint8_t temp;uint16_t y0=y;
    uint8_t csize=(size/8+((size%8)?1:0))*(size);
    uint8_t qh=chr>>8,ql=chr&0xFF;
    if(qh<0x81||ql<0x40||ql==0xff||qh==0xff){
        for(uint8_t i=0;i<csize;i++){
            pfont_b[i]=0x00;
        }
    }
    else{
        if(ql<0x7f){ql-=0x40;}
        else{ql-=0x41;}
        qh-=0x81;
        unsigned long foffset=((unsigned long)190*qh+ql)*csize;
        switch(size){
            case 12:Norflash_read(pfont_b,foffset+Font_mbr.gbk_12_address,csize);break;
            case 16:Norflash_read(pfont_b,foffset+Font_mbr.gbk_16_address,csize);break;
            case 24:Norflash_read(pfont_b,foffset+Font_mbr.gbk_24_address,csize);break;
        }
    }
    for(uint8_t t=0;t<csize;t++){
        temp=pfont_b[t];
        for(uint8_t t1=0;t1<8;t1++){
            if(temp&0x80){Lcd_set_point(x,y,color);}
            else if(is_float==0){Lcd_set_point(x,y,bgcolor);}
            temp<<=1;y++;
            if((y-y0)==size){y=y0;x++;break;}
        }
    }
}
void Lcd_draw_str_chinese(uint16_t x,uint16_t y,char* str,uint8_t size,uint16_t color,uint8_t is_float,uint16_t bgcolor){
    uint16_t x0=x,temp;uint8_t bHz=0;
    uint8_t *pstr=(uint8_t *)str;
    while(*pstr!=0){
        if(!bHz){
            if(*pstr>0x80){bHz=1;}
            else{
                if(x>=(480-(size/2))){return;}
                if(*pstr==13||*pstr==10){y+=size;x=x0;pstr++;}
                else{Lcd_draw_chr(x,y,*pstr,size,color,is_float,bgcolor);}
                pstr++;x+=size/2;
            }
        }
        else{
            bHz=0;
            if(x>=(480-(size/2))){return;}
            temp=((uint16_t)(*pstr))<<8;
            pstr+=1;temp|=*pstr;
            Lcd_draw_chr_chinese(x,y,temp,size,color,is_float,bgcolor);
            pstr+=1;x+=size;
        }
    }
}
void Lcd_draw_str_invertly(uint16_t x,uint16_t y,char* str,uint8_t size){
    uint32_t avr=0,avg=0,avb=0,avw=0;
    uint16_t cache=0,color_p=0;
    uint16_t dx=strlen(str)*size/2,dy=size;
    uint8_t rl,gl,bl,r,g,b,max,min,w;
    for(uint16_t i=y;i<y+dy;i++){
        for(uint16_t j=x;j<x+dx;j++){
            cache=Lcd_get_point(j,i);
            rl = (cache>>11)&0x1F;
            gl = (cache>>5 )&0x3F;
            bl = (cache>>0 )&0x1F;
            r  = (rl<<3)|(rl>>2);
            g  = (gl<<2)|(gl>>4);
            b  = (bl<<3)|(bl>>2);
            max = r;
            min = r;
            if(max<g){max=g;}
            if(max<b){max=b;}
            if(min>g){min=g;}
            if(min>b){min=b;}
            w = max-min;
            avr += r*w;
            avg += g*w;
            avb += b*w;
            avw += w;
        }
    }
    if(avw==0){color_p=0xFFFF;}
    else{
        color_p |= ((avr/avw)&0x00F8)<<8;
        color_p |= ((avg/avw)&0x00FC)<<3;
        color_p |= ((avb/avw)&0x00F8)>>3;
    }
    Lcd_draw_str_chinese(x,y,str,size,smart_color_reverse(color_p),1,0);
}
