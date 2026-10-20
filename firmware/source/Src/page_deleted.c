#include "page_deleted.h"
static uint8_t sort_type=0;
static uint8_t sort_is_big=1;
static char sort_str[data_base_max_name_len+1];
static uint8_t page_id=0;
static uint16_t index_show_page_0=0;
static uint16_t index_show_page_1=0;
static struct time_class now_t;
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
static struct Ui_button button_restore[4]={
   {.x=  2,.y=398,.width=60,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="恢复"},
   {.x=242,.y=398,.width=60,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="恢复"},
   {.x=  2,.y=638,.width=60,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="恢复"},
   {.x=242,.y=638,.width=60,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="恢复"}};
static struct Ui_button button_delete[4]={
   {.x=178,.y=398,.width=60,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="删除"},
   {.x=418,.y=398,.width=60,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="删除"},
   {.x=178,.y=638,.width=60,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="删除"},
   {.x=418,.y=638,.width=60,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="删除"}};
static struct Ui_button button_big={
    .x=10,.y=76,.width=104,.height=40,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="        "};
static struct Ui_button button_delete_all={
    .x=390,.y=34,.width=88,.height=24,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=16,.is_pressed=0,.text="清空回收站"};
static struct Ui_button button_return={
    .x=10,.y=36,.width=120,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="<-- 返回"};
static struct Ui_button button_restore_1={
    .x=180,.y=36,.width=120,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="恢复至库"};
