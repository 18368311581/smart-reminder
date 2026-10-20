#include "page_home.h"
static uint8_t sort_type=0;
static uint8_t sort_is_big=1;
static char sort_str[data_base_max_name_len+1];
static uint8_t page_id=0;
static uint16_t index_show_page_0=0;
static uint16_t index_show_page_1=0;
static struct time_class now_t;
static Item_class item_cache;
static char number_str_cache[15];
static const uint16_t color_button=0xBDFE;
static const uint16_t color_bg=0x6657;
static const uint16_t color_fr=0x2C2F;
static struct Ui_button button_pgup={
    .x=0,.y=160,.width=480,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="向上翻页"};
static struct Ui_button button_pgdn={
    .x=0,.y=672,.width=480,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="向下翻页"};
static struct Ui_button button_delete[4]={
   {.x=70,.y=398,.width=100,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="删除"},
   {.x=310,.y=398,.width=100,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="删除"},
   {.x=70,.y=638,.width=100,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="删除"},
   {.x=310,.y=638,.width=100,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="删除"}};
static struct Ui_button button_big={
    .x=10,.y=76,.width=104,.height=40,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="        "};
static void draw_pic_single(uint16_t x,uint16_t y,uint16_t rindex){
    Item_class* p_item=&(item_data.items[item_data.index_sorted[rindex]]);
    char temp_s[50];sprintf(temp_s,"0:/smart_remd_stat/%010u.jpg",p_item->save_pic_index);
    Lcd_draw_jpeg_pic(temp_s,x,y,240,240);
    time_diff_str(&p_item->bad_time,&now_t,temp_s);
    Lcd_draw_str_chinese(x+1,y+1,temp_s,16,0x0000,0,0xFFFF);
    Lcd_draw_rect(x,y,162,18,0x0000,0);
    strcpy(temp_s,p_item->name_str);
    str_cut_to_ellipsis_with_limit(temp_s,18);
    Lcd_draw_str_invertly(x+1,y+18,temp_s,16);
    Lcd_draw_str_invertly(x+1,y+34,"数量  ",16);
    sprintf(temp_s,"%.5f",p_item->number);
    float_string_refine(temp_s);
    Lcd_draw_str_invertly(x+49,y+34,temp_s,16);
    sprintf(temp_s,"%u/%u",rindex+1,item_data.sorted_count);
    Lcd_draw_str_invertly(x+239-strlen(temp_s)*8,y+34,temp_s,16);
}
static void draw_pics_i(uint8_t i){
    if(i==0){
        if(index_show_page_0*4>=item_data.sorted_count){return;}
        draw_pic_single(0,192,index_show_page_0*4);
        Ui_draw_button_init_draw(&button_delete[0]);
    }
    else if(i==1){
        if(index_show_page_0*4+1>=item_data.sorted_count){return;}
        draw_pic_single(240,192,index_show_page_0*4+1);
        Ui_draw_button_init_draw(&button_delete[1]);
    }
    else if(i==2){
        if(index_show_page_0*4+2>=item_data.sorted_count){return;}
        draw_pic_single(0,432,index_show_page_0*4+2);
        Ui_draw_button_init_draw(&button_delete[2]);
    }
    else if(i==3){
        if(index_show_page_0*4+3>=item_data.sorted_count){return;}
        draw_pic_single(240,432,index_show_page_0*4+3);
        Ui_draw_button_init_draw(&button_delete[3]);
    }
}
static const uint16_t x_types[8]={128,200,210,282,292,364,374,470};
static const char x_types_str[4][10]={"生产日","过期日","保质期","名称搜索"};
static const char big_types_str[2][9]={"升序排序","降序排序"};
static void x_types_draw(void){
    Lcd_draw_rect(0,76,480,40,color_bg,1);
    for(uint8_t i=0;i<4;i++){
        if(i==sort_type){
            Lcd_draw_rect(x_types[i*2]-5,76,x_types[i*2+1]-x_types[i*2]+10,40,color_fr,1);
            Lcd_draw_str_chinese(x_types[i*2],84,(char*)x_types_str[i],24,0xFFFF,1,0);
        }
        else{
            Lcd_draw_str_chinese(x_types[i*2],84,(char*)x_types_str[i],24,0x0000,1,0);
        }
    }
}
static void sort_type_touch_get_with_draw(uint16_t x,uint16_t y){
    if(y<=76||y>116||sys_count_icon>0){return;}
    uint8_t index_get=4;
    for(uint8_t i=0;i<4;i++){
        if(x_types[2*i]<=x&&x<=x_types[2*i+1]){
            index_get=i;break;
        }
    }
    if(index_get<4&&index_get!=sort_type){
        Lcd_draw_rect(x_types[index_get*2]-5,76,x_types[index_get*2+1]-x_types[index_get*2]+10,40,color_fr,1);
        Lcd_draw_str_chinese(x_types[index_get*2],84,(char*)x_types_str[index_get],24,0xFFFF,1,0);
        Lcd_draw_rect(x_types[sort_type*2]-5,76,x_types[sort_type*2+1]-x_types[sort_type*2]+10,40,color_bg,1);
        Lcd_draw_str_chinese(x_types[sort_type*2],84,(char*)x_types_str[sort_type],24,0x0000,1,0);
        sort_type=index_get;
        Item_data_sort(sort_type,sort_is_big,0);
        delay_ms(100);
        Lcd_draw_rect(0,192,480,480,0xFFFF,1);
        time_class_get_now(&now_t);
        draw_pics_i(0);draw_pics_i(1);draw_pics_i(2);draw_pics_i(3);
        sys_count_icon=5;
    }
}
static struct Ui_button button_unsave={
    .x=10,.y=234,.width=104,.height=40,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="<-- 返回"};
