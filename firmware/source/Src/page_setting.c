#include "page_setting.h"
uint8_t is_setting_show=0;
static const uint32_t setting_address=(15*1024+768)*1024;
static setting_pack now_setting;
static uint8_t save_checkly_flag;
static uint8_t predate_set=0;
static uint8_t prehour_set=0;
static const char auto_exposure_level_table[5][7]={"等级0","等级1" ,"等级2" ,"等级3" ,"等级4" };
static const char light_mode_table[5][7]         ={"自动" ,"日光下","办公室","阴天"  ,"家里"  };
static const char color_saturation_table[5][7]   ={"-2"   ,"-1"    ,"0"     ,"1"     ,"2"     };
static const char brightness_table[5][7]         ={"-2"   ,"-1"    ,"0"     ,"1"     ,"2"     };
static const char contrast_table[5][7]           ={"-2"   ,"-1"    ,"0"     ,"1"     ,"2"     };
static const uint16_t color_button=0xBDFE;
static const uint16_t color_bg=0x96DF;
static struct Ui_button lcd_light_minus_one={
    .x=100,.y=64,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static void lcd_light_update_draw(void){
    char tep[3];
    sprintf(tep,"%2d",now_setting.lcd_light);
    Lcd_draw_str(122,68,tep,24,Lcd_color_black,0,color_bg);
}
static struct Ui_button lcd_light_add_one={
    .x=172,.y=64,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button update_font={
    .x=202,.y=64,.width=110,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="更新字库"};
static struct Ui_button save_data_base={
    .x=322,.y=64,.width=126,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="备份数据库"};
static struct Ui_button year_minus={
    .x=100,.y=104,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static void year_update_draw(void){
    char tep[5];
    sprintf(tep,"%4d",Ertc_get_year());
    Lcd_draw_str(122,108,tep,24,Lcd_color_black,0,color_bg);
}
static struct Ui_button year_add={
    .x=196,.y=104,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button month_minus={
    .x=240,.y=104,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static void month_update_draw(void){
    char tep[3];
    sprintf(tep,"%2d",Ertc_get_month());
    Lcd_draw_str(262,108,tep,24,Lcd_color_black,0,color_bg);
}
static struct Ui_button month_add={
    .x=312,.y=104,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button day_minus={
    .x=356,.y=104,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static void day_update_draw(void){
    char tep[3];
    sprintf(tep,"%2d",Ertc_get_day());
    Lcd_draw_str(378,108,tep,24,Lcd_color_black,0,color_bg);
}
static struct Ui_button day_add={
    .x=428,.y=104,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button hour_minus={
    .x=100,.y=144,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static void hour_update_draw(void){
    char tep[3];
    sprintf(tep,"%2d",Ertc_get_hour());
    Lcd_draw_str(122,148,tep,24,Lcd_color_black,0,color_bg);
}
static struct Ui_button hour_add={
    .x=148,.y=144,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button minute_minus={
    .x=184,.y=144,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static void minute_update_draw(void){
    char tep[3];
    sprintf(tep,"%02d",Ertc_get_minute());
    Lcd_draw_str(206,148,tep,24,Lcd_color_black,0,color_bg);
}
static struct Ui_button minute_add={
    .x=232,.y=144,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button second_minus={
    .x=268,.y=144,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static void second_update_draw(void){
    char tep[3];
    sprintf(tep,"%02d",Ertc_get_second());
    Lcd_draw_str(290,148,tep,24,Lcd_color_black,0,color_bg);
}
static struct Ui_button second_add={
    .x=316,.y=144,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button sys_test={
    .x=344,.y=144,.width=104,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="系统版本"};
static struct Ui_button camera_test={
    .x=240,.y=184,.width=208,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="摄像机测试  开始"};
static struct Ui_button color_saturation_button={
    .x=108,.y=224,.width=125,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text=""};
static struct Ui_button light_mode_button={
    .x=323,.y=224,.width=125,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text=""};
static struct Ui_button contrast_button={
    .x=108,.y=264,.width=125,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text=""};
static struct Ui_button brightness_button={
    .x=323,.y=264,.width=125,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text=""};
static struct Ui_button auto_exposure_button={
    .x=180,.y=304,.width=130,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text=""};
static struct Ui_button save_setting_button={
    .x=323,.y=304,.width=125,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="保存设置"};
static uint8_t is_camera_testing;
void SET_PRO_func(void){
    if(is_setting_show&&is_camera_testing==0){
        char tep[5];sprintf(tep,"%02d",Ertc_get_second());
        Lcd_draw_str(290,148,tep,24,Lcd_color_black,0,color_bg);
        sprintf(tep,"%02d",Ertc_get_minute());
        Lcd_draw_str(206,148,tep,24,Lcd_color_black,0,color_bg);
        uint8_t hour_tp=Ertc_get_hour();
        if(hour_tp!=prehour_set){
            prehour_set=hour_tp;
            sprintf(tep,"%2d",prehour_set);
            Lcd_draw_str(122,148,tep,24,Lcd_color_black,0,color_bg);
            uint8_t day_tp=Ertc_get_day();
            if(day_tp!=predate_set){
                predate_set=day_tp;
                sprintf(tep,"%2d",predate_set);
                Lcd_draw_str(378,108,tep,24,Lcd_color_black,0,color_bg);
                sprintf(tep,"%2d",Ertc_get_month());
                Lcd_draw_str(262,108,tep,24,Lcd_color_black,0,color_bg);
                sprintf(tep,"%4d",Ertc_get_year());
                Lcd_draw_str(122,108,tep,24,Lcd_color_black,0,color_bg);
            }
        }
    }
}
static uint8_t crc_calc(void){
    uint8_t crc=0x00;
    uint8_t data[6]={now_setting.auto_exposure_level,now_setting.brightness,
    now_setting.color_saturation,now_setting.contrast,now_setting.lcd_light,now_setting.light_mode};
    for(uint8_t i=0;i<6;i++){
        crc^=data[i];
        for(uint8_t j=0;j<8;j++){
            if(crc&0x80){
                crc=(crc<<1)^0x31;
            }
            else{
                crc<<=1;
            }
        }
    }
    return crc;
}
static void setting_apply(void){
    Lcd_set_back_light(now_setting.lcd_light);
    Camera_set_auto_exposure_level(now_setting.auto_exposure_level);
    auto_exposure_button.text=(char*)auto_exposure_level_table[now_setting.auto_exposure_level];
    Camera_set_brightness(now_setting.brightness);
    brightness_button.text=(char*)brightness_table[now_setting.brightness];
    Camera_set_color_saturation(now_setting.color_saturation);
    color_saturation_button.text=(char*)color_saturation_table[now_setting.color_saturation];
    Camera_set_light_mode(now_setting.light_mode);
    light_mode_button.text=(char*)light_mode_table[now_setting.light_mode];
    Camera_set_contrast(now_setting.contrast);
    contrast_button.text=(char*)contrast_table[now_setting.contrast];
}
void Setting_save(void){
    now_setting.crc=crc_calc();
    Norflash_write((uint8_t*)&now_setting,setting_address,sizeof(now_setting));
}
void Setting_load(void){
    save_checkly_flag=0;
    Norflash_read((uint8_t*)&now_setting,setting_address,sizeof(now_setting));
    if(crc_calc()!=now_setting.crc||setting_password!=now_setting.password){
        now_setting.password=setting_password;
        now_setting.lcd_light=50;
        now_setting.auto_exposure_level=1;
        now_setting.light_mode=0;
        now_setting.color_saturation=0;
        now_setting.brightness=2;
        now_setting.contrast=2;
        now_setting.crc=crc_calc();
        Setting_save();
    }
    setting_apply();
}
void Lcd_reset(void){
    Lcd_set_back_light(now_setting.lcd_light);
}
void Setting_save_checkly(void){
    if(save_checkly_flag){
        Setting_save();
        save_checkly_flag=0;
    }
}
void Page_setting_draw(void){
    page_lcd_clear(color_bg);
    is_setting_show=1;
    is_camera_testing=0;
    Lcd_draw_str_chinese(156,34,"系  统  设  置",24,Lcd_color_black,1,0);
    Lcd_draw_str_chinese(32,68,"亮度:",24,Lcd_color_black,1,0);
    Ui_draw_button_init_draw(&lcd_light_minus_one);
    lcd_light_update_draw();
    Ui_draw_button_init_draw(&lcd_light_add_one);
    Ui_draw_button_init_draw(&update_font);
    Ui_draw_button_init_draw(&save_data_base);
    Lcd_draw_str_chinese(32,108,"日期:",24,Lcd_color_black,1,0);
    Ui_draw_button_init_draw(&year_minus);
    year_update_draw();
    Lcd_draw_str_chinese(170,108,"年",24,Lcd_color_black,0,color_bg);
    Ui_draw_button_init_draw(&year_add);
    Ui_draw_button_init_draw(&month_minus);
    month_update_draw();
    Lcd_draw_str_chinese(286,108,"月",24,Lcd_color_black,0,color_bg);
    Ui_draw_button_init_draw(&month_add);
    Ui_draw_button_init_draw(&day_minus);
    day_update_draw();
    Lcd_draw_str_chinese(402,108,"日",24,Lcd_color_black,0,color_bg);
    Ui_draw_button_init_draw(&day_add);
    Lcd_draw_str_chinese(32,148,"时间:",24,Lcd_color_black,1,0);
    Ui_draw_button_init_draw(&hour_minus);
    hour_update_draw();
    Ui_draw_button_init_draw(&hour_add);
    Lcd_draw_str(170,148,":",24,Lcd_color_black,0,color_bg);
    Ui_draw_button_init_draw(&minute_minus);
    minute_update_draw();
    Ui_draw_button_init_draw(&minute_add);
    Lcd_draw_str(254,148,":",24,Lcd_color_black,0,color_bg);
    Ui_draw_button_init_draw(&second_minus);
    second_update_draw();
    Ui_draw_button_init_draw(&second_add);
    Ui_draw_button_init_draw(&sys_test);
    Lcd_draw_str_chinese(32,188,"摄像机参数设置:",24,Lcd_color_black,0,color_bg);
    Ui_draw_button_init_draw(&camera_test);
    Lcd_draw_str_chinese( 32,228,"饱和度",24,Lcd_color_black,0,color_bg);
    Ui_draw_button_init_draw(&color_saturation_button);
    Lcd_draw_str_chinese(247,228,"白平衡",24,Lcd_color_black,0,color_bg);
    Ui_draw_button_init_draw(&light_mode_button);
    Lcd_draw_str_chinese( 32,268,"对比度",24,Lcd_color_black,0,color_bg);
    Ui_draw_button_init_draw(&contrast_button);
    Lcd_draw_str_chinese(247,268,"光亮度",24,Lcd_color_black,0,color_bg);
    Ui_draw_button_init_draw(&brightness_button);
    Lcd_draw_str_chinese( 32,308,"自动曝光等级",24,Lcd_color_black,0,color_bg);
    Ui_draw_button_init_draw(&auto_exposure_button);
    Ui_draw_button_init_draw(&save_setting_button);
    Lcd_draw_rect(59,339,362,362,0x0000,0);
    Lcd_draw_rect(58,338,364,364,0x0000,0);
    Lcd_draw_rect(60,340,360,360,0xFFFF,1);
    Camera_icon_draw(192,472,0x0000);
}
void Page_setting_process(uint16_t x,uint16_t y){
    if(is_camera_testing){
        if(Ui_draw_button_get(&camera_test,x,y)&&sys_count_icon==0){
            Camera_draw_lcd_stop();
            Ui_draw_button_pressed(&camera_test);
            camera_test.text="摄像机测试  开始";
            Ui_draw_button_text(&camera_test);
            delay_ms(100);
            Ui_draw_button_normal(&camera_test);
            Icon_can_press=1;
            is_camera_testing=0;
            TIM2_Start();
            Lcd_draw_rect(60,340,360,360,0xFFFF,1);
            Camera_icon_draw(192,472,0x0000);
            sys_count_icon=15;
            return;
        }
        else if(Ui_draw_button_get(&color_saturation_button,x,y)&&sys_count_icon==0){
            Camera_draw_lcd_stop();
            if(now_setting.color_saturation==4){now_setting.color_saturation=0;}
            else if(now_setting.color_saturation<4){now_setting.color_saturation+=1;}
            color_saturation_button.text=(char*)color_saturation_table[now_setting.color_saturation];
            Camera_set_color_saturation(now_setting.color_saturation);
            Ui_draw_button_text(&color_saturation_button);
            Ui_draw_button_pressed(&color_saturation_button);
            delay_ms(100);
            Ui_draw_button_normal(&color_saturation_button);
            Camera_draw_lcd_start(60,340,360,360);
            sys_count_icon=5;
            return;
        }
        else if(Ui_draw_button_get(&light_mode_button,x,y)&&sys_count_icon==0){
            Camera_draw_lcd_stop();
            if(now_setting.light_mode==4){now_setting.light_mode=0;}
            else if(now_setting.light_mode<4){now_setting.light_mode+=1;}
            light_mode_button.text=(char*)light_mode_table[now_setting.light_mode];
            Camera_set_light_mode(now_setting.light_mode);
            Ui_draw_button_text(&light_mode_button);
            Ui_draw_button_pressed(&light_mode_button);
            delay_ms(100);
            Ui_draw_button_normal(&light_mode_button);
            Camera_draw_lcd_start(60,340,360,360);
            sys_count_icon=5;
            return;
        }
        else if(Ui_draw_button_get(&contrast_button,x,y)&&sys_count_icon==0){
            Camera_draw_lcd_stop();
            if(now_setting.contrast==4){now_setting.contrast=0;}
            else if(now_setting.contrast<4){now_setting.contrast+=1;}
            contrast_button.text=(char*)contrast_table[now_setting.contrast];
            Camera_set_contrast(now_setting.contrast);
            Ui_draw_button_text(&contrast_button);
            Ui_draw_button_pressed(&contrast_button);
            delay_ms(100);
            Ui_draw_button_normal(&contrast_button);
            Camera_draw_lcd_start(60,340,360,360);
            sys_count_icon=5;
            return;
        }
        else if(Ui_draw_button_get(&brightness_button,x,y)&&sys_count_icon==0){
            Camera_draw_lcd_stop();
            if(now_setting.brightness==4){now_setting.brightness=0;}
            else if(now_setting.brightness<4){now_setting.brightness+=1;}
            brightness_button.text=(char*)brightness_table[now_setting.brightness];
            Camera_set_brightness(now_setting.brightness);
            Ui_draw_button_text(&brightness_button);
            Ui_draw_button_pressed(&brightness_button);
            delay_ms(100);
            Ui_draw_button_normal(&brightness_button);
            Camera_draw_lcd_start(60,340,360,360);
            sys_count_icon=5;
            return;
        }
        else if(Ui_draw_button_get(&auto_exposure_button,x,y)&&sys_count_icon==0){
            Camera_draw_lcd_stop();
            if(now_setting.auto_exposure_level==4){now_setting.auto_exposure_level=0;}
            else if(now_setting.auto_exposure_level<4){now_setting.auto_exposure_level+=1;}
            auto_exposure_button.text=(char*)auto_exposure_level_table[now_setting.auto_exposure_level];
            Camera_set_auto_exposure_level(now_setting.auto_exposure_level);
            Ui_draw_button_text(&auto_exposure_button);
            Ui_draw_button_pressed(&auto_exposure_button);
            delay_ms(100);
            Ui_draw_button_normal(&auto_exposure_button);
            Camera_draw_lcd_start(60,340,360,360);
            sys_count_icon=5;
            return;
        }
    }
    else{
        if(64<=y&&y<=96){
            if(Ui_draw_button_get_with_draw(&lcd_light_minus_one,x,y)){
                if(now_setting.lcd_light>1){
                    save_checkly_flag=1;
                    now_setting.lcd_light-=1;
                    lcd_light_update_draw();
                    Lcd_set_back_light(now_setting.lcd_light);
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&lcd_light_add_one,x,y)){
                if(now_setting.lcd_light<99){
                    save_checkly_flag=1;
                    now_setting.lcd_light+=1;
                    lcd_light_update_draw();
                    Lcd_set_back_light(now_setting.lcd_light);
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&update_font,x,y)){
                TIM2_Stop();
                chinese_fonts_rewrite();
                delay_ms(100);
                Ui_message_box("更新完毕",2);
                Ui_message_box("建议重启",2);
                Icons_draw();
                Page_setting_draw();
                TIM2_Start();
                return;
            }
            else if(Ui_draw_button_get_with_draw(&save_data_base,x,y)){
                Item_data_backup();
                Ui_message_box("备份完毕",2);
                return;
            }
        }
        else if(104<=y&&y<=136){
            if(Ui_draw_button_get_with_draw(&year_minus,x,y)){
                uint16_t t=Ertc_get_year();
                if(t>2020){
                    t-=1;Ertc_set_year(t);year_update_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&year_add,x,y)){
                uint16_t t=Ertc_get_year();
                if(t<2099){
                    t+=1;Ertc_set_year(t);year_update_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&month_minus,x,y)){
                uint8_t t=Ertc_get_month();
                if(t>1){
                    t-=1;Ertc_set_month(t);month_update_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&month_add,x,y)){
                uint8_t t=Ertc_get_month();
                if(t<12){
                    t+=1;Ertc_set_month(t);month_update_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&day_minus,x,y)){
                uint8_t t=Ertc_get_day();
                if(t>1){
                    t-=1;Ertc_set_day(t);day_update_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&day_add,x,y)){
                uint8_t t=Ertc_get_day();
                if(t<get_month_days(Ertc_get_year(),Ertc_get_month())){
                    t+=1;Ertc_set_day(t);day_update_draw();
                }
                return;
            }
        }
        else if(144<=y&&y<=180){
            if(Ui_draw_button_get_with_draw(&hour_minus,x,y)){
                uint16_t t=Ertc_get_hour();
                if(t>0){
                    t-=1;Ertc_set_hour(t);hour_update_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&hour_add,x,y)){
                uint16_t t=Ertc_get_hour();
                if(t<23){
                    t+=1;Ertc_set_hour(t);hour_update_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&minute_minus,x,y)){
                uint8_t t=Ertc_get_minute();
                if(t>0){
                    t-=1;Ertc_set_minute(t);minute_update_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&minute_add,x,y)){
                uint8_t t=Ertc_get_minute();
                if(t<59){
                    t+=1;Ertc_set_minute(t);minute_update_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&second_minus,x,y)){
                uint8_t t=Ertc_get_second();
                if(t>0){
                    t-=1;Ertc_set_second(t);second_update_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&second_add,x,y)){
                uint8_t t=Ertc_get_second();
                if(t<59){
                    t+=1;Ertc_set_second(t);second_update_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&sys_test,x,y)){
                Ui_message_box("系统版本 260519",2);
                return;
            }
        }
        else if(sys_count_icon==0&&Ui_draw_button_get_with_draw(&camera_test,x,y)){
            camera_test.text="摄像机测试  停止";
            Ui_draw_button_text(&camera_test);
            Icon_can_press=0;
            is_camera_testing=1;
            TIM2_Stop();
            Camera_draw_lcd_start(60,340,360,360);
            sys_count_icon=15;
            return;
        }
        else if(sys_count_icon==0&&Ui_draw_button_get_with_draw(&color_saturation_button,x,y)){
            if(now_setting.color_saturation==4){now_setting.color_saturation=0;}
            else if(now_setting.color_saturation<4){now_setting.color_saturation+=1;}
            color_saturation_button.text=(char*)color_saturation_table[now_setting.color_saturation];
            Camera_set_color_saturation(now_setting.color_saturation);
            Ui_draw_button_text(&color_saturation_button);sys_count_icon=5;
            return;
        }
        else if(sys_count_icon==0&&Ui_draw_button_get_with_draw(&light_mode_button,x,y)){
            if(now_setting.light_mode==4){now_setting.light_mode=0;}
            else if(now_setting.light_mode<4){now_setting.light_mode+=1;}
            light_mode_button.text=(char*)light_mode_table[now_setting.light_mode];
            Camera_set_light_mode(now_setting.light_mode);
            Ui_draw_button_text(&light_mode_button);sys_count_icon=5;
            return;
        }
        else if(sys_count_icon==0&&Ui_draw_button_get_with_draw(&contrast_button,x,y)){
            if(now_setting.contrast==4){now_setting.contrast=0;}
            else if(now_setting.contrast<4){now_setting.contrast+=1;}
            contrast_button.text=(char*)contrast_table[now_setting.contrast];
            Camera_set_contrast(now_setting.contrast);
            Ui_draw_button_text(&contrast_button);sys_count_icon=5;
            return;
        }
        else if(sys_count_icon==0&&Ui_draw_button_get_with_draw(&brightness_button,x,y)){
            if(now_setting.brightness==4){now_setting.brightness=0;}
            else if(now_setting.brightness<4){now_setting.brightness+=1;}
            brightness_button.text=(char*)brightness_table[now_setting.brightness];
            Camera_set_brightness(now_setting.brightness);
            Ui_draw_button_text(&brightness_button);sys_count_icon=5;
            return;
        }
        else if(sys_count_icon==0&&Ui_draw_button_get_with_draw(&auto_exposure_button,x,y)){
            if(now_setting.auto_exposure_level==4){now_setting.auto_exposure_level=0;}
            else if(now_setting.auto_exposure_level<4){now_setting.auto_exposure_level+=1;}
            auto_exposure_button.text=(char*)auto_exposure_level_table[now_setting.auto_exposure_level];
            Camera_set_auto_exposure_level(now_setting.auto_exposure_level);
            Ui_draw_button_text(&auto_exposure_button);sys_count_icon=5;
            return;
        }
        else if(Ui_draw_button_get_with_draw(&save_setting_button,x,y)){
            Setting_save();
            Ui_message_box("保存完毕",2);
            return;
        }
    }
}