static struct Ui_button button_delete_1={
    .x=350,.y=36,.width=120,.height=32,.border_width=3,
    .color_light=Lcd_color_white,.color_dark=Lcd_color_gray,
    .color_ground=color_button,.color_text=0x0000,
    .text_size=24,.is_pressed=0,.text="永久删除"};
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
static void draw_pic_single_big(void){
    Item_class* p_item=&(item_data.items[item_data.index_sorted[index_show_page_1]]);
    char temp_s[50];sprintf(temp_s,"0:/smart_remd_stat/%010u.jpg",p_item->save_pic_index);
    Lcd_draw_jpeg_pic(temp_s,0,224,480,480);
    Lcd_draw_str_chinese(6,75,p_item->name_str,24,0x0000,1,0);
    struct time_class* x=&(p_item->produce_time);
    sprintf(temp_s,"%04d-%02d-%02d %02d:%02d:%02d 生产",x->year+2000,x->month,x->date,x->hour,x->minute,x->second);
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
static void draw_pics_i(uint8_t i){
    if(i==0){
        if(index_show_page_0*4>=item_data.sorted_count){return;}
        draw_pic_single(0,192,index_show_page_0*4);
        Ui_draw_button_init_draw(&button_restore[0]);
        Ui_draw_button_init_draw(&button_delete[0]);
    }
    else if(i==1){
        if(index_show_page_0*4+1>=item_data.sorted_count){return;}
        draw_pic_single(240,192,index_show_page_0*4+1);
        Ui_draw_button_init_draw(&button_restore[1]);
        Ui_draw_button_init_draw(&button_delete[1]);
    }
    else if(i==2){
        if(index_show_page_0*4+2>=item_data.sorted_count){return;}
        draw_pic_single(0,432,index_show_page_0*4+2);
        Ui_draw_button_init_draw(&button_restore[2]);
        Ui_draw_button_init_draw(&button_delete[2]);
    }
    else if(i==3){
        if(index_show_page_0*4+3>=item_data.sorted_count){return;}
        draw_pic_single(240,432,index_show_page_0*4+3);
        Ui_draw_button_init_draw(&button_restore[3]);
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
    uint8_t index_get=5;
    for(uint8_t i=0;i<4;i++){
        if(x_types[2*i]<=x&&x<=x_types[2*i+1]){
            index_get=i;break;
        }
    }
    if(index_get<5&&index_get!=sort_type){
        Lcd_draw_rect(x_types[index_get*2]-5,76,x_types[index_get*2+1]-x_types[index_get*2]+10,40,color_fr,1);
        Lcd_draw_str_chinese(x_types[index_get*2],84,(char*)x_types_str[index_get],24,0xFFFF,1,0);
        Lcd_draw_rect(x_types[sort_type*2]-5,76,x_types[sort_type*2+1]-x_types[sort_type*2]+10,40,color_bg,1);
        Lcd_draw_str_chinese(x_types[sort_type*2],84,(char*)x_types_str[sort_type],24,0x0000,1,0);
        sort_type=index_get;
        Item_data_sort(sort_type,sort_is_big,1);
        delay_ms(100);
        Lcd_draw_rect(0,192,480,480,0xFFFF,1);
        time_class_get_now(&now_t);
        draw_pics_i(0);draw_pics_i(1);draw_pics_i(2);draw_pics_i(3);
        sys_count_icon=5;
    }
}
void Page_deleted_draw(void){
    page_lcd_clear(0xFFFF);
    if(page_id==0){
        Lcd_draw_rect(0,32,480,128,color_bg,1);
        Lcd_draw_str_chinese(192,34,"回 收 站",24,0x0000,1,0);
        x_types_draw();
        Item_data_sort(sort_type,sort_is_big,1);
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
        Ui_draw_button_init_draw(&button_delete_all);
        Lcd_draw_rect(8,120,464,32,0x0000,0);
        Lcd_draw_rect(9,121,462,30,0xFFFF,1);
        if(strlen(sort_str)==0){
            Lcd_draw_str_chinese(12,124,"请在此输入搜索名称",24,Lcd_color_gray,1,0);
        }else{
            Lcd_draw_str_chinese(12,124,sort_str,24,0x0000,1,0);
        }
        Ui_draw_button_init_draw(&button_pgup);
        time_class_get_now(&now_t);
        draw_pics_i(0);draw_pics_i(1);draw_pics_i(2);draw_pics_i(3);
        Ui_draw_button_init_draw(&button_pgdn);
    }
    else if(page_id==1){
        Lcd_draw_rect(0,32,480,192,color_bg,1);
        Ui_draw_button_init_draw(&button_return);
        Ui_draw_button_init_draw(&button_restore_1);
        Ui_draw_button_init_draw(&button_delete_1);
        draw_pic_single_big();
    }
}
void Page_deleted_process(uint16_t x,uint16_t y){
    if(page_id==0){
        if(y>76&y<=116){
            if(Ui_draw_button_get_with_draw(&button_big,x,y)&&sys_count_icon==0){
                sort_is_big=1-sort_is_big;
                button_big.text=(char*)big_types_str[sort_is_big];
                Ui_draw_button_text(&button_big);
                Item_data_sort(sort_type,sort_is_big,1);
                Lcd_draw_rect(0,192,480,480,0xFFFF,1);
                time_class_get_now(&now_t);
                draw_pics_i(0);draw_pics_i(1);draw_pics_i(2);draw_pics_i(3);
                sys_count_icon=5;return;
            }
            sort_type_touch_get_with_draw(x,y);return;
        }
        else if((9<=x&&x<471&&121<=y&&y<151)){
            Lcd_draw_rect(9,121,462,30,0xFFFF,1);
            Key_board(sort_str,12,124,24,data_base_max_name_len-2,1);sys_count_icon=5;
            if(strlen(sort_str)==0){
                Lcd_draw_str_chinese(12,124,"请在此输入搜索名称",24,Lcd_color_gray,1,0);
            }
            strcpy(item_data.sort_name_str,sort_str);
            draw_pics_i(2);draw_pics_i(3);
            Ui_draw_button_init_draw(&button_pgdn);
            Icons_draw();return;
        }
        else if(Ui_draw_button_get_with_draw(&button_delete_all,x,y)){
            if(Ui_message_box("确认全部删除?",0)&&Ui_message_box("将清空回收站?",0)){
                uint16_t write=0;uint32_t x;char path[50];
                for(uint16_t read=0;read<item_data.count;read++){
                    if((item_data.items[read].status&0x02)>>1==1){
                        if(write!=read){
                            x=item_data.items[write].save_pic_index;
                            sprintf(path,"0:/smart_remd_stat/%010u.jpg",x);f_unlink(path);
                            item_data.items[write]=item_data.items[read];
                        }
                        write++;
                    }
                }
                item_data.count=write;
                item_data.is_sorted=0;
                item_data.sorted_count=0;
                Lcd_draw_rect(0,192,480,480,0xFFFF,1);
                return;
            }
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
            if(index_show_page_0*4+1<item_data.sorted_count){
                index_show_page_0+=1;
                Lcd_draw_rect(0,192,480,480,0xFFFF,1);
                time_class_get_now(&now_t);
                draw_pics_i(0);draw_pics_i(1);draw_pics_i(2);draw_pics_i(3);
            }sys_count_icon=5;return;
        }
        else if(192<=y&&y<672&&sys_count_icon==0){
            for(uint8_t i=0;i<4;i++){
                if(Ui_draw_button_get_with_draw(&button_restore[i],x,y)){
                    Item_data_restore(item_data.index_sorted[index_show_page_0*4+i]);
                    Item_data_sort(sort_type,sort_is_big,1);
                    if(item_data.sorted_count%4==0&&item_data.sorted_count>0){index_show_page_0-=1;}
                    Lcd_draw_rect(0,192,480,480,0xFFFF,1);
                    time_class_get_now(&now_t);
                    draw_pics_i(0);draw_pics_i(1);draw_pics_i(2);draw_pics_i(3);
                    sys_count_icon=5;
                    return;
                }
                if(Ui_draw_button_get_with_draw(&button_delete[i],x,y)){
                    if(Ui_message_box("确认永久删除?",0)){
                        uint32_t x=item_data.items[item_data.index_sorted[index_show_page_0*4+i]].save_pic_index;
                        char path[50];sprintf(path,"0:/smart_remd_stat/%010u.jpg",x);f_unlink(path);
                        Item_data_delete(item_data.index_sorted[index_show_page_0*4+i]);
                        Item_data_sort(sort_type,sort_is_big,1);
                        if(item_data.sorted_count%4==0&&item_data.sorted_count>0){index_show_page_0-=1;}
                        Lcd_draw_rect(0,192,480,480,0xFFFF,1);
                        time_class_get_now(&now_t);
                        draw_pics_i(0);draw_pics_i(1);draw_pics_i(2);draw_pics_i(3);
                    }
                    sys_count_icon=5;
                    return;
                }
            }
            uint8_t i=(x>=240?1:0)+(y>=432?2:0);
            if(i+index_show_page_0*4>=item_data.sorted_count){return;}
            page_id=1;index_show_page_1=i+index_show_page_0*4;
            Page_deleted_draw();Icon_can_press=0;
        }
    }
    else if(page_id==1){
        if(y<40||y>72){return;}
        if(Ui_draw_button_get_with_draw(&button_return,x,y)){
            page_id=0;Icon_can_press=1;
            Page_deleted_draw();return;
        }
        else if(Ui_draw_button_get_with_draw(&button_delete_1,x,y)){
            if(Ui_message_box("确认永久删除?",0)){
                page_id=0;uint32_t x=item_data.items[item_data.index_sorted[index_show_page_1]].save_pic_index;
                char path[50];sprintf(path,"0:/smart_remd_stat/%010u.jpg",x);f_unlink(path);
                Item_data_remove(item_data.index_sorted[index_show_page_1]);
                if(item_data.sorted_count%4==0){index_show_page_0-=1;}
                Page_deleted_draw();Icon_can_press=1;
            }
        }
        else if(Ui_draw_button_get_with_draw(&button_restore_1,x,y)){
            page_id=0;
            Item_data_restore(item_data.index_sorted[index_show_page_1]);
            if(item_data.sorted_count%4==0&&item_data.sorted_count>0){index_show_page_0-=1;}
            Page_deleted_draw();Icon_can_press=1;
        }
    }
}
static void update_draw(uint16_t x,uint16_t y,uint16_t rindex){
    Item_class* p_item=&(item_data.items[item_data.index_sorted[rindex]]);
    char temp_s[50];time_diff_str(&p_item->bad_time,&now_t,temp_s);
    Lcd_draw_str_chinese(x+1,y+1,temp_s,16,0x0000,0,0xFFFF);
}
void page_deleted_show_update(void){
    if(Icon_index_now==3){
        if(page_id==0){
            time_class_get_now(&now_t);
            if(index_show_page_0*4<item_data.sorted_count){update_draw(0,192,index_show_page_0*4);}
            if(index_show_page_0*4+1<item_data.sorted_count){update_draw(240,192,index_show_page_0*4+1);}
            if(index_show_page_0*4+2<item_data.sorted_count){update_draw(0,432,index_show_page_0*4+2);}
            if(index_show_page_0*4+3<item_data.sorted_count){update_draw(240,432,index_show_page_0*4+3);}
        }
        else{
            time_class_get_now(&now_t);
            Item_class* p_item=&(item_data.items[item_data.index_sorted[index_show_page_1]]);
            char temp_s[50];time_diff_str(&p_item->bad_time,&now_t,temp_s);
            Lcd_draw_str_chinese(6,165,temp_s,24,color_fr,0,color_bg);
        }
    }
}
