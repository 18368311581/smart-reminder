#include "page_other.h"
static uint8_t is_clocking_show=0;
static uint8_t page_id=0;
static const uint16_t color_button=0xBDFE;
static struct Ui_button describe_button={
    .x=48,.y=82,.width=96,.height=96,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="说明"};
static struct Ui_button clock_button={
    .x=192,.y=82,.width=96,.height=96,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="时钟"};
static struct Ui_button random_button={
    .x=336,.y=82,.width=96,.height=96,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="抽签"};
static struct Ui_button sense_button={
    .x=48,.y=226,.width=96,.height=96,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="测量"};
static struct Ui_button calc_button={
    .x=192,.y=226,.width=96,.height=96,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="计算"};
static struct Ui_button sys_close_button={
    .x=336,.y=226,.width=96,.height=96,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="休眠"};
static struct Ui_button return_button={
    .x=10,.y=34,.width=120,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="<-- 返回"};
static struct Ui_button btn_min_sub={
    .x=120,.y=260,.width=32,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static struct Ui_button btn_min_add={
    .x=352,.y=260,.width=32,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button btn_max_sub={
    .x=120,.y=320,.width=32,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static struct Ui_button btn_max_add={
    .x=352,.y=320,.width=32,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button btn_mode_change={
    .x=156,.y=380,.width=192,.height=64,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="单次"};
static struct Ui_button btn_start_stop={
    .x=156,.y=472,.width=192,.height=64,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="开始"};
static struct Ui_button measure_once_button={
    .x=100,.y=564,.width=280,.height=56,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="单次测量"};
static struct Ui_button measure_button={
    .x=100,.y=634,.width=280,.height=56,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="开始测量"};
static struct Ui_button calc_info_button={
    .x=355,.y=426,.width=120,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="显示提示"};
static char number_str_cache[18];
static uint8_t last_sec=0;
static uint8_t last_min=0;
static uint8_t last_hour=0;
static uint8_t last_day=0;
static const uint16_t color_clock_bg=0xEE9B;
static int ran_min=0;
static int ran_max=5;
static int ran_current=0;
static uint8_t ran_is_running=0;
static uint8_t ths_is_running=0;
static char calc_str_history_1[50];
static double calc_answer_history_1;
static char calc_str_history_2[50];
static double calc_answer_history_2;
static char calc_str_now[50];
static uint8_t calc_info_show=0;
static void draw_hand(uint16_t angle,uint16_t len,uint16_t color,uint8_t w){
    int32_t dx=(len*math_sin_table[angle])/1000;
    int32_t dy=(len*math_cos_table[angle])/1000;
    Lcd_draw_line_width(240,380,dx,-dy,color,w);
    Lcd_draw_circle(240+dx,380-dy,w/2,color,1);
}
void Ths_ui_task(void){
    if(ths_is_running){
        Ths_get();char str[12];
        sprintf(str,"%-10.2f",Ths_temperature_get);
        Lcd_draw_str_magnify(72,130,str,24,2,0xFC60,0,0xFFFF);
        sprintf(str,"%-10.2f",Ths_humidity_get);
        Lcd_draw_str_magnify(72,250,str,24,2,0x05FF,0,0xFFFF);
        sprintf(str,"%-10.2f",Ths_pressure_get/1000.0f);
        Lcd_draw_str_magnify(72,370,str,24,2,0x3666,0,0xFFFF);
        sprintf(str,"%-10.2f",Ths_altitude_get);
        Lcd_draw_str_magnify(72,490,str,24,2,0x79CF,0,0xFFFF);
    }
}
void CLK_SHW_func(void){
    if(is_clocking_show){
        char temp_s[32];
        uint8_t sec  = Ertc_get_second();
        uint8_t min  = Ertc_get_minute();
        uint8_t hour = Ertc_get_hour();
        uint8_t day  = Ertc_get_day();
        if(day!=last_day){
            last_day=day;
            uint16_t year = Ertc_get_year();
            uint8_t  mon  = Ertc_get_month();
            uint8_t  day  = Ertc_get_day();
            uint8_t  week = Ertc_get_week();
            Ertc_get_format_date(temp_s);
            Lcd_draw_str(128,80,temp_s,32,0x0000,0,0xFFFF);
            Ertc_get_format_week_short(temp_s);
            Lcd_draw_str(304,80,temp_s,32,0x0000,0,0xFFFF);
        }
        draw_hand((last_hour%12)*30+last_min/2,75,color_clock_bg,7);
        draw_hand(last_min*6+last_sec/10,125,color_clock_bg,5);
        draw_hand(last_sec*6,155,color_clock_bg,3);
        draw_hand((last_sec*6+180)%360,25,color_clock_bg,3);
        draw_hand((hour%12)*30+min/2,75,0x0000,7);
        draw_hand(min*6+sec/10,125,0x07E0,5);
        Lcd_draw_circle(240,380,8,0x0000,1);
        draw_hand(sec*6,155,0xF800,3);
        draw_hand((sec*6+180)%360,25,0xF800,3);
        Lcd_draw_circle(240,380,5,color_clock_bg,1);
        Lcd_draw_circle(240,380,6,0xF800,0);
        Lcd_draw_circle(240,380,5,0xF800,0);
        Lcd_draw_circle(240,380,4,0xF800,0);
        Lcd_draw_circle(240,380,3,color_clock_bg,0);
        sprintf(temp_s,"%02d:%02d:%02d",hour,min,sec);
        Lcd_draw_str(176,130,temp_s,32,0x0000,0,0xFFFF);
        last_sec=sec;last_min=min;last_hour=hour;
    }
}
static void random_draw_number(void){
    char buf[16];sprintf(buf,"%d",ran_current);
    Lcd_draw_rect(0,130,480,32,0xFFFF,1);
    Lcd_draw_str(240-8*strlen(buf),130,buf,32,0x0000,0,0xFFFF);
}
static void random_draw_range(void){
    char buf[16];sprintf(buf,"%d",ran_min);
    Lcd_draw_str(160,264,buf,24,0x0000,0,0xFFFF);
    sprintf(buf,"%d",ran_max);
    Lcd_draw_str(160,324,buf,24,0x0000,0,0xFFFF);
}
void Page_other_draw(void){
    page_lcd_clear(0xFFFF);
    if(page_id==0){
        Lcd_draw_str_chinese(156,34,"其  它  功  能",24,Lcd_color_black,1,0);
        Ui_draw_button_init_draw(&clock_button);
        Ui_draw_button_init_draw(&random_button);
        Ui_draw_button_init_draw(&describe_button);
        Ui_draw_button_init_draw(&sense_button);
        Ui_draw_button_init_draw(&calc_button);
        Ui_draw_button_init_draw(&sys_close_button);
    }
    else if(page_id==1){
        Ui_draw_button_init_draw(&return_button);
        Lcd_draw_str_chinese(156,34,"功  能  说  明",24,Lcd_color_black,1,0);
        Lcd_draw_str_chinese(228,202,"无",24,Lcd_color_black,1,0);
        Lcd_draw_str_chinese(2,600,"Copyright @ 2026 Peichen Jiang",24,Lcd_color_black,1,0);
        Lcd_draw_str_chinese(2,626,"This software is provided for personal",24,Lcd_color_black,1,0);
        Lcd_draw_str_chinese(2,652,"and educational use only.Please retain",24,Lcd_color_black,1,0);
        Lcd_draw_str_chinese(2,678,"this copyright notice in all copies.",24,Lcd_color_black,1,0);
    }
    else if(page_id==2){
        Ui_draw_button_init_draw(&return_button);
        Lcd_draw_str_chinese(192,34,"时    钟",24,Lcd_color_black,1,0);
        Lcd_draw_circle(240,380,201,color_clock_bg,1);
        circle_width3(240,380,203,0x0000);
        Lcd_draw_circle(240,380,203,0x0000,0);
        Lcd_draw_circle(240,380,204,0x0000,0);
        int32_t angle=0,x,y,dx,dy,isin,icos,x_t,y_t;
        for(int i=0;i<60;i++){
            isin=math_sin_table[angle];
            icos=math_cos_table[angle];
            x=+(200*isin)/1000;
            y=-(200*icos)/1000;
            if(i%5==0){
                dx=+(180*isin)/1000-x;
                dy=-(180*icos)/1000-y;
                x_t=+(170*isin)/1000+240-6;
                y_t=-(170*icos)/1000+380-12;
                if(i>49){
                    Lcd_draw_chr(x_t-6,y_t,'1',24,0x0000,1,0);
                    Lcd_draw_chr(x_t+6,y_t,i/5-10+'0',24,0x0000,1,0);
                }
                else if(i!=0){
                    Lcd_draw_chr(x_t,y_t,i/5+'0',24,0x0000,1,0);
                }
                else{
                    Lcd_draw_chr(228,198,'1',24,0x0000,1,0);
                    Lcd_draw_chr(240,198,'2',24,0x0000,1,0);
                }
            }
            else{
                dx=+(190*isin)/1000-x;
                dy=-(190*icos)/1000-y;
            }
            x+=240;y+=380;
            Lcd_draw_line_width(x,y,dx,dy,0x0000,3);
            angle+=6;
        }
        Lcd_draw_circle(240,380,6,0x0000,1);
        Lcd_draw_circle(240,380,205,0xFFFF,0);
        last_day=0;
        CLK_SHW_func();
    }
    else if(page_id==3){
        Ui_draw_button_init_draw(&return_button);
        Lcd_draw_str_chinese(192,34,"抽    签",24,Lcd_color_black,1,0);
        Lcd_draw_str_chinese(40,264,"最小值",24,0x0000,1,0);
        Lcd_draw_rect(156,260,192,32,0x0000,0);
        Lcd_draw_str_chinese(40,324,"最大值",24,0x0000,1,0);
        Lcd_draw_rect(156,320,192,32,0x0000,0);
        Ui_draw_button_init_draw(&btn_min_sub);
        Ui_draw_button_init_draw(&btn_min_add);
        Ui_draw_button_init_draw(&btn_max_sub);
        Ui_draw_button_init_draw(&btn_max_add);
        Ui_draw_button_init_draw(&btn_start_stop);
        Ui_draw_button_init_draw(&btn_mode_change);
        random_draw_range();
        random_draw_number();
    }
    else if(page_id==4){
        Ui_draw_button_init_draw(&return_button);
        Lcd_draw_str_chinese(192,34,"测    量",24,0x18E3,0,0xFFFF);
        Lcd_draw_str(392,146,"C",32,0xFC60,0,0xFFFF);
        Lcd_draw_circle(388,152,4,0xFC60,0);
        Lcd_draw_str(392,266,"%",32,0x05FF,0,0xFFFF);
        Lcd_draw_str(360,386,"kPa",32,0x3666,0,0xFFFF);
        Lcd_draw_str(392,506,"m",32,0x79CF,0,0xFFFF);
        Lcd_draw_line_width(40,190,400,0,0xC618,5);
        Lcd_draw_line_width(40,310,400,0,0xC618,5);
        Lcd_draw_line_width(40,430,400,0,0xC618,5);
        Lcd_draw_str_chinese(72,100,"温度",24,0xFC60,0,0xFFFF);
        Lcd_draw_str_chinese(72,220,"湿度",24,0x05FF,0,0xFFFF);
        Lcd_draw_str_chinese(72,340,"气压",24,0x3666,0,0xFFFF);
        Lcd_draw_str_chinese(72,460,"海拔",24,0x79CF,0,0xFFFF);
        Ui_draw_button_init_draw(&measure_once_button);
        Ui_draw_button_init_draw(&measure_button);
        ran_is_running=1;Ths_ui_task();ran_is_running=0;
    }
    else if(page_id==5){
        Ui_draw_button_init_draw(&return_button);
        Lcd_draw_str_chinese(192,34,"计    算",24,0x0000,0,0xFFFF);
        char temp_str[40];
        Lcd_draw_str_chinese(10,100,"历史结果 2(y)",24,0x0000,0,0xFFFF);
        Lcd_draw_str(10,136,calc_str_history_2,24,0x0000,0,0xFFFF);
        Lcd_draw_str(10,172,"=",24,0x0000,0,0xFFFF);
        sprintf(temp_str,"%30.15g",calc_answer_history_2);
        Lcd_draw_rect(30,172,360,24,0xFFFF,1);
        Lcd_draw_str(470-strlen(temp_str)*12,172,temp_str,24,0x0000,1,0);
        Lcd_draw_str_chinese(10,230,"历史结果 1(x)",24,0x0000,0,0xFFFF);
        Lcd_draw_str(10,266,calc_str_history_1,24,0x0000,0,0xFFFF);
        Lcd_draw_str(10,302,"=",24,0x0000,0,0xFFFF);
        sprintf(temp_str,"%30.15g",calc_answer_history_1);
        Lcd_draw_rect(30,302,360,24,0xFFFF,1);
        Lcd_draw_str(470-strlen(temp_str)*12,302,temp_str,24,0x0000,1,0);
        Lcd_draw_line_width(5,213,470,0,0xC618,5);
        Lcd_draw_line_width(5,343,470,0,0xC618,5);
        Lcd_draw_rect(6,360,468,32,0x0000,0);
        Lcd_draw_str(10,364,calc_str_now,24,0x0000,0,0xFFFF);
        Lcd_draw_line_width(5,409,470,0,0xC618,5);
        Ui_draw_button_init_draw(&calc_info_button);
        if(calc_info_show){
            Lcd_draw_str_chinese(10,434,"输入数学表达式进行计算                ",24,0x7BCF,0,0xFFFF);
            Lcd_draw_str_chinese(10,470,"可用算符: 加+ 减- 乘* 除/ 指数^ 括号()",24,0x7BCF,0,0xFFFF);
            Lcd_draw_str_chinese(10,506,"可用函数: 绝对值abs 自然对数ln 正切tan",24,0x7BCF,0,0xFFFF);
            Lcd_draw_str_chinese(10,542,"   正弦sin 余弦cos 开方sqrt 常用对数lg",24,0x7BCF,0,0xFFFF);
            Lcd_draw_str_chinese(10,578,"可用常量: 圆周率pi  自然常数e         ",24,0x7BCF,0,0xFFFF);
            Lcd_draw_str_chinese(10,614,"可用变量: 上一次结果x  上上次结果y    ",24,0x7BCF,0,0xFFFF);
            Lcd_draw_str_chinese(10,650,"注意: 乘法不可省略 三角函数是弧度制   ",24,0x7BCF,0,0xFFFF);
        }
    }
}
uint8_t sys_close_button_get(uint16_t x,uint16_t y){
    if(page_id==0&&Ui_draw_button_get_with_draw(&sys_close_button,x,y)){
        if(Ui_message_box("确认休眠",0)){return 1;}
    }
    return 0;
}
void Page_other_process(uint16_t x,uint16_t y){
    if(page_id==0){
        if(Ui_draw_button_get_with_draw(&describe_button,x,y)){
            page_id=1;Page_other_draw();
        }
        else if(Ui_draw_button_get_with_draw(&clock_button,x,y)){
            is_clocking_show=1;page_id=2;Page_other_draw();last_day=0;Icon_can_press=0;
        }
        else if(Ui_draw_button_get_with_draw(&random_button,x,y)){
            page_id=3;Page_other_draw();
        }
        else if(Ui_draw_button_get_with_draw(&sense_button,x,y)){
            page_id=4;Page_other_draw();
        }
        else if(Ui_draw_button_get_with_draw(&calc_button,x,y)){
            page_id=5;Page_other_draw();
        }
    }
    else if(page_id==1){
        if(Ui_draw_button_get_with_draw(&return_button,x,y)){
            page_id=0;Page_other_draw();Icon_can_press=1;
        }
    }
    else if(page_id==2){
        if(Ui_draw_button_get_with_draw(&return_button,x,y)){
            is_clocking_show=0;page_id=0;Page_other_draw();Icon_can_press=1;
        }
    }
    else if(page_id==3){
        if(Ui_draw_button_get_with_draw(&return_button,x,y)){
            ran_is_running=0;page_id=0;Page_other_draw();Icon_can_press=1;
        }
        else if(Ui_draw_button_get_with_draw(&btn_min_sub,x,y)){
            ran_min--;
            if(ran_min>=ran_max){ran_min=ran_max-1;}
            random_draw_range();
        }
        else if(Ui_draw_button_get_with_draw(&btn_min_add,x,y)){
            ran_min++;
            if(ran_min>=ran_max){ran_min=ran_max-1;}
            random_draw_range();
        }
        else if(156<=x&&x<348&&260<=y&&y<292&&sys_count_icon==0){
            sys_count_icon=5;sprintf(number_str_cache,"%d",ran_min);
            while(1){
                Key_board(number_str_cache,160,264,24,14,2);
                if(conv_string_to_int32(number_str_cache,&ran_min)==0){
                    if(ran_max<=ran_min){
                        Ui_message_box("数量大于最大值",1);
                        ran_min=ran_max-1;
                        sprintf(number_str_cache,"%d",ran_min);
                    }
                    if(ran_min<-999999){
                        Ui_message_box("数量太小了",1);
                        sprintf(number_str_cache,"-999999");
                        ran_min=-999999;
                    }
                    Lcd_draw_rect(0,480,480,320,0xFFFF,1);
                    Ui_draw_button_init_draw(&btn_start_stop);
                    random_draw_range();
                    Icons_draw();break;
                }
                Ui_message_box("数量输入错误",1);
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&btn_max_sub,x,y)){
            ran_max--;
            if(ran_max<=ran_min){ran_max=ran_min+1;}
            random_draw_range();
            return;
        }
        else if(Ui_draw_button_get_with_draw(&btn_max_add,x,y)){
            ran_max++;
            random_draw_range();
            return;
        }
        else if(156<=x&&x<348&&320<=y&&y<352&&sys_count_icon==0){
            sys_count_icon=5;sprintf(number_str_cache,"%d",ran_max);
            while(1){
                Key_board(number_str_cache,160,324,24,14,2);
                if(conv_string_to_int32(number_str_cache,&ran_max)==0){
                    if(ran_max<=ran_min){
                        Ui_message_box("数量小于最小值",1);
                        ran_max=ran_min+1;
                        sprintf(number_str_cache,"%d",ran_max);
                    }
                    if(ran_max>999999){
                        Ui_message_box("数量太大了",1);
                        sprintf(number_str_cache,"999999");
                        ran_max=999999;
                    }
                    Lcd_draw_rect(0,480,480,320,0xFFFF,1);
                    Ui_draw_button_init_draw(&btn_start_stop);
                    random_draw_range();
                    Icons_draw();break;
                }
                Ui_message_box("数量输入错误",1);
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&btn_mode_change,x,y)&&sys_count_icon==0){
            if(!ran_is_running){ran_current=Rng_get_range_int(ran_min,ran_max);random_draw_number();}
            sys_count_icon=5;
        }
        else if(Ui_draw_button_get_with_draw(&btn_start_stop,x,y)&&sys_count_icon==0){
            sys_count_icon=5;
            if(ran_is_running==0){Icon_can_press=0;btn_start_stop.text="停止";}
            else{Icon_can_press=1;btn_start_stop.text="开始";}
            ran_is_running=1-ran_is_running;Ui_draw_button_text(&btn_start_stop);
        }
    }
    else if(page_id==4){
        if(Ui_draw_button_get_with_draw(&return_button,x,y)){
            ths_is_running=0;page_id=0;Page_other_draw();Icon_can_press=1;
        }
        else if(Ui_draw_button_get_with_draw(&measure_once_button,x,y)&&sys_count_icon==0){
            if(!ths_is_running){ths_is_running=1;Ths_ui_task();ths_is_running=0;}
            sys_count_icon=5;
        }
        else if(Ui_draw_button_get_with_draw(&measure_button,x,y)&&sys_count_icon==0){
            ths_is_running=1-ths_is_running;
            if(ths_is_running){measure_button.text="停止测量";Icon_can_press=0;}
            else{measure_button.text="开始测量";Icon_can_press=1;}
            Ui_draw_button_text(&measure_button);
            sys_count_icon=5;
        }
    }
    else if(page_id==5){
        if(Ui_draw_button_get_with_draw(&return_button,x,y)){
            page_id=0;Page_other_draw();
        }
        else if(6<=x&&x<474&&360<=y&&y<392&&sys_count_icon==0){
            sys_count_icon=5;
            double answer;char calc_str[50]={0};char temp_str[40];
            strcpy(calc_str,calc_str_now);
            Key_board(calc_str,10,364,24,38,2);
            Ui_draw_button_init_draw(&calc_info_button);
            if(calc_info_show){
                Lcd_draw_str_chinese(10,470,"可用算符: 加+ 减- 乘* 除/ 指数^ 括号()",24,0x7BCF,0,0xFFFF);
                Lcd_draw_str_chinese(10,506,"可用函数: 绝对值abs 自然对数ln 正切tan",24,0x7BCF,0,0xFFFF);
                Lcd_draw_str_chinese(10,542,"   正弦sin 余弦cos 开方sqrt 常用对数lg",24,0x7BCF,0,0xFFFF);
                Lcd_draw_str_chinese(10,578,"可用常量: 圆周率pi  自然常数e         ",24,0x7BCF,0,0xFFFF);
                Lcd_draw_str_chinese(10,614,"可用变量: 上一次结果x  上上次结果y    ",24,0x7BCF,0,0xFFFF);
                Lcd_draw_str_chinese(10,650,"注意: 乘法不可省略 三角函数是弧度制   ",24,0x7BCF,0,0xFFFF);
            }
            Icons_draw();
            if(strlen(calc_str)==0){return;}
            if(smart_calc_from_str(calc_str,&answer,calc_answer_history_1,calc_answer_history_2)){
                calc_answer_history_2=calc_answer_history_1;
                calc_answer_history_1=answer;
                strcpy(calc_str_history_2,calc_str_history_1);
                strcpy(calc_str_history_1,calc_str);
                memset(calc_str_now,0,50);
                Lcd_draw_rect(10,136,470,24,0xFFFF,1);
                Lcd_draw_str(10,136,calc_str_history_2,24,0x0000,0,0xFFFF);
                sprintf(temp_str,"%30.15g",calc_answer_history_2);
                Lcd_draw_rect(30,172,360,24,0xFFFF,1);
                Lcd_draw_str(470-strlen(temp_str)*12,172,temp_str,24,0x0000,0,0xFFFF);
                Lcd_draw_rect(10,266,470,24,0xFFFF,1);
                Lcd_draw_str(10,266,calc_str_history_1,24,0x0000,0,0xFFFF);
                sprintf(temp_str,"%30.15g",calc_answer_history_1);
                Lcd_draw_rect(30,302,360,24,0xFFFF,1);
                Lcd_draw_str(470-strlen(temp_str)*12,302,temp_str,24,0x0000,0,0xFFFF);
                Lcd_draw_rect(7,361,466,30,0xFFFF,1);
            }
            else{
                Ui_message_box("表达式错误",1);
                strcpy(calc_str_now,calc_str);
            }
        }
        else if(Ui_draw_button_get_with_draw(&calc_info_button,x,y)&&sys_count_icon==0){
            sys_count_icon=5;
            if(calc_info_show==1){
                calc_info_button.text="显示提示";
                Lcd_draw_rect(0,420,480,270,0xFFFF,1);
            }
            else{
                calc_info_button.text="隐藏提示";
                Lcd_draw_str_chinese(10,434,"输入数学表达式进行计算                ",24,0x7BCF,0,0xFFFF);
                Lcd_draw_str_chinese(10,470,"可用算符: 加+ 减- 乘* 除/ 指数^ 括号()",24,0x7BCF,0,0xFFFF);
                Lcd_draw_str_chinese(10,506,"可用函数: 绝对值abs 自然对数ln 正切tan",24,0x7BCF,0,0xFFFF);
                Lcd_draw_str_chinese(10,542,"   正弦sin 余弦cos 开方sqrt 常用对数lg",24,0x7BCF,0,0xFFFF);
                Lcd_draw_str_chinese(10,578,"可用常量: 圆周率pi  自然常数e         ",24,0x7BCF,0,0xFFFF);
                Lcd_draw_str_chinese(10,614,"可用变量: 上一次结果x  上上次结果y    ",24,0x7BCF,0,0xFFFF);
                Lcd_draw_str_chinese(10,650,"注意: 乘法不可省略 三角函数是弧度制   ",24,0x7BCF,0,0xFFFF);
            }
            calc_info_show=1-calc_info_show;
            Ui_draw_button_init_draw(&calc_info_button);
        }
    }
}
void Random_ui_task(void){
    if(ran_is_running){
        ran_current=Rng_get_range_int(ran_min,ran_max);
        random_draw_number();
    }
}
