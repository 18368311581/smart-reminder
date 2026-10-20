#include "page_warning.h"
static uint8_t sort_type=0;
static uint8_t sort_is_big=1;
static uint8_t sort_filter=0;
static uint8_t page_id=0;
static uint16_t index_show_page_0=0;
static uint16_t index_show_page_1=0;
static struct time_class now_t;
static const uint16_t color_button=0xBDFE;
static const uint16_t color_line_bg=0xCF59;
static const uint16_t color_bg=0x6657;
static const uint16_t gradient[140]={
    0x07E0,0x07E0,0x07E0,0x07E0,0x07E0,0x07E0,0x07E0,0x07E0,0x07E0,0x07E0,
    0x0FE0,0x0FE0,0x0FE0,0x0FE0,0x0FE0,0x17E0,0x17E0,0x17E0,0x17E0,0x1FE0,
    0x1FE0,0x1FE0,0x1FE0,0x27E0,0x27E0,0x27E0,0x27E0,0x2FE0,0x2FE0,0x2FE0,
    0x37E0,0x37E0,0x37E0,0x3FE0,0x3FE0,0x3FE0,0x47E0,0x47E0,0x47E0,0x4FE0,
    0x4FE0,0x4FE0,0x57E0,0x57E0,0x5FE0,0x5FE0,0x5FE0,0x67E0,0x67E0,0x67E0,
    0x6FE0,0x6FE0,0x77E0,0x77E0,0x7FE0,0x7FE0,0x7FE0,0x87E0,0x87E0,0x8FE0,
    0x8FE0,0x97E0,0x97E0,0x97E0,0x9FE0,0x9FE0,0xA7E0,0xA7E0,0xAFE0,0xAFE0,
    0xB7E0,0xB7E0,0xBFE0,0xBFE0,0xC7E0,0xC7E0,0xCFE0,0xCFE0,0xD7E0,0xD7E0,
    0xDFE0,0xDFE0,0xE7E0,0xE7E0,0xEFE0,0xEFE0,0xF7E0,0xF7E0,0xFFC0,0xFFA0,
    0xFF80,0xFF60,0xFF40,0xFF20,0xFEE0,0xFEC0,0xFEA0,0xFE80,0xFE60,0xFE40,
    0xFE20,0xFE00,0xFDC0,0xFDA0,0xFD80,0xFD60,0xFD40,0xFD00,0xFCE0,0xFCC0,
    0xFCA0,0xFC80,0xFC40,0xFC20,0xFC00,0xFBE0,0xFBA0,0xFB80,0xFB60,0xFB40,
    0xFB00,0xFAE0,0xFAC0,0xFAA0,0xFA60,0xFA40,0xFA20,0xFA00,0xF9C0,0xF9A0,
    0xF980,0xF940,0xF920,0xF900,0xF8C0,0xF8A0,0xF880,0xF840,0xF820,0xF800,
};
static const char sort_type_str[2][5]={"时间","比例"};
static const char sort_is_big_str[2][9]={"  升序  ","  降序  "};
static const char sort_filter_str[3][7]={" 全部 ","已过期","未过期"};
static struct Ui_button button_sort_type={
    .x=12,.y=62,.width=144,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="    "};
static struct Ui_button button_sort_is_big={
    .x=168,.y=62,.width=144,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="        "};
static struct Ui_button button_sort_filter={
    .x=324,.y=62,.width=144,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="      "};
static struct Ui_button button_pgup={
    .x=0,.y=96,.width=480,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="向上翻页"};
static struct Ui_button button_pgdn={
    .x=0,.y=672,.width=480,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="向下翻页"};
static struct Ui_button button_return={
    .x=10,.y=36,.width=120,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="返回"};
static struct Ui_button button_jump={
    .x=350,.y=36,.width=120,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="跳转"};
