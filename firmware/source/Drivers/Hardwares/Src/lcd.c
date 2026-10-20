#include "lcd.h"
static SRAM_HandleTypeDef lcd_handle;
static uint16_t lcd_read_reg(volatile uint16_t reg){
    volatile uint16_t dat;LCD_REG=reg;dat=LCD_RAM;return dat;
}
static void lcd_change_reg(volatile uint16_t reg,uint16_t clrbit,uint16_t setbit){
    volatile uint16_t original;LCD_REG=reg;original=LCD_RAM;
    original&=~(clrbit);original|=(setbit);LCD_RAM=original;
}
static void lcd_register_init_program(void){
    while((LCD_REG&2)!=0);
    LCD_REG=0x05;LCD_RAM=0x8A;
    LCD_REG=0x06;LCD_RAM=0x21;
    LCD_REG=0x07;LCD_RAM=0x8A;
    LCD_REG=0x08;LCD_RAM=0x85;
    LCD_REG=0x09;LCD_RAM=0x8A;
    LCD_REG=0x0A;LCD_RAM=0x64;
    lcd_change_reg(0x00,0x00,0x80);
    while((lcd_read_reg(0x01)&0x80)==0);
    LCD_REG=0xE0;LCD_RAM=0x20;
    LCD_REG=0xE1;LCD_RAM=0x03;
    LCD_REG=0xE2;LCD_RAM=0x78;
    LCD_REG=0xE3;LCD_RAM=0x20;
    lcd_change_reg(0xE4,0x00,0x01);
    while((lcd_read_reg(0xE4)&0x01)==0);
    lcd_change_reg(0x01,0x58,0x11);
    lcd_change_reg(0x02,0xC0,0x00);
    lcd_change_reg(0x03,0x07,0x00);
    LCD_REG=0x20;LCD_RAM=0x00;
    LCD_REG=0x21;LCD_RAM=0x00;
    LCD_REG=0x22;LCD_RAM=0x00;
    LCD_REG=0x23;LCD_RAM=0x00;
    LCD_REG=0x24;LCD_RAM=0x20;
    LCD_REG=0x25;LCD_RAM=0x03;
    LCD_REG=0x26;LCD_RAM=0x00;
    LCD_REG=0x27;LCD_RAM=0x00;
    LCD_REG=0x28;LCD_RAM=0x00;
    LCD_REG=0x29;LCD_RAM=0x00;
    lcd_change_reg(0x10,0x0C,0x04);
    lcd_change_reg(0x10,0x01,0x00);
    lcd_change_reg(0x12,0x80,0x80);
    lcd_change_reg(0x13,0xF0,0x00);
    LCD_REG=0x14;LCD_RAM=0x63;
    LCD_REG=0x15;LCD_RAM=0x00;
    LCD_REG=0x16;LCD_RAM=0x04;
    LCD_REG=0x17;LCD_RAM=0x06;
    LCD_REG=0x18;LCD_RAM=0x19;
    LCD_REG=0x19;LCD_RAM=0x00;
    LCD_REG=0x1A;LCD_RAM=0xDF;
    LCD_REG=0x1B;LCD_RAM=0x01;
    LCD_REG=0x1C;LCD_RAM=0x16;
    LCD_REG=0x1D;LCD_RAM=0x00;
    LCD_REG=0x1E;LCD_RAM=0x15;
    LCD_REG=0x1F;LCD_RAM=0x00;
    LCD_REG=0x50;LCD_RAM=0x00;
    LCD_REG=0x51;LCD_RAM=0x00;
    LCD_REG=0x52;LCD_RAM=0x00;
    LCD_REG=0x53;LCD_RAM=0x00;
    LCD_REG=0x54;LCD_RAM=0x20;
    LCD_REG=0x55;LCD_RAM=0x03;
    lcd_change_reg(0x5E,0x07,0x01);
    LCD_REG=0x84;LCD_RAM=0x83;
    lcd_change_reg(0x85,0xCC,0x00);
    lcd_change_reg(0x86,0,0x20);
    LCD_REG=0x8C;LCD_RAM=0x00;
    LCD_REG=0x8D;LCD_RAM=0x00;
    LCD_REG=0x8E;LCD_RAM=0x64;
    LCD_REG=0x8F;LCD_RAM=0x00;
    lcd_change_reg(0x86,0x00,0x10);
}
void Lcd_init(void){
    GPIO_InitTypeDef GPIO_InitStruct={0};
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
    GPIO_InitStruct.Pin       = Lcd_CS_GPIO_Pin;
    HAL_GPIO_Init(Lcd_CS_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_WR_GPIO_Pin;
    HAL_GPIO_Init(Lcd_WR_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_RD_GPIO_Pin;
    HAL_GPIO_Init(Lcd_RD_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_RS_GPIO_Pin;
    HAL_GPIO_Init(Lcd_RS_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D0_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D0_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D1_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D1_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D2_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D2_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D3_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D3_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D4_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D4_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D5_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D5_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D6_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D6_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D7_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D7_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D8_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D8_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D9_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D9_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D10_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D10_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D11_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D11_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D12_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D12_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D13_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D13_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D14_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D14_GPIO_Port,&GPIO_InitStruct);
    GPIO_InitStruct.Pin       = Lcd_D15_GPIO_Pin;
    HAL_GPIO_Init(Lcd_D15_GPIO_Port,&GPIO_InitStruct);
    lcd_handle.Instance                = FSMC_NORSRAM_DEVICE;
    lcd_handle.Extended                = FSMC_NORSRAM_EXTENDED_DEVICE;
    lcd_handle.Init.NSBank             = FSMC_NORSRAM_BANK4;
    lcd_handle.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
    lcd_handle.Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;
    lcd_handle.Init.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    lcd_handle.Init.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE;
    lcd_handle.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    lcd_handle.Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;
    lcd_handle.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
    lcd_handle.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
    lcd_handle.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
    lcd_handle.Init.ExtendedMode       = FSMC_EXTENDED_MODE_ENABLE;
    lcd_handle.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    lcd_handle.Init.WriteBurst         = FSMC_WRITE_BURST_DISABLE;
    lcd_handle.Init.PageSize           = FSMC_PAGE_SIZE_NONE;
    FSMC_NORSRAM_TimingTypeDef read_timing={0};
    read_timing.AddressSetupTime      = 16;
    read_timing.DataSetupTime         = 60;
    read_timing.BusTurnAroundDuration = 0;
    read_timing.AccessMode            = FSMC_ACCESS_MODE_A;
    FSMC_NORSRAM_TimingTypeDef write_timing={0};
    write_timing.AddressSetupTime      = 4;
    write_timing.DataSetupTime         = 4;
    write_timing.BusTurnAroundDuration = 0;
    write_timing.AccessMode            = FSMC_ACCESS_MODE_A;
    HAL_SRAM_Init(&lcd_handle,&read_timing,&write_timing);
    lcd_register_init_program();
    Lcd_set_draw_dir(Lcd_draw_director_L2R_U2D);
    Lcd_set_window(0,0,480,800);
    Lcd_clear(0xFFFF);
    Lcd_set_back_light(100);
    Lcd_turn_on();
}
void Lcd_set_back_light(uint8_t light_percent){
    if(light_percent>100||light_percent==0){return;}
    LCD_REG=0x8C;LCD_RAM=light_percent;
    LCD_REG=0x8D;LCD_RAM=0;
}
inline void Lcd_write_start(void){
    LCD_REG=0x04;
}
inline void Lcd_write(uint16_t x){
    LCD_RAM=x;
}
void Lcd_set_cursor(uint16_t x,uint16_t y){
    x=479-x;
    LCD_REG=0x5F;LCD_RAM=y;
    LCD_REG=0x60;LCD_RAM=y>>8;
    LCD_REG=0x61;LCD_RAM=x;
    LCD_REG=0x62;LCD_RAM=x>>8;
}
void Lcd_turn_on(void){
    lcd_change_reg(0x12,0,64);
    lcd_change_reg(0x85,0,8);
}
void Lcd_turn_off(void){
    lcd_change_reg(0x12,64,0);
    lcd_change_reg(0x85,12,0);
}
void Lcd_clear(uint16_t color){
    LCD_REG=0x5F;LCD_RAM=0;
    LCD_REG=0x60;LCD_RAM=0;
    LCD_REG=0x61;LCD_RAM=479;
    LCD_REG=0x62;LCD_RAM=1;
    uint32_t s=48000;while(s--){
        while((LCD_REG&0x80)!=0){__NOP();}
        Lcd_write_start();
        Lcd_write(color);
        Lcd_write(color);
        Lcd_write(color);
        Lcd_write(color);
        Lcd_write(color);
        Lcd_write(color);
        Lcd_write(color);
        Lcd_write(color);
    }
}
void Lcd_set_draw_dir(uint8_t dir){
    lcd_change_reg(0x02,54,dir*18);
}
void Lcd_set_window(uint16_t x,uint16_t y,uint16_t dx,uint16_t dy){
    x=480-x-dx;
    LCD_REG=0x56;LCD_RAM=y;
    LCD_REG=0x57;LCD_RAM=y>>8;
    LCD_REG=0x58;LCD_RAM=x;
    LCD_REG=0x59;LCD_RAM=x>>8;
    LCD_REG=0x5A;LCD_RAM=dy;
    LCD_REG=0x5B;LCD_RAM=dy>>8;
    LCD_REG=0x5C;LCD_RAM=dx;
    LCD_REG=0x5D;LCD_RAM=dx>>8;
}
uint16_t Lcd_get_point(uint16_t x,uint16_t y){
    volatile uint16_t color;
    if((x>=480)||(y>=800)){return 0;}
    Lcd_set_cursor(x,y);Lcd_write_start();
    color=LCD_RAM;delay_us(2);color=LCD_RAM;
    return color;
}
void Lcd_set_point(uint16_t x,uint16_t y,volatile uint16_t color){
    Lcd_set_cursor(x,y);Lcd_write_start();LCD_RAM=color;
}
void Lcd_draw_line(uint16_t x,uint16_t y,int16_t dx,int16_t dy,uint16_t color){
    if(dx==0||dx==1){
        if(dy<0){dy=-dy;y-=dy;}
        for(uint16_t t=0;t<dy;t++){Lcd_set_point(x,y+t,color);}
        return;
    }
    if(dy==0||dy==1){
        if(dx<0){dx*=-1;x-=dx;}
        for(uint16_t t=0;t<dx;t++){Lcd_set_point(x+t,y,color);}
        return;
    }
    uint16_t x1=x+dx,y1=y+dy;int err;
    int sx=(dx>=0)?1:-1;
    int sy=(dy>=0)?1:-1;
    dx=(dx>=0)?dx:-dx;
    dy=(dy>=0)?dy:-dy;
    if(dx>dy){
        err=dx>>1;
        while(x!=x1){
            Lcd_set_point(x,y,color);
            err-=dy;x+=sx;
            if(err<0){y+=sy;err+=dx;}
        }
    }
    else{
        err=dy>>1;
        while(y!=y1){
            Lcd_set_point(x,y,color);
            err-=dx;y+=sy;
            if(err<0){x+=sx;err+=dy;}
        }
    }
}
void Lcd_draw_line_width(uint16_t x,uint16_t y,int16_t dx,int16_t dy,uint16_t color,uint8_t width){
    if(width<=1){Lcd_draw_line(x,y,dx,dy,color);return;}
    int8_t half=width>>1;
    if(dx==0||dx==1){
        for(int8_t i=-half;i<=half;i++){
            Lcd_draw_line(x+i,y,dx,dy,color);
        }return;
    }
    if(dy==0||dy==1){
        for(int8_t i=-half;i<=half;i++){
            Lcd_draw_line(x,y+i,dx,dy,color);
        }return;
    }
    uint16_t x_d=x+dx,y_d=y+dy;
    int8_t sx=(dx>=0)?1:-1,sy=(dy>=0)?1:-1;
    dx=(dx>=0)?dx:-dx;dy=(dy>=0)?dy:-dy;
    if(dx>dy){
        int err=dx/2,e;
        while(1){
            for(int8_t i=-half;i<=half;i++){
                Lcd_set_point(x,y+i,color);
            }
            e=err;
            if(e>-dx){err-=dy;x+=sx;}
            if(e<dy){err+=dx;y+=sy;}
            if(x==x_d&&y==y_d){break;}
        }
    }
    else{
        int err=-dy/2,e;
        while(1){
            for(int8_t i=-half;i<=half;i++){
                Lcd_set_point(x+i,y,color);
            }
            e=err;
            if(e>-dx){err-=dy;x+=sx;}
            if(e<dy){err+=dx;y+=sy;}
            if(x==x_d&&y==y_d){break;}
        }
    }
}
void Lcd_draw_rect(uint16_t x,uint16_t y,uint16_t dx,uint16_t dy,uint16_t color,uint8_t is_fill){
    if(is_fill){
        Lcd_set_window(x,y,dx,dy);
        Lcd_set_cursor(x,y);
        uint32_t s=dx*dy;
        while(s>=8){
            while((LCD_REG&0x80)!=0){__NOP();}
            Lcd_write_start();
            Lcd_write(color);
            Lcd_write(color);
            Lcd_write(color);
            Lcd_write(color);
            Lcd_write(color);
            Lcd_write(color);
            Lcd_write(color);
            Lcd_write(color);
            s-=8;
        }
        while((LCD_REG&0x80)!=0){__NOP();}
        Lcd_write_start();
        while(s--){Lcd_write(color);}
        Lcd_set_window(0,0,480,800);
    }
    else{
        Lcd_draw_line(x,y,dx-1,0,color);
        Lcd_draw_line(x,y,0,dy-1,color);
        Lcd_draw_line(x,y+dy-1,dx,0,color);
        Lcd_draw_line(x+dx-1,y,0,dy,color);
    }
}
void Lcd_draw_circle(uint16_t x,uint16_t y,uint16_t r,uint16_t color,uint8_t is_fill){
    if(!is_fill){
        int32_t a=0,b=r-1,di=3-((r-1)<<1);
        while(a<=b){
            Lcd_set_point(x+a,y-b,color);
            Lcd_set_point(x+b,y-a,color);
            Lcd_set_point(x+b,y+a,color);
            Lcd_set_point(x+a,y+b,color);
            Lcd_set_point(x-a,y+b,color);
            Lcd_set_point(x-b,y+a,color);
            Lcd_set_point(x-a,y-b,color);
            Lcd_set_point(x-b,y-a,color);
            a+=1;
            if(di<0){
                di+=4*a+6;
            }
            else{
                di+=10+4*(a-b);
                b-=1;
            }
        }
    }
    else{
        uint32_t i,xr=r,imax=((uint32_t)r*707)/1000+1,sqmax=(uint32_t)r*(uint32_t)r;
        Lcd_draw_line(x-r,y,2*r,0,color);
        for(i=1;i<=imax;i++){
            if((i*i+xr*xr)>=sqmax){
                if(xr>imax){
                    Lcd_draw_line(x-i+1,y+xr,2*(i-1),0,color);
                    Lcd_draw_line(x-i+1,y-xr,2*(i-1),0,color);
                }
                xr-=1;
            }
            Lcd_draw_line(x-xr,y+i,2*xr,0,color);
            Lcd_draw_line(x-xr,y-i,2*xr,0,color);
        }
    }
}
void Lcd_draw_icon_highbit(uint16_t x,uint16_t y,uint16_t dx,uint16_t dy,uint8_t* list,uint16_t color,uint16_t bgcolor){
    Lcd_set_window(x,y,dx,dy);
    Lcd_set_cursor(x,y);
    Lcd_write_start();
    uint8_t temp;uint32_t kl=dx*dy/8;
    for(uint32_t i=0;i<kl;i++){
        temp=list[i];
        while((LCD_REG&0x80)!=0);
        Lcd_write_start();
        for(uint8_t j=0;j<8;j++){
            if(temp&0x80){Lcd_write(color);}
            else{Lcd_write(bgcolor);}
            temp<<=1;
        }
    }
    Lcd_set_window(0,0,480,800);
}
void Lcd_draw_icon_lowebit(uint16_t x,uint16_t y,uint16_t dx,uint16_t dy,uint8_t* list,uint16_t color,uint16_t bgcolor){
    Lcd_set_window(x,y,dx,dy);
    Lcd_set_cursor(x,y);
    Lcd_write_start();
    uint8_t temp;uint32_t kl=dx*dy/8;
    for(uint32_t i=0;i<kl;i++){
        temp=list[i];
        while((LCD_REG&0x80)!=0);
        Lcd_write_start();
        for(uint8_t j=0;j<8;j++){
            if(temp&0x01){Lcd_write(color);}
            else{Lcd_write(bgcolor);}
            temp>>=1;
        }
    }
    Lcd_set_window(0,0,480,800);
}
void Lcd_draw_chr(uint16_t x,uint16_t y,uint8_t chr,uint8_t size,uint16_t color,uint8_t is_float,uint16_t bgcolor){
    uint8_t temp;uint16_t y0=y;uint8_t *pfont=0;
    uint8_t csize=(size/8+((size%8)?1:0))*(size/2);
    switch(size){
        case 12:pfont=(uint8_t *)asc_1206[chr-' '];break;
        case 16:pfont=(uint8_t *)asc_1608[chr-' '];break;
        case 24:pfont=(uint8_t *)asc_2412[chr-' '];break;
        case 32:pfont=(uint8_t *)asc_3216[chr-' '];break;
        default:return;
    }
    for(uint8_t t=0;t<csize;t++){
        temp=pfont[t];
        for(uint8_t t1=0;t1<8;t1++){
            if(temp&0x80){
                Lcd_set_point(x,y,color);
            }
            else if(is_float==0){
                Lcd_set_point(x,y,bgcolor);
            }
            temp<<=1;
            y++;
            if((y-y0)==size){
                y=y0;
                x++;
                break;
            }
        }
    }
}
void Lcd_draw_str(uint16_t x,uint16_t y,char* str,uint8_t size,uint16_t color,uint8_t is_float,uint16_t bgcolor){
    if(y>(800-size)){return;}
    uint16_t x0=x;
    while((*str<='~')&&(*str>=' ')){
        if(x>=(480-size/2)){return;}
        if(*str==13||*str==10){y+=size;x=x0;str++;}
        else{Lcd_draw_chr(x,y,*str,size,color,is_float,bgcolor);}
        x+=size/2;str++;
    }
}
void Lcd_draw_chr_magnify(uint16_t x,uint16_t y,uint8_t chr,uint8_t size,uint8_t rate,uint16_t color,uint8_t is_float,uint16_t bgcolor){
    uint8_t temp;uint16_t y0=y;uint8_t *pfont=0;
    uint8_t csize=(size/8+((size%8)?1:0))*(size/2);
    switch(size){
        case 12:pfont=(uint8_t *)asc_1206[chr-' '];break;
        case 16:pfont=(uint8_t *)asc_1608[chr-' '];break;
        case 24:pfont=(uint8_t *)asc_2412[chr-' '];break;
        case 32:pfont=(uint8_t *)asc_3216[chr-' '];break;
        default:return;
    }
    for(uint8_t t=0;t<csize;t++){
        temp=pfont[t];
        for(uint8_t t1=0;t1<8;t1++){
            if(temp&0x80){
                for(uint8_t rx=0;rx<rate;rx++){
                    for(uint8_t ry=0;ry<rate;ry++){
                        Lcd_set_point(x+rx,y+ry,color);
                    }
                }
            }
            else if(is_float==0){
                for(uint8_t rx=0;rx<rate;rx++){
                    for(uint8_t ry=0;ry<rate;ry++){
                        Lcd_set_point(x+rx,y+ry,bgcolor);
                    }
                }
            }
            temp<<=1;
            y+=rate;
            if((y-y0)==size*rate){
                y=y0;
                x+=rate;
                break;
            }
        }
    }
}
void Lcd_draw_str_magnify(uint16_t x,uint16_t y,char* str,uint8_t size,uint8_t rate,uint16_t color,uint8_t is_float,uint16_t bgcolor){
    if(y>(800-size*rate)){return;}
    uint16_t x0=x;
    while((*str<='~')&&(*str>=' ')){
        if(x>=(480-size*rate/2)){return;}
        if(*str==13||*str==10){y+=size*rate;x=x0;str++;}
        else{Lcd_draw_chr_magnify(x,y,*str,size,rate,color,is_float,bgcolor);}
        x+=size*rate/2;str++;
    }
}
void Lcd_set_point_list(uint16_t x,uint16_t y,uint16_t dx,uint16_t dy,uint16_t *list){
    Lcd_set_window(x,y,dx,dy);
    Lcd_set_cursor(x,y);
    uint32_t s=dx*dy;
    while(s>=8){
        while((LCD_REG&0x80)!=0){__NOP();}
        Lcd_write_start();
        Lcd_write(*list++);
        Lcd_write(*list++);
        Lcd_write(*list++);
        Lcd_write(*list++);
        Lcd_write(*list++);
        Lcd_write(*list++);
        Lcd_write(*list++);
        Lcd_write(*list++);
        s-=8;
    }
    while((LCD_REG&0x80)!=0){__NOP();}
    Lcd_write_start();
    while(s--){Lcd_write(*list++);}
    Lcd_set_window(0,0,480,800);
}
