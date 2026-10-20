#include "input_ui.h"
void Ui_draw_button_normal(struct Ui_button* button){
    for(uint8_t i=0;i<button->border_width;i++){
        Lcd_draw_line(button->x+i,button->y+i,button->width-1-2*i,0,button->color_light);
        Lcd_draw_line(button->x+i+1,button->y+button->height-i-1,button->width-1-2*i,0,button->color_dark);
        Lcd_draw_line(button->x+i,button->y+i+1,0,button->height-1-2*i,button->color_light);
        Lcd_draw_line(button->x+button->width-i-1,button->y+i,0,button->height-1-2*i,button->color_dark);
    }
}
void Ui_draw_button_pressed(struct Ui_button* button){
    for(uint8_t i=0;i<button->border_width;i++){
        Lcd_draw_line(button->x+i,button->y+i,button->width-1-2*i,0,button->color_dark);
        Lcd_draw_line(button->x+i+1,button->y+button->height-i-1,button->width-1-2*i,0,button->color_light);
        Lcd_draw_line(button->x+i,button->y+i+1,0,button->height-1-2*i,button->color_dark);
        Lcd_draw_line(button->x+button->width-i-1,button->y+i,0,button->height-1-2*i,button->color_light);
    }
}
void Ui_draw_button_text(struct Ui_button* button){
    Lcd_draw_rect(button->x+button->border_width,button->y+button->border_width,\
    button->width-button->border_width*2,button->height-button->border_width*2,button->color_ground,1);
    uint16_t offx=button->x+button->width/2,offy=button->y+button->height/2;
    offx-=(button->text_size/2)*strlen(button->text)/2;offy-=button->text_size/2;
    Lcd_draw_str_chinese(offx,offy,button->text,button->text_size,button->color_text,1,button->color_ground);
}
void Ui_draw_button_init_draw(struct Ui_button* button){
    Ui_draw_button_normal(button);
    Ui_draw_button_text(button);
}
void Ui_draw_button_bg(struct Ui_button* button){
    Lcd_draw_rect(button->x+button->border_width,button->y+button->border_width,\
    button->width-button->border_width*2,button->height-button->border_width*2,button->color_ground,1);
}
uint8_t Ui_draw_button_get(struct Ui_button* button,uint16_t x,uint16_t y){
    if(button->x<x&&x<button->x+button->width&&(!button->is_pressed)){
        if(button->y<y&&y<button->y+button->height){
            return 1;
        }
    }
    return 0;
}
uint8_t Ui_draw_button_get_with_draw(struct Ui_button* button,uint16_t x,uint16_t y){
    if(button->x<x&&x<button->x+button->width&&(!button->is_pressed)){
        if(button->y<y&&y<button->y+button->height){
            Ui_draw_button_pressed(button);
            delay_ms(100);
            Ui_draw_button_normal(button);
            return 1;
        }
    }
    return 0;
}
uint8_t Ui_draw_button_get_axis_x(struct Ui_button* button,uint16_t x){
    return button->x<x&&x<button->x+button->width&&(!button->is_pressed);
}
uint8_t Ui_message_box(char* str,uint8_t type){
    uint16_t* lcd_cache=(uint16_t*)Malloc_sram_alloc(2*280*200);
    uint16_t* lcd_caehc=lcd_cache;
    uint8_t ret=0;
    for(uint16_t j=320;j<520;j++){
        for(uint16_t i=100;i<380;i++){
            *lcd_caehc++=Lcd_get_point(i,j);
        }
    }
    Lcd_draw_rect(100,320,280,200,0x8410,0);
    Lcd_draw_rect(101,321,278,198,0x8410,0);
    if(type==1){
        Lcd_draw_rect(102,322,276,32,0xFD20,1);
        Lcd_draw_str_chinese(216,326,"警告",24,0xFFFF,1,0);
    }
    else if(type==2){
        Lcd_draw_rect(102,322,276,32,0x05F7,1);
        Lcd_draw_str_chinese(216,326,"提示",24,0xFFFF,1,0);
    }
    else if(type==0){
        Lcd_draw_rect(102,322,276,32,0x2672,1);
        Lcd_draw_str_chinese(216,326,"询问",24,0xFFFF,1,0);
    }
    Lcd_draw_rect(102,354,276,116,0xB71D,1);
    Lcd_draw_str_chinese(240-strlen(str)*6,400,str,24,0x0000,1,0);
    Lcd_draw_rect(102,470,276,48,0x8DDE,1);
    struct Ui_button btn_ok={.x=180,.y=476,.width=120,.height=36,.border_width=3,
        .color_light=0xFFFF,.color_dark=0x8410,.color_ground=0xDEFB,.color_text=0x0000,
        .text_size=24,.is_pressed=0,.text="确定"};
    struct Ui_button btn_cancel={.x=260,.y=476,.width=80,.height=36,.border_width=3,
        .color_light=0xFFFF,.color_dark=0x8410,.color_ground=0xDEFB,.color_text=0x0000,
        .text_size=24,.is_pressed=0,.text="取消"};
    if(type==0){
        btn_ok.x=140;btn_ok.width=80;
        Ui_draw_button_init_draw(&btn_cancel);
    }
    Ui_draw_button_init_draw(&btn_ok);
    delay_ms(20);Touch_get();delay_ms(20);
    while(1){
        if(Touch_get()){
            if(Ui_draw_button_get_with_draw(&btn_ok,Touch_get_x[0],Touch_get_y[0])){
                ret=1;break;
            }
            if(type==0&&Ui_draw_button_get_with_draw(&btn_cancel,Touch_get_x[0],Touch_get_y[0])){
                ret=0;break;
            }
        }delay_ms(20);
    }
    Lcd_set_point_list(100,320,280,200,lcd_cache);
    Malloc_sram_free(lcd_cache);
    return ret;
}
void Ui_show_message(char* str,uint32_t show_ms){
    uint16_t* lcd_cache=(uint16_t*)Malloc_sram_alloc(2*280*200);
    uint16_t* lcd_caehc=lcd_cache;
    for(uint16_t j=350;j<450;j++){
        for(uint16_t i=150;i<330;i++){
            *lcd_caehc++=Lcd_get_point(i,j);
        }
    }
    Lcd_draw_rect(150,350,180,100,0x8410,0);
    Lcd_draw_rect(151,351,178, 98,0x8410,0);
    Lcd_draw_rect(152,352,176, 96,0xB71D,1);
    Lcd_draw_str_chinese(240-strlen(str)*6,363,str,24,0x0000,1,0);
    delay_ms(show_ms);
    Lcd_set_point_list(150,350,180,100,lcd_cache);
    Malloc_sram_free(lcd_cache);
}