static void draw_progress_bar(uint16_t y,Item_class* p_item){
    Lcd_draw_rect(193,y-1,142,26,0x0000,0);
    int32_t now_sec  = time_to_seconds(&now_t);
    int32_t bad_sec  = time_to_seconds(&p_item->bad_time);
    int32_t prod_sec = time_to_seconds(&p_item->produce_time);
    int32_t total    = bad_sec-prod_sec;
    int32_t used     = now_sec-prod_sec;
    if(total<=0){return;}
    if(used>=total){Lcd_draw_rect(194,y,140,24,0xF800,1);return;}
    int32_t progress=(used*1000)/total;
    if(progress<0){progress=0;}
    uint16_t fill_w=(140*progress)/1000;
    for(uint16_t i=0;i<fill_w;i++){
        Lcd_draw_line(194+i,y,0,24,gradient[i]);
    }
}
static void draw_item_line(uint16_t y,uint16_t rindex){
    Lcd_draw_line(0,y,464,0,0x9574);
    Lcd_draw_line(0,y+31,464,0,0x9574);
    Lcd_draw_rect(1,y+1,462,30,color_line_bg,1);
    if(rindex<item_data.sorted_count){
        Item_class* p_item=&(item_data.items[item_data.index_sorted[rindex]]);
        Lcd_draw_rect(4,y+4,8,24,get_warning_bar_color(p_item,&now_t),1);
        char temp_s[50];strcpy(temp_s,p_item->name_str);
        str_cut_to_ellipsis_with_limit(temp_s,14);
        Lcd_draw_str_chinese(16,y+4,temp_s,24,0x0000,1,0);
        draw_progress_bar(y+4,p_item);
        time_diff_printf_warning(&p_item->bad_time,&now_t,temp_s);
        Lcd_draw_str_chinese(338,y+4,temp_s,24,0x0000,1,0);
    }
}
static void Ui_draw_warning_page_0_update(void){
    uint16_t y=128,index=index_show_page_0*17;char temp_s[50];
    for(uint8_t i=0;i<17;i++){
        if(index<item_data.sorted_count){
            Item_class* p_item=&(item_data.items[item_data.index_sorted[index]]);
            Lcd_draw_rect(4,y+4,8,24,get_warning_bar_color(p_item,&now_t),1);
            draw_progress_bar(y+4,p_item);
            time_diff_printf_warning(&p_item->bad_time,&now_t,temp_s);
            Lcd_draw_str_chinese(338,y+4,temp_s,24,0x0000,0,color_line_bg);
            index+=1;y+=32;
        }
        else{return;}
    }
}
static void Ui_draw_warning_page_0_draw_lines(void){
    uint16_t y=128,index=index_show_page_0*17;
    for(uint8_t i=0;i<17;i++){
        draw_item_line(y,index);
        index+=1;y+=32;
    }
    Lcd_draw_rect(462,128,18,544,0x0000,0);
    Lcd_draw_rect(463,129,16,542,0xFFFF,1);
    uint32_t total    = item_data.sorted_count/17;
    uint32_t used     = index_show_page_0;
    uint32_t progress = (used*1000)/total;
    uint32_t cent_y   = (500*progress)/1000+150;
    uint16_t y_low    = cent_y-21;
    uint16_t y_hgh    = cent_y+21;
    if(y_low<129){y_low=129;}
    if(y_hgh>671){y_hgh=671;}
    uint16_t delta_y=y_hgh-y_low;
    Lcd_draw_rect(462,y_low,18,delta_y,0x0000,1);
    Lcd_draw_rect(463,y_low+1,16,delta_y-2,0xA514,1);
}
static void draw_pic_single_big(void){
    Item_class* p_item=&(item_data.items[item_data.index_sorted[index_show_page_1]]);
    char temp_s[50];sprintf(temp_s,"0:/smart_remd_stat/%010u.jpg",p_item->save_pic_index);
    Lcd_draw_jpeg_pic(temp_s,0,224,480,480);
    Lcd_draw_str_chinese(6,75,p_item->name_str,24,0x0000,1,0);
    struct time_class* x=&(p_item->produce_time);
    sprintf(temp_s,"%04d-%02d-%02d %02d:%02d:%02d 生产",x->year+2000,x->month,x->date,x->hour,x->minute,x->second);//temp_s="2024-02-01 13:10:10 生产"
    Lcd_draw_str_chinese(6,105,temp_s,24,0x0000,1,0);
    struct time_class* y=&(p_item->bad_time);
    sprintf(temp_s,"%04d-%02d-%02d %02d:%02d:%02d 过期",y->year+2000,y->month,y->date,y->hour,y->minute,y->second);
    Lcd_draw_str_chinese(6,135,temp_s,24,0x0000,1,0);
    time_diff_str(&p_item->bad_time,&now_t,temp_s);
    Lcd_draw_str_chinese(6,165,temp_s,24,0x0000,1,0);
    sprintf(temp_s,"%.5f",p_item->number);
    float_string_refine(temp_s);
    Lcd_draw_str_chinese(6,195,"数量  ",24,0x0000,1,0);
    Lcd_draw_str_chinese(6+72,195,temp_s,24,0x0000,1,0);
}
void Page_warning_draw(void){
    page_lcd_clear(0xFFFF);
    if(page_id==0){
        time_class_get_now(&now_t);
        Lcd_draw_rect(0,32,480,64,color_bg,1);
        Lcd_draw_str_chinese(156,34,"过  期  警  告",24,0x0000,1,0);
        Item_data_warning_sort(sort_type,sort_is_big,sort_filter,&now_t);
        if(index_show_page_0*17>=item_data.sorted_count){
            if(item_data.sorted_count==0){
                index_show_page_0=0;
            }
            else{
                index_show_page_0=(item_data.sorted_count-1)/17;
            }
        }
        button_sort_type.text=(char*)sort_type_str[sort_type];
        button_sort_is_big.text=(char*)sort_is_big_str[sort_is_big];
        button_sort_filter.text=(char*)sort_filter_str[sort_filter];
        Ui_draw_button_init_draw(&button_sort_type);
        Ui_draw_button_init_draw(&button_sort_is_big);
        Ui_draw_button_init_draw(&button_sort_filter);
        Ui_draw_button_init_draw(&button_pgup);
        Ui_draw_warning_page_0_draw_lines();
        Ui_draw_button_init_draw(&button_pgdn);
    }
    else if(page_id==1){
        Lcd_draw_rect(0,32,480,192,color_bg,1);
        Ui_draw_button_init_draw(&button_return);
        Ui_draw_button_init_draw(&button_jump);
        draw_pic_single_big();
    }
}
void Page_warning_process(uint16_t x,uint16_t y){
    if(page_id==0){
        if(128<=y&&y<672&&sys_count_icon==0){
            uint8_t index_y=(y-128)/32;
            uint16_t index_cache=index_y+17*index_show_page_0;
            if(index_cache<item_data.sorted_count){
                index_show_page_1=index_cache;
                page_id=1;Icon_can_press=0;
                sys_count_icon=5;
                Page_warning_draw();
            }
        }
        else{
            if(Ui_draw_button_get_with_draw(&button_sort_filter,x,y)&&sys_count_icon==0){
                sort_filter=(sort_filter+1)%3;
                button_sort_filter.text=(char*)sort_filter_str[sort_filter];
                Ui_draw_button_text(&button_sort_filter);
                time_class_get_now(&now_t);
                Item_data_warning_sort(sort_type,sort_is_big,sort_filter,&now_t);
                Ui_draw_warning_page_0_draw_lines();
                sys_count_icon=5;return;
            }
            else if(Ui_draw_button_get_with_draw(&button_sort_type,x,y)&&sys_count_icon==0){
                sort_type=1-sort_type;
                button_sort_type.text=(char*)sort_type_str[sort_type];
                Ui_draw_button_text(&button_sort_type);
                time_class_get_now(&now_t);
                Item_data_warning_sort(sort_type,sort_is_big,sort_filter,&now_t);
                Ui_draw_warning_page_0_draw_lines();
                sys_count_icon=5;return;
            }
            else if(Ui_draw_button_get_with_draw(&button_sort_is_big,x,y)&&sys_count_icon==0){
                sort_is_big=1-sort_is_big;
                button_sort_is_big.text=(char*)sort_is_big_str[sort_is_big];
                Ui_draw_button_text(&button_sort_is_big);
                time_class_get_now(&now_t);
                Item_data_warning_sort(sort_type,sort_is_big,sort_filter,&now_t);
                Ui_draw_warning_page_0_draw_lines();
                sys_count_icon=5;return;
            }
            else if(Ui_draw_button_get_with_draw(&button_pgup,x,y)&&sys_count_icon==0){
                if(index_show_page_0>0){
                    index_show_page_0-=1;
                    time_class_get_now(&now_t);
                    Ui_draw_warning_page_0_draw_lines();
                }sys_count_icon=5;return;
            }
            else if(Ui_draw_button_get_with_draw(&button_pgdn,x,y)&&sys_count_icon==0){
                if(index_show_page_0*17+17<item_data.sorted_count){
                    index_show_page_0+=1;
                    time_class_get_now(&now_t);
                    Ui_draw_warning_page_0_draw_lines();
                }sys_count_icon=5;return;
            }
        }
    }
    else if(page_id==1){
        if(Ui_draw_button_get_with_draw(&button_return,x,y)&&sys_count_icon==0){
            page_id=0;Icon_can_press=1;
            sys_count_icon=5;Page_warning_draw();
        }
        else if(Ui_draw_button_get_with_draw(&button_jump,x,y)&&sys_count_icon==0){
            page_id=0;sys_count_icon=5;
            Item_class* p_item=&(item_data.items[item_data.index_sorted[index_show_page_1]]);
            page_warning_jump(p_item);
        }
    }
}
void page_warning_show_update(void){
    if(Icon_index_now==4){
        if(page_id==0){
            time_class_get_now(&now_t);
            Ui_draw_warning_page_0_update();
        }
        else if(page_id==1){
            time_class_get_now(&now_t);
            Item_class* p_item=&(item_data.items[item_data.index_sorted[index_show_page_1]]);
            char temp_s[50];time_diff_str(&p_item->bad_time,&now_t,temp_s);
            Lcd_draw_str_chinese(6,165,temp_s,24,0x0000,0,color_bg);
        }
    }
}