static struct Ui_button button_save={
    .x=366,.y=234,.width=104,.height=40,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="保存更改"};
static struct Ui_button number_add_one={
    .x=254,.y=60,.width=48,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+1"};
static struct Ui_button number_minus_one={
    .x=310,.y=60,.width=48,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-1"};
static struct Ui_button number_multi_two={
    .x=366,.y=60,.width=48,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="*2"};
static struct Ui_button number_divide_two={
    .x=422,.y=60,.width=48,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="/2"};
static struct Ui_button year_minus={
    .x=174,.y=156,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static void year_update_draw(void){
    char tep[5];
    sprintf(tep,"%4d",item_cache.bad_time.year+2000);
    Lcd_draw_str(196,160,tep,24,Lcd_color_black,0,color_bg);
}
static struct Ui_button year_add={
    .x=246,.y=156,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button month_minus={
    .x=270,.y=156,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static void month_update_draw(void){
    char tep[3];
    sprintf(tep,"%2d",item_cache.bad_time.month);
    Lcd_draw_str(292,160,tep,24,Lcd_color_black,0,color_bg);
}
static struct Ui_button month_add={
    .x=318,.y=156,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button day_minus={
    .x=342,.y=156,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static void day_update_draw(void){
    char tep[3];
    sprintf(tep,"%2d",item_cache.bad_time.date);
    Lcd_draw_str(364,160,tep,24,Lcd_color_black,0,color_bg);
}
static struct Ui_button day_add={
    .x=390,.y=156,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button hour_minus={
    .x=196,.y=190,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static void hour_update_draw(void){
    char tep[3];
    sprintf(tep,"%2d",item_cache.bad_time.hour);
    Lcd_draw_str(220,194,tep,24,Lcd_color_black,0,color_bg);
}
static struct Ui_button hour_add={
    .x=246,.y=190,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button minute_minus={
    .x=270,.y=190,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static void minute_update_draw(void){
    char tep[3];
    sprintf(tep,"%2d",item_cache.bad_time.minute);
    Lcd_draw_str(292,194,tep,24,Lcd_color_black,0,color_bg);
}
static struct Ui_button minute_add={
    .x=318,.y=190,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static struct Ui_button second_minus={
    .x=342,.y=190,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="-"};
static void second_update_draw(void){
    char tep[3];
    sprintf(tep,"%2d",item_cache.bad_time.second);
    Lcd_draw_str(364,194,tep,24,Lcd_color_black,0,color_bg);
}
static struct Ui_button second_add={
    .x=390,.y=190,.width=20,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="+"};
static void number_entry_update(void){
    sprintf(number_str_cache, "%.5f",item_cache.number);
    float_string_refine(number_str_cache);
    Lcd_draw_rect(63,64,180,24,0xFFFF,1);
    Lcd_draw_str(63,64,number_str_cache,24,0x0000,0,0xFFFF);
}
void Page_home_draw(void){
    page_lcd_clear(0xFFFF);
    if(page_id==0){
        Lcd_draw_rect(0,32,480,128,color_bg,1);
        Lcd_draw_str_chinese(192,34,"主    页",24,0x0000,1,0);
        x_types_draw();
        Item_data_sort(sort_type,sort_is_big,0);
        if(index_show_page_0*4>=item_data.sorted_count){
            if(item_data.sorted_count==0){
                index_show_page_0=0;
            }
            else{
                index_show_page_0=(item_data.sorted_count-1)/4;
            }
        }
        button_big.text=(char*)big_types_str[sort_is_big];
        Ui_draw_button_init_draw(&button_big);
        Lcd_draw_rect(8,120,464,32,0x0000,0);
        Lcd_draw_rect(9,121,462,30,0xFFFF,1);
        if(strlen(sort_str)==0){
            Lcd_draw_str_chinese(12,124,"请在此输入搜索名称",24,Lcd_color_gray,1,0);
        }else{
            Lcd_draw_str_chinese(12,124,sort_str,24,0x0000,1,0);
        }
        Ui_draw_button_init_draw(&button_pgup);
        Lcd_draw_rect(0,192,480,480,0xFFFF,1);
        time_class_get_now(&now_t);
        draw_pics_i(0);draw_pics_i(1);draw_pics_i(2);draw_pics_i(3);
        Ui_draw_button_init_draw(&button_pgdn);
    }
    else if(page_id==1){
        char temp_s[50];page_lcd_clear(color_bg);
        Lcd_draw_str_chinese(174,34,"编 辑 物 品",24,Lcd_color_black,1,0);
        sprintf(temp_s,"0:/smart_remd_stat/%010u.jpg",item_cache.save_pic_index);
        Lcd_draw_jpeg_pic(temp_s,0,224,480,480);
        Lcd_draw_str_chinese(10,64,"数量",24,Lcd_color_black,1,0);
        Lcd_draw_rect(60,60,184,32,Lcd_color_black,0);
        Lcd_draw_rect(61,61,182,30,Lcd_color_white,1);
        number_entry_update();
        Ui_draw_button_init_draw(&number_add_one);
        Ui_draw_button_init_draw(&number_minus_one);
        Ui_draw_button_init_draw(&number_multi_two);
        Ui_draw_button_init_draw(&number_divide_two);
        Lcd_draw_rect(5,94,470,32,Lcd_color_black,0);
        Lcd_draw_rect(6,95,468,30,Lcd_color_white,1);
        if(strlen(item_cache.name_str)==0){
            Lcd_draw_str_chinese(8,98,"请在此输入名称",24,Lcd_color_gray,1,0);
        }else{
            Lcd_draw_str_chinese(8,98,item_cache.name_str,24,0x0000,1,0);
        }
        struct time_class x=item_cache.produce_time;
        sprintf(temp_s,"生产时间: %04d-%02d-%02d %02d:%02d:%02d",x.year+2000,x.month,x.date,x.hour,x.minute,x.second);//temp_s="生产时间 2024-02-01 13:10:10"
        Lcd_draw_str_chinese(64,130,temp_s,24,0x0000,1,0);
        Lcd_draw_str_chinese(64,160,"过期日期:",24,0x0000,1,0);
        Ui_draw_button_init_draw(&year_minus);
        year_update_draw();
        Ui_draw_button_init_draw(&year_add);
        Ui_draw_button_init_draw(&month_minus);
        month_update_draw();
        Ui_draw_button_init_draw(&month_add);
        Ui_draw_button_init_draw(&day_minus);
        day_update_draw();
        Ui_draw_button_init_draw(&day_add);
        Lcd_draw_str_chinese(64,194,"过期时间:",24,0x0000,1,0);
        Ui_draw_button_init_draw(&hour_minus);
        hour_update_draw();
        Ui_draw_button_init_draw(&hour_add);
        Ui_draw_button_init_draw(&minute_minus);
        minute_update_draw();
        Ui_draw_button_init_draw(&minute_add);
        Ui_draw_button_init_draw(&second_minus);
        second_update_draw();
        Ui_draw_button_init_draw(&second_add);
        Ui_draw_button_init_draw(&button_save);
        Ui_draw_button_init_draw(&button_unsave);
    }
}
void Page_home_process(uint16_t x,uint16_t y){
    if(page_id==0){
        if(y>76&y<=116){
            if(Ui_draw_button_get_with_draw(&button_big,x,y)&&sys_count_icon==0){
                sort_is_big=1-sort_is_big;
                button_big.text=(char*)big_types_str[sort_is_big];
                Ui_draw_button_text(&button_big);
                Item_data_sort(sort_type,sort_is_big,0);
                Lcd_draw_rect(0,192,480,480,0xFFFF,1);
                time_class_get_now(&now_t);
                draw_pics_i(0);draw_pics_i(1);draw_pics_i(2);draw_pics_i(3);
                sys_count_icon=10;return;
            }
            sort_type_touch_get_with_draw(x,y);return;
        }
        else if((9<=x&&x<471&&121<=y&&y<151)){
            Lcd_draw_rect(9,121,462,30,0xFFFF,1);
            Key_board(sort_str,12,124,24,data_base_max_name_len-2,1);
            if(strlen(sort_str)==0){
                Lcd_draw_str_chinese(12,124,"请在此输入搜索名称",24,Lcd_color_gray,1,0);
            }
            strcpy(item_data.sort_name_str,sort_str);
            draw_pics_i(2);draw_pics_i(3);Ui_draw_button_init_draw(&button_pgdn);
            Icons_draw();sys_count_icon=5;return;
        }
        else if(Ui_draw_button_get_with_draw(&button_pgup,x,y)&&sys_count_icon==0){
            if(index_show_page_0>0){
                index_show_page_0-=1;
                Lcd_draw_rect(0,192,480,480,0xFFFF,1);
                time_class_get_now(&now_t);
                draw_pics_i(0);draw_pics_i(1);draw_pics_i(2);draw_pics_i(3);
            }sys_count_icon=5;return;
        }
        else if(Ui_draw_button_get_with_draw(&button_pgdn,x,y)&&sys_count_icon==0){
            if(index_show_page_0*4+4<item_data.sorted_count){
                index_show_page_0+=1;
                Lcd_draw_rect(0,192,480,480,0xFFFF,1);
                time_class_get_now(&now_t);
                draw_pics_i(0);draw_pics_i(1);draw_pics_i(2);draw_pics_i(3);
            }sys_count_icon=5;return;
        }
        else if(192<=y&&y<672&&sys_count_icon==0){
            for(uint8_t i=0;i<4;i++){
                if(Ui_draw_button_get_with_draw(&button_delete[i],x,y)){
                    Item_data_delete(item_data.index_sorted[index_show_page_0*4+i]);
                    Item_data_sort(sort_type,sort_is_big,0);
                    if(item_data.sorted_count%4==0&&item_data.sorted_count>0){index_show_page_0-=1;}
                    Lcd_draw_rect(0,192,480,480,0xFFFF,1);
                    time_class_get_now(&now_t);
                    draw_pics_i(0);draw_pics_i(1);draw_pics_i(2);draw_pics_i(3);
                    sys_count_icon=5;
                    return;
                }
            }
            uint8_t i=(x>=240?1:0)+(y>=432?2:0);
            if(i+index_show_page_0*4>=item_data.sorted_count){return;}
            page_id=1;index_show_page_1=i+index_show_page_0*4;
            item_cache=item_data.items[item_data.index_sorted[index_show_page_1]];
            Page_home_draw();Icon_can_press=0;
        }
    }
    else if(page_id==1){
        if(61<=x&&x<243&&61<=y&&y<91){
            while(1){
                Key_board(number_str_cache,63,64,24,12,2);float tk;
                if(conv_string_to_float(number_str_cache,&tk)==0){
                    item_cache.number=tk;
                    if(item_cache.number>1000000){
                        Ui_message_box("数量太大了",1);
                        sprintf(number_str_cache,"1000000.0");
                        item_cache.number=1000000.0;
                    }
                    if(item_cache.number==0){
                        Ui_message_box("数量不为零",1);
                        sprintf(number_str_cache,"1");
                        item_cache.number=1;
                    }
                    Lcd_draw_rect(0,480,480,320,color_bg,1);
                    char temp_s[50];sprintf(temp_s,"0:/smart_remd_stat/%010u.jpg",item_cache.save_pic_index);
                    Lcd_draw_jpeg_pic(temp_s,0,224,480,480);
                    Ui_draw_button_init_draw(&button_save);
                    Ui_draw_button_init_draw(&button_unsave);
                    Icons_draw();
                    number_entry_update();
                    break;
                }
                Ui_message_box("数量输入错误",1);
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&number_add_one,x,y)){
            if(item_cache.number<65530){
                item_cache.number+=1;
                number_entry_update();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&number_minus_one,x,y)){
            if(item_cache.number>1){
                item_cache.number-=1;
                number_entry_update();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&number_multi_two,x,y)){
            if(item_cache.number>0&&item_cache.number<32765){
                item_cache.number*=2;
                number_entry_update();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&number_divide_two,x,y)){
            if(item_cache.number>1){
                item_cache.number/=2;
                number_entry_update();
            }
            return;
        }
        else if(6<=x&&x<474&&95<=y&&y<125){
            Key_board(item_cache.name_str,8,98,24,data_base_max_name_len-2,1);
            if(strlen(item_cache.name_str)==0){
                Lcd_draw_str_chinese(8,98,"请在此输入名称",24,Lcd_color_gray,1,0);
            }
            Lcd_draw_rect(0,480,480,320,color_bg,1);
            char temp_s[50];sprintf(temp_s,"0:/smart_remd_stat/%010u.jpg",item_cache.save_pic_index);
            Lcd_draw_jpeg_pic(temp_s,0,224,480,480);
            Ui_draw_button_init_draw(&button_save);
            Ui_draw_button_init_draw(&button_unsave);
            Icons_draw();
            return;
        }
        else if(Ui_draw_button_get_with_draw(&year_minus,x,y)){
            if(item_cache.bad_time.year>20){
                item_cache.bad_time.year-=1;year_update_draw();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&year_add,x,y)){
            if(item_cache.bad_time.year<99){
                item_cache.bad_time.year+=1;year_update_draw();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&month_minus,x,y)){
            if(item_cache.bad_time.month>1){
                item_cache.bad_time.month-=1;month_update_draw();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&month_add,x,y)){
            if(item_cache.bad_time.month<12){
                item_cache.bad_time.month+=1;month_update_draw();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&day_minus,x,y)){
            if(item_cache.bad_time.date>1){
                item_cache.bad_time.date-=1;day_update_draw();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&day_add,x,y)){
            uint8_t x=get_month_days(item_cache.bad_time.year,item_cache.bad_time.month);
            if(item_cache.bad_time.date<x){
                item_cache.bad_time.date+=1;day_update_draw();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&hour_minus,x,y)){
            if(item_cache.bad_time.hour>0){
                item_cache.bad_time.hour-=1;hour_update_draw();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&hour_add,x,y)){
            if(item_cache.bad_time.hour<23){
                item_cache.bad_time.hour+=1;hour_update_draw();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&minute_minus,x,y)){
            if(item_cache.bad_time.minute>0){
                item_cache.bad_time.minute-=1;minute_update_draw();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&minute_add,x,y)){
            if(item_cache.bad_time.minute<59){
                item_cache.bad_time.minute+=1;minute_update_draw();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&second_minus,x,y)){
            if(item_cache.bad_time.second>0){
                item_cache.bad_time.second-=1;second_update_draw();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&second_add,x,y)){
            if(item_cache.bad_time.second<59){
                item_cache.bad_time.second+=1;second_update_draw();
            }
            return;
        }
        else if(Ui_draw_button_get_with_draw(&button_save,x,y)){
            if(time_to_seconds(&(item_cache.produce_time))>=(time_to_seconds(&(item_cache.bad_time)))){
                Ui_message_box("过期应大于生产",1);
                sys_count_icon=5;
                return;
            }
            page_id=0;Icon_can_press=1;sys_count_icon=5;
            item_data.items[item_data.index_sorted[index_show_page_1]]=item_cache;
            Item_data_set_flag();
            Item_data_sort(sort_type,sort_is_big,0);
            for(uint16_t i=0;i<item_data.sorted_count;i++){
                if(time_is_equal(&(item_data.items[item_data.index_sorted[i]].produce_time),&(item_cache.produce_time))){
                    index_show_page_0=i/4;
                    index_show_page_1=i;
                    break;
                }
            }
            memset(&item_cache,0,sizeof(Item_class));
            Page_home_draw();
            return;
        }
        else if(Ui_draw_button_get_with_draw(&button_unsave,x,y)){
            page_id=0;Icon_can_press=1;sys_count_icon=5;
            memset(&item_cache,0,sizeof(Item_class));
            Page_home_draw();
            return;
        }
    }
}
static void update_draw(uint16_t x,uint16_t y,uint16_t rindex){
    Item_class* p_item=&(item_data.items[item_data.index_sorted[rindex]]);
    char temp_s[50];time_diff_str(&p_item->bad_time,&now_t,temp_s);
    Lcd_draw_str_chinese(x+1,y+1,temp_s,16,0x0000,0,0xFFFF);
}
void page_home_show_update(void){
    if(Icon_index_now==1){
        if(page_id==0){
            time_class_get_now(&now_t);
            if(index_show_page_0*4<item_data.sorted_count){update_draw(0,192,index_show_page_0*4);}
            if(index_show_page_0*4+1<item_data.sorted_count){update_draw(240,192,index_show_page_0*4+1);}
            if(index_show_page_0*4+2<item_data.sorted_count){update_draw(0,432,index_show_page_0*4+2);}
            if(index_show_page_0*4+3<item_data.sorted_count){update_draw(240,432,index_show_page_0*4+3);}
        }
    }
}
void page_warning_jump(Item_class* item){
    page_id=1;Icon_can_press=0;Icon_index_now=1;
    Item_data_sort(sort_type,sort_is_big,0);
    for(uint16_t i=0;i<item_data.sorted_count;i++){
        if(time_is_equal(&(item_data.items[item_data.index_sorted[i]].produce_time),&(item->produce_time))){
            index_show_page_0=i/4;
            index_show_page_1=i;
            break;
        }
    }
    item_cache=item_data.items[item_data.index_sorted[index_show_page_1]];
    Icons_draw();
    Page_home_draw();
}
