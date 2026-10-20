#include "page_entering.h"
static uint8_t page_id;
static const uint16_t color_1=0x5D1D;
static const uint16_t color_button=0xBDFE;
static const uint16_t color_3=0x8FDF;
static const uint16_t color_4=0xDED5;
static struct Ui_button catch_button={
    .x=20,.y=528,.width=440,.height=96,.border_width=5,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="录入"};
static struct Ui_button release_button_1={
    .x=20,.y=640,.width=440,.height=48,.border_width=5,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="取消"};
static struct Ui_button number_add_one={
    .x=64,.y=360,.width=64,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+1"};
static struct Ui_button number_minus_one={
    .x=160,.y=360,.width=64,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-1"};
static struct Ui_button number_multi_two={
    .x=256,.y=360,.width=64,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="*2"};
static struct Ui_button number_divide_two={
    .x=352,.y=360,.width=64,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="/2"};
static struct Ui_button year_add_one={
    .x= 64,.y=512,.width=72,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button month_add_one={
    .x=144,.y=512,.width=72,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button day_add_one={
    .x=224,.y=512,.width=72,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button hour_add_one={
    .x=304,.y=512,.width=72,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button year_minus_one={
    .x= 64,.y=576,.width=72,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static struct Ui_button month_minus_one={
    .x=144,.y=576,.width=72,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static struct Ui_button day_minus_one={
    .x=224,.y=576,.width=72,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static struct Ui_button hour_minus_one={
    .x=304,.y=576,.width=72,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static struct Ui_button ok_button={
    .x=64,.y=620,.width=160,.height=64,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="确   定"};
static struct Ui_button release_button_2={
    .x=256,.y=620,.width=160,.height=64,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="取   消"};
static uint32_t pic_name_cache;
static char number_str_cache[15];
static float number_cache;
static char name_str_cache[data_base_max_name_len];
static uint8_t year_cache;
static uint8_t month_cache;
static uint8_t day_cache;
static uint8_t hour_cache;
static void var_init(void){
    number_str_cache[0] = '1';
    number_str_cache[1] = '\0';
    for(uint8_t i=2;i<15;i++){
        number_str_cache[i]=0;
    }
    number_cache        = 1;
    name_str_cache[0]   = '\0';
    for(uint8_t i=1;i<data_base_max_name_len;i++){
        number_str_cache[i]=0;
    }
    year_cache          = 0;
    month_cache         = 0;
    day_cache           = 0;
    hour_cache          = 0;
}
void page_id_1_bg_draw(){
    for(uint32_t j=0;j<24;j++){Lcd_draw_line(0,512+j,480,0,color_blend(color_3,color_1,j,24));}
    Lcd_draw_rect(0,536,480,144,color_1,1);
    for(uint32_t j=0;j<24;j++){Lcd_draw_line(0,680+j,480,0,color_blend(color_1,color_3,j,24));}
}
static void time_cache_draw(void){
    char tenp_s[3];
    sprintf(tenp_s,"%2d",year_cache );
    Lcd_draw_str(64 ,548,tenp_s,24,Lcd_color_black,0,color_4);
    sprintf(tenp_s,"%2d",month_cache);
    Lcd_draw_str(144,548,tenp_s,24,Lcd_color_black,0,color_4);
    sprintf(tenp_s,"%2d",day_cache  );
    Lcd_draw_str(224,548,tenp_s,24,Lcd_color_black,0,color_4);
    sprintf(tenp_s,"%2d",hour_cache );
    Lcd_draw_str(304,548,tenp_s,24,Lcd_color_black,0,color_4);
}
static void draw_after_input(void){
    Lcd_draw_str_chinese(64,484,"剩余有效时长(保质期):",24,Lcd_color_black,1,0);
    Lcd_draw_str_chinese(116,548,"年",24,Lcd_color_black,1,0);
    Lcd_draw_str_chinese(196,548,"月",24,Lcd_color_black,1,0);
    Lcd_draw_str_chinese(276,548,"天",24,Lcd_color_black,1,0);
    Lcd_draw_str_chinese(356,548,"小时",24,Lcd_color_black,1,0);
    Ui_draw_button_init_draw(&year_add_one);
    Ui_draw_button_init_draw(&month_add_one);
    Ui_draw_button_init_draw(&day_add_one);
    Ui_draw_button_init_draw(&hour_add_one);
    Ui_draw_button_init_draw(&year_minus_one);
    Ui_draw_button_init_draw(&month_minus_one);
    Ui_draw_button_init_draw(&day_minus_one);
    Ui_draw_button_init_draw(&hour_minus_one);
    Ui_draw_button_init_draw(&ok_button);
    Ui_draw_button_init_draw(&release_button_2);
    time_cache_draw();
}
static void draw_edit_ui(void){
    char path[50];
    page_lcd_clear(color_4);
    Lcd_draw_str_chinese(156,34,"添加新物品入库",24,Lcd_color_black,1,0);
    sprintf(path,"0:/smart_remd_stat/%010u.jpg",pic_name_cache);
    Lcd_draw_jpeg_pic(path,120,64,240,240);
    Lcd_draw_str_chinese(64,324,"数量:",24,Lcd_color_black,1,0);
    Lcd_draw_rect(130,320,286,32,Lcd_color_black,0);
    Lcd_draw_rect(131,321,284,30,Lcd_color_white,1);
    Ui_draw_button_init_draw(&number_add_one);
    Ui_draw_button_init_draw(&number_minus_one);
    Ui_draw_button_init_draw(&number_multi_two);
    Ui_draw_button_init_draw(&number_divide_two);
    Lcd_draw_str_chinese(64,404,"在下方输入名称与备注(最长18字)",24,Lcd_color_black,1,0);
    Lcd_draw_rect(5,444,470,32,Lcd_color_black,0);
    Lcd_draw_rect(6,445,468,30,Lcd_color_white,1);
    if(strlen(name_str_cache)==0){
        Lcd_draw_str_chinese(8,448,"请在此输入名称",24,Lcd_color_gray,1,0);
    }else{
        Lcd_draw_str_chinese(8,448,name_str_cache,24,0x0000,1,0);
    }
    draw_after_input();
}
static void number_entry_update(void){
    sprintf(number_str_cache, "%.5f", number_cache);
    float_string_refine(number_str_cache);
    Lcd_draw_rect(133,324,260,24,0xFFFF,1);
    Lcd_draw_str(133,324,number_str_cache,24,0x0000,0,0xFFFF);
}
void Page_entering_draw(void){
    page_id=0;
    page_lcd_clear(0xDED5);
    Lcd_draw_str_chinese(192,34,"录    入",24,0x0000,1,0);
    circle_width3(240,368,182,Lcd_color_black);
    Lcd_draw_circle(240,368,180,0x5EF5,1);
    Camera_icon_draw(192,308,Lcd_color_black);
    Lcd_draw_str_chinese(192,410,"拍摄录入",24,Lcd_color_black,1,color_button);
}
void Page_entering_process(uint16_t x,uint16_t y){
    if(page_id==0){
        uint32_t d_2=(x-240)*(x-240)+(y-368)*(y-368);
        if(d_2<=32500){
            page_id=1;
            Icon_can_press=0;
            page_lcd_clear(0xFFFF);
            page_id_1_bg_draw();
            Ui_draw_button_init_draw(&catch_button);
            Ui_draw_button_init_draw(&release_button_1);
            TIM2_Stop();
            Camera_draw_lcd_start(0,32,480,480);
        }
        return;
    }
    else if(page_id==1){
        if(Ui_draw_button_get(&catch_button,x,y)){
            Camera_draw_lcd_stop();
            pic_name_cache=get_time_fat_format();
            Camera_photo_save_jpeg(pic_name_cache);
            page_id=2;
            Ui_draw_button_pressed(&catch_button);
            page_lcd_clear(color_4);
            TIM2_Start();
            var_init();
            draw_edit_ui();
            number_entry_update();
            return;
        }
        if(Ui_draw_button_get(&release_button_1,x,y)){
            Camera_draw_lcd_stop();
            Ui_draw_button_pressed(&release_button_1);
            Icon_can_press=1;
            TIM2_Start();
            Page_entering_draw();
            return;
        }
    }
    else if(page_id==2){
        if(y<480){
            if(131<=x&&x<329&&321<=y&&y<351){
                while(1){
                    Key_board(number_str_cache,133,324,24,12,2);
                    if(conv_string_to_float(number_str_cache,&number_cache)==0){
                        if(number_cache>1000000){
                            Ui_message_box("数量太大了",1);
                            sprintf(number_str_cache,"1000000.0");
                            number_cache=1000000.0;
                        }
                        if(number_cache==0){
                            Ui_message_box("数量不为零",1);
                            sprintf(number_str_cache,"1");
                            number_cache=1;
                        }
                        Lcd_draw_rect(0,480,480,320,color_4,1);
                        draw_after_input();
                        Icons_draw();
                        number_entry_update();
                        break;
                    }
                    Ui_message_box("数量输入错误",1);
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&number_add_one,x,y)){
                if(number_cache<65530){
                    number_cache+=1;
                    number_entry_update();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&number_minus_one,x,y)){
                if(number_cache>1){
                    number_cache-=1;
                    number_entry_update();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&number_multi_two,x,y)){
                if(number_cache>0&&number_cache<32765){
                    number_cache*=2;
                    number_entry_update();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&number_divide_two,x,y)){
                if(number_cache>1){
                    number_cache/=2;
                    number_entry_update();
                }
                return;
            }
            else if(6<=x&&x<474&&445<=y&&y<475){
                Key_board(name_str_cache,8,448,24,data_base_max_name_len-2,1);
                if(strlen(name_str_cache)==0){
                    Lcd_draw_str_chinese(8,448,"请在此输入名称",24,Lcd_color_gray,1,0);
                }
                Lcd_draw_rect(0,480,480,320,color_4,1);
                draw_after_input();
                Icons_draw();
                return;
            }
        }
        else{
            if(Ui_draw_button_get_with_draw(&year_add_one,x,y)){
                if(year_cache<99){
                    year_cache+=1;
                    time_cache_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&month_add_one,x,y)){
                if(month_cache<12){
                    month_cache+=1;
                    time_cache_draw();
                }
                else if(month_cache==12&&year_cache<99){
                    year_cache+=1;
                    month_cache=0;
                    time_cache_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&day_add_one,x,y)){
                if(day_cache<30){
                    day_cache+=1;
                    time_cache_draw();
                }
                else if(day_cache==30&&month_cache<12){
                    month_cache+=1;
                    day_cache=0;
                    time_cache_draw();
                }
                else if(day_cache==30&&month_cache==12&&year_cache<99){
                    year_cache+=1;
                    day_cache=0;
                    month_cache=0;
                    time_cache_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&hour_add_one,x,y)){
                if(hour_cache<23){
                    hour_cache+=1;
                    time_cache_draw();
                }
                else if(hour_cache==23&&day_cache<30){
                    hour_cache=0;
                    day_cache+=1;
                    time_cache_draw();
                }
                else if(hour_cache==23&&day_cache==30&&month_cache<12){
                    month_cache+=1;
                    day_cache=0;
                    hour_cache=0;
                    time_cache_draw();
                }
                else if(hour_cache==23&&day_cache==30&&month_cache==12&&year_cache<99){
                    year_cache+=1;
                    day_cache=0;
                    month_cache=0;
                    hour_cache=0;
                    time_cache_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&year_minus_one,x,y)){
                if(year_cache>0){
                    year_cache-=1;
                    time_cache_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&month_minus_one,x,y)){
                if(month_cache>0){
                    month_cache-=1;
                    time_cache_draw();
                }
                else if(month_cache==0&&year_cache>0){
                    year_cache-=1;
                    month_cache=12;
                    time_cache_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&day_minus_one,x,y)){
                if(day_cache>0){
                    day_cache-=1;
                    time_cache_draw();
                }
                else if(day_cache==0&&month_cache>0){
                    month_cache-=1;
                    day_cache=30;
                    time_cache_draw();
                }
                else if(day_cache==0&&month_cache==0&&year_cache>0){
                    year_cache-=1;
                    day_cache=30;
                    month_cache=12;
                    time_cache_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&hour_minus_one,x,y)){
                if(hour_cache>0){
                    hour_cache-=1;
                    time_cache_draw();
                }
                else if(hour_cache==0&&day_cache>0){
                    hour_cache=23;
                    day_cache-=1;
                    time_cache_draw();
                }
                else if(hour_cache==0&&day_cache==0&&month_cache>0){
                    month_cache-=1;
                    hour_cache=23;
                    day_cache=30;
                    time_cache_draw();
                }
                else if(hour_cache==0&&day_cache==0&&month_cache==0&&year_cache>0){
                    year_cache-=1;
                    day_cache=30;
                    month_cache=12;
                    hour_cache=23;
                    time_cache_draw();
                }
                return;
            }
            else if(Ui_draw_button_get_with_draw(&ok_button,x,y)){
                Item_class add_one={0};
                add_one.save_pic_index=pic_name_cache;
                add_one.number=number_cache;
                time_class_get_now(&add_one.produce_time);
                add_one.bad_time=add_one.produce_time;
                uint32_t total_hours=0;
                total_hours+=hour_cache;
                total_hours+=(uint32_t)day_cache*24;
                total_hours+=(uint32_t)month_cache*30*24;
                total_hours+=(uint32_t)year_cache*365*24;
                while(total_hours--){
                    add_one.bad_time.hour++;
                    if(add_one.bad_time.hour>=24){
                        add_one.bad_time.hour=0;
                        add_one.bad_time.date++;
                        if(add_one.bad_time.date>get_month_days(add_one.bad_time.year,add_one.bad_time.month)){
                            add_one.bad_time.date=1;
                            add_one.bad_time.month++;
                            if(add_one.bad_time.month>12){
                                add_one.bad_time.month=1;
                                add_one.bad_time.year++;
                            }
                        }
                    }
                }
                strcat(add_one.name_str,name_str_cache);
                Item_data_add(&add_one);
                Icon_can_press=1;
                Page_entering_draw();
                return;
            }
            else if(Ui_draw_button_get_with_draw(&release_button_2,x,y)){
                char path[50];
                sprintf(path,"0:/smart_remd_stat/%010u.jpg",pic_name_cache);
                f_unlink(path);
                Icon_can_press=1;
                Page_entering_draw();
                return;
            }
        }
    }
}
