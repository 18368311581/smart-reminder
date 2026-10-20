#include "data_base.h"
struct item_data_base item_data;
char search_str[data_base_max_name_len+1];
static const uint32_t data_base_address_a=(15*1024+256)*1024;
static const uint32_t data_base_address_b=(15*1024+512)*1024;
static const char data_base_address_sd[]="0:/smart_remd_stat/stat.bin";
static const uint32_t data_base_capacity=1024;
static uint8_t g_sort_mode;
static uint8_t g_is_big_first;
static uint8_t save_check_flag;
int32_t time_to_seconds(struct time_class* t){
    int32_t rtv=0;
    for(uint16_t y=0;y<t->year;y++){rtv+=(is_leap(y)?366:365);}
    for(uint8_t m=1;m<t->month;m++){rtv+=get_month_days(t->year,m);}
    rtv+=(t->date-1);rtv*=24;rtv+=t->hour;
    rtv*=60;rtv+=t->minute;rtv*=60;rtv+=t->second;
    return rtv;
}
static int time_cmp(struct time_class *t1,struct time_class *t2){
    if(t1->year   != t2->year  )return t1->year   - t2->year;
    if(t1->month  != t2->month )return t1->month  - t2->month;
    if(t1->date   != t2->date  )return t1->date   - t2->date;
    if(t1->hour   != t2->hour  )return t1->hour   - t2->hour;
    if(t1->minute != t2->minute)return t1->minute - t2->minute;
    return t1->second - t2->second;
}
uint8_t time_is_equal(struct time_class *t1,struct time_class *t2){
    if(t1->year   != t2->year  )return 0;
    if(t1->month  != t2->month )return 0;
    if(t1->date   != t2->date  )return 0;
    if(t1->hour   != t2->hour  )return 0;
    if(t1->minute != t2->minute)return 0;
    return t1->second==t2->second;
}
static int32_t interval_calc(uint16_t index){
    int32_t return_bit=0;
    return_bit|=((item_data.items[index].bad_time.year   - item_data.items[index].produce_time.year  )&0x3F)<<25;
    return_bit|=((item_data.items[index].bad_time.month  - item_data.items[index].produce_time.month )&0x0F)<<21;
    return_bit|=((item_data.items[index].bad_time.date   - item_data.items[index].produce_time.date  )&0x1F)<<16;
    return_bit|=((item_data.items[index].bad_time.hour   - item_data.items[index].produce_time.hour  )&0x1F)<<11;
    return_bit|=((item_data.items[index].bad_time.minute - item_data.items[index].produce_time.minute)&0x3F)<<5;
    return_bit|=((item_data.items[index].bad_time.second - item_data.items[index].produce_time.second)/2)&0x1F;
    return return_bit;
}
static uint8_t str_distance(char *s1,char *s2){
    uint8_t len1=strlen(s1);
    uint8_t len2=strlen(s2);
    uint8_t dp[41];
    for(uint8_t j=0;j<=len2;j++){
        dp[j]=j;
    }
    for(uint8_t i=1;i<=len1;i++){
        uint8_t prev=dp[0];
        dp[0]=i;
        for(uint8_t j=1;j<=len2;j++){
            uint8_t temp=dp[j];
            int cost=(s1[i-1]==s2[j-1])?0:1;
            int a=dp[j]+1;
            int b=dp[j-1]+1;
            int c=prev+cost;
            int min=a<b?a:b;
            dp[j]=min<c?min:c;
            prev=temp;
        }
    }
    return dp[len2];
}
static int cmp_func(const void *a,const void *b){
    uint16_t ia=*(uint16_t*)a;
    uint16_t ib=*(uint16_t*)b;
    int ret=0;
    if(g_sort_mode==0){ret=time_cmp(&item_data.items[ia].produce_time,&item_data.items[ib].produce_time);}
    else if(g_sort_mode==1){ret=time_cmp(&item_data.items[ia].bad_time,&item_data.items[ib].bad_time);}
    else if(g_sort_mode==2||g_sort_mode==3){
        if(item_data.aux_value[ia]<item_data.aux_value[ib]){ret=-1;}
        else if(item_data.aux_value[ia]>item_data.aux_value[ib]){ret=1;}
        else{ret=0;}
    }
    if(g_is_big_first)ret=-ret;
    return ret;
}
void Item_data_init(void){
    uint32_t total_size=(sizeof(Item_class)+sizeof(uint16_t)+sizeof(int32_t))*data_base_capacity;
    uint8_t *mem=(uint8_t*)Malloc_sram_alloc(total_size);
    item_data.items=(Item_class*)mem;
    mem+=sizeof(Item_class)*data_base_capacity;
    item_data.index_sorted=(uint16_t*)mem;
    mem+=sizeof(uint16_t)*data_base_capacity;
    item_data.aux_value=(int32_t*)mem;
    item_data.count=0;
    item_data.is_sorted=0;
    item_data.sorted_count=0;
    item_data.capacity=data_base_capacity;
}
static uint32_t db_crc32(uint32_t crc,uint8_t *data,uint32_t len){
    if(len==0){return crc;}
    for(uint32_t i=0;i<len;i++){
        crc^=data[i];
        for(uint8_t j=0;j<8;j++){
            if(crc&1){
                crc=(crc>>1)^0xEDB88320;
            }
            else{
                crc>>=1;
            }
        }
    }
    return crc;
}
static uint8_t  current_area    = 0;
static uint32_t current_version = 0;
static void item_data_save_norflash(void){
    uint32_t addr=(current_area==0)?data_base_address_b:data_base_address_a;
    db_header_t header;
    header.magic   = data_base_password;
    header.version = current_version+1;
    header.count   = item_data.count;
    uint32_t data_size=item_data.count*sizeof(Item_class);
    header.crc=db_crc32(0xFFFFFFFF,(uint8_t*)item_data.items,data_size);
    Norflash_write((uint8_t*)item_data.items,addr+sizeof(db_header_t),data_size);
    Norflash_write((uint8_t*)&header,addr,sizeof(header));
    current_area^=1;
    current_version++;
}
static uint8_t item_data_load_norflash(uint32_t addr,db_header_t *out_header){
    db_header_t header;
    Norflash_read((uint8_t*)&header,addr,sizeof(header));
    if(header.magic!=data_base_password){return 1;}
    if(header.count>data_base_capacity){return 2;}
    uint32_t data_size=header.count*sizeof(Item_class);
    uint32_t crc=0xFFFFFFFF;uint8_t temp[64];
    uint32_t read_addr=addr+sizeof(header);uint32_t remain=data_size;
    while(remain){
        uint32_t chunk=(remain>sizeof(temp))?sizeof(temp):remain;
        Norflash_read(temp,read_addr,chunk);
        crc=db_crc32(crc,temp,chunk);
        read_addr+=chunk;
        remain-=chunk;
    }
    if(crc!=header.crc){return 3;}
    Norflash_read((uint8_t*)item_data.items,addr+sizeof(header),data_size);
    *out_header=header;
    return 0;
}
static void item_data_save_sd_card(void){
    FIL fp;UINT bw;
    if(f_open(&fp,data_base_address_sd,FA_CREATE_ALWAYS|FA_WRITE)!=FR_OK){return;}
    db_header_t header;
    header.magic   = data_base_password;
    header.version = current_version;
    header.count   = item_data.count;
    uint32_t size=item_data.count*sizeof(Item_class);
    header.crc=db_crc32(0xFFFFFFFF,(uint8_t*)item_data.items,size);
    f_write(&fp,&header,sizeof(header),&bw);
    f_write(&fp,item_data.items,size,&bw);
    f_close(&fp);
}
static uint8_t item_data_load_sd_card(void){
    FIL fp;UINT br;
    if(f_open(&fp,data_base_address_sd,FA_READ)!=FR_OK)return 1;
    db_header_t header;
    f_read(&fp,&header,sizeof(header),&br);
    if(header.magic!=data_base_password){f_close(&fp);return 2;}
    if(header.count>data_base_capacity){return 3;}
    uint32_t size=header.count*sizeof(Item_class);
    f_read(&fp,item_data.items,size,&br);
    uint32_t crc=db_crc32(0xFFFFFFFF,(uint8_t*)item_data.items,size);
    f_close(&fp);
    if(crc!=header.crc){return 4;}
    item_data.count=header.count;
    current_version=header.version;
    return 0;
}
void Item_data_load(void){
    db_header_t ha,hb;
    uint8_t ra=item_data_load_norflash(data_base_address_a,&ha);
    uint8_t rb=item_data_load_norflash(data_base_address_b,&hb);
    if(ra==0&&rb==0){
        if(ha.version>=hb.version){
            item_data_load_norflash(data_base_address_a,&ha);
            current_area=0;
            current_version=ha.version;
            item_data.count=ha.count;
        }
        else{
            item_data_load_norflash(data_base_address_b,&hb);
            current_area=1;
            current_version=hb.version;
            item_data.count=hb.count;
        }
        Item_data_update();
    }
    else if(ra==0){
        item_data_load_norflash(data_base_address_a,&ha);
        current_area=0;
        current_version=ha.version;
        item_data.count=ha.count;
        Item_data_update();
        item_data_save_norflash();
    }
    else if(rb==0){
        item_data_load_norflash(data_base_address_b,&hb);
        current_area=1;
        current_version=hb.version;
        item_data.count=hb.count;
        Item_data_update();
        item_data_save_norflash();
    }
    else{
        if(item_data_load_sd_card()==0){
            Ui_message_box("数据损坏,从SD恢复",1);
            current_area=0;
            Item_data_update();
            item_data_save_norflash();
            item_data_save_norflash();
        }
        else{
            Ui_message_box("数据损坏,已重置",1);
            item_data.count        = 0;
            current_version        = 0;
            current_area           = 0;
            item_data_save_sd_card();
            item_data_save_norflash();
            item_data_save_norflash();
        }
    }
}
void Item_data_save(void){
    Item_data_update();
    item_data_save_norflash();
}
void Item_data_backup(void){
    Item_data_update();
    item_data_save_norflash();
    item_data_save_sd_card();
}
void Item_data_remove(uint16_t index){
    save_check_flag=1;
    for(uint16_t i=index;i<item_data.count-1;i++){
        item_data.items[i]=item_data.items[i+1];
    }
    item_data.count-=1;
    item_data.is_sorted=0;
}
void Item_data_add(Item_class* item){
    if(item_data.count>=item_data.capacity){return;}
    save_check_flag=1;
    uint16_t pos=item_data.count;
    while(pos>0&&time_cmp(&item_data.items[pos-1].produce_time,&item->produce_time)>0){
        item_data.items[pos]=item_data.items[pos-1];
        pos-=1;
    }
    item_data.items[pos]=*item;
    item_data.count+=1;
    item_data.is_sorted=0;
}
void Item_data_delete(uint16_t index){
    if(index>=item_data.count)return;
    save_check_flag=1;
    item_data.is_sorted=0;
    if((item_data.items[index].status&0x02)>>1==1){
        Item_data_remove(index);
    }
    else{
        item_data.items[index].status|=0x02;
    }
}
void Item_data_restore(uint16_t index){
    if(index>=item_data.count)return;
    item_data.is_sorted=0;
    save_check_flag=1;
    item_data.items[index].status&=0xFD;
}
void time_class_get_now(struct time_class* input){
    input->second = Ertc_get_second();
    input->minute = Ertc_get_minute();
    input->hour   = Ertc_get_hour();
    input->date   = Ertc_get_day();
    input->month  = Ertc_get_month();
    input->year   = Ertc_get_year()-2000;
}
void Item_data_update(void){
    struct time_class now;
    time_class_get_now(&now);
    for(int i=0;i<item_data.count;i++){
        if(time_cmp(&item_data.items[i].bad_time,&now)<=0){
            item_data.items[i].status|=0x01;
        }
        else{
            item_data.items[i].status&=0xFE;
        }
    }
}
void Item_data_save_checkly(void){
    if(save_check_flag){
        save_check_flag=0;
        Ui_show_message("保存数据库…",250);
        item_data_save_norflash();
    }
}
void Item_data_set_flag(void){
    save_check_flag=1;
    item_data.is_sorted=0;
    item_data.sorted_count=0;
}
void Item_data_sort(uint8_t sort_mode,uint8_t is_big_first,uint8_t is_delete){
    item_data.is_sorted=1;
    g_sort_mode=sort_mode;
    g_is_big_first=is_big_first;
    uint16_t count=0;
    for(uint16_t i=0;i<item_data.count;i++){
        if((item_data.items[i].status&0x02)>>1==is_delete){
            item_data.index_sorted[count]=i;count+=1;
        }
    }
    item_data.sorted_count=count;
    if(item_data.sorted_count==0){return;}
    if(sort_mode==2){
        for(uint16_t i=0;i<count;i++){
            item_data.aux_value[item_data.index_sorted[i]]=interval_calc(item_data.index_sorted[i]);
        }
    }
    else if(sort_mode==3){
        for(uint16_t i=0;i<count;i++){
            item_data.aux_value[item_data.index_sorted[i]]=str_distance(item_data.items[item_data.index_sorted[i]].name_str,item_data.sort_name_str);
        }
    }
    qsort(item_data.index_sorted,item_data.sorted_count,sizeof(uint16_t),cmp_func);
}
void Item_data_warning_sort(uint8_t sort_type,uint8_t is_big_first,uint8_t filter,struct time_class* now){
    item_data.is_sorted=1;
    g_is_big_first=is_big_first;
    uint16_t count=0;
    for(uint16_t i=0;i<item_data.count;i++){
        if((item_data.items[i].status&0x02)>>1==0){
            item_data.index_sorted[count]=i;
            int cmp=time_cmp(&item_data.items[i].bad_time,now);
            uint8_t is_expired=(cmp<0);
            if(filter==1&&!is_expired){continue;}
            if(filter==2&&is_expired){continue;}
            count++;
        }
    }
    item_data.sorted_count=count;
    if(count==0){return;}
    int32_t now_sec=time_to_seconds(now);
    for(uint16_t i=0;i<count;i++){
        uint16_t idx     = item_data.index_sorted[i];
        int32_t bad_sec  = time_to_seconds(&item_data.items[idx].bad_time);
        int32_t prod_sec = time_to_seconds(&item_data.items[idx].produce_time);
        int32_t total    = bad_sec-prod_sec;
        int32_t remain   = bad_sec-now_sec;
        int32_t risk     = 0;
        if(remain<0){
            risk=0x7FFFFFFF-remain;
        }
        else{
            if(sort_type==0){
                risk=-remain;
            }
            else{
                if(total<=0){risk=0;}
                else{risk=((total-remain)*100000)/total;}
            }
        }
        item_data.aux_value[idx]=risk;
    }
    g_sort_mode=2;
    qsort(item_data.index_sorted,item_data.sorted_count,sizeof(uint16_t),cmp_func);
}
void time_diff_str(struct time_class* target,struct time_class* now,char *out){
    int cmp=time_cmp(target,now);
    struct time_class start,end;uint8_t is_future;
    if(cmp>=0){
        start=*now;end=*target;is_future=1;
    }
    else{
        start=*target;end=*now;is_future=0;
    }
    int32_t diff_sec=time_to_seconds(&end)-time_to_seconds(&start);
    if(diff_sec<120){
        if(is_future){
            sprintf(out,"马上过期            ");
        }
        else{
            sprintf(out,"刚刚过期            ");
        }
        return;
    }
    int year  = end.year  - start.year;
    int month = end.month - start.month;
    int day   = end.date  - start.date;
    int hour  = end.hour  - start.hour;
    int min   = end.minute- start.minute;
    int sec   = end.second- start.second;
    if(sec<0){sec+=60;min--;}
    if(min<0){min+=60;hour--;}
    if(hour<0){hour+=24;day--;}
    if(day<0){
        if(end.month==1){
            day+=get_month_days(end.year-1, 12);
        }
        else{
            day+=get_month_days(end.year,end.month-1);
        }
        month--;
    }
    if(month<0){month+=12;year--;}
    if(is_future){
        if(year>0){
            if(month>0){sprintf(out,"%2d年%2d个月后过期    ",year,month);}
            else{       sprintf(out,"%2d年后过期          ",year);}
        }else if(month>0){
            if(day>0){  sprintf(out,"%2d个月%2d天后过期    ",month,day );}
            else{       sprintf(out,"%2d个月后过期        ",month);}
        }else if(day>0){
            if(hour>0){ sprintf(out,"%2d天%2d小时后过期    ",day,hour);}
            else{       sprintf(out,"%2d天后过期          ",day);}
        }else if(hour>0){
            if(min>0){  sprintf(out,"%2d小时%2d分钟后过期  ",hour,min);}
            else{       sprintf(out,"%2d小时后过期        ",hour);}
        }
        else{           sprintf(out,"%2d分钟后过期        ",min);}
    }
    else{
        if(year>0){
            if(month>0){sprintf(out,"已过期%2d年%2d个月    ",year,month);}
            else{       sprintf(out,"已过期%2d年          ",year);}
        }else if(month>0){
            if(day>0){  sprintf(out,"已过期%2d个月%2d天    ",month,day );}
            else{       sprintf(out,"已过期%2d个月        ",month);}
        }else if(day>0){
            if(hour>0){ sprintf(out,"已过期%2d天%2d小时    ",day,hour);}
            else{       sprintf(out,"已过期%2d天          ",day);}
        }else if(hour>0){
            if(min>0){  sprintf(out,"已过期%2d小时%2d分钟  ",hour,min);}
            else{       sprintf(out,"已过期%2d小时        ",hour);}
        }
        else{           sprintf(out,"已过期%2d分钟        ",min);}
    }
    uint8_t len=0,write=0;
    while(out[len]){len++;}
    for(uint8_t read=0;read<len;read++){
        if(out[read]!=' '){
            out[write++]=out[read];
        }
    }
    while(write<len){out[write++]=' ';}
}
void time_diff_printf_warning(struct time_class* target,struct time_class* now,char *out){
    int cmp=time_cmp(target,now);
    struct time_class start,end;uint8_t is_future;
    if(cmp>=0){start=*now;end=*target;is_future=1;}
    else{start=*target;end=*now;is_future=0;}
    int32_t diff_sec=time_to_seconds(&end)-time_to_seconds(&start);
    if(diff_sec<120){
        if(is_future){sprintf(out,"马上过期  ");}
        else{sprintf(out,"刚刚过期  ");}
        return;
    }
    int year  = end.year   - start.year;
    int month = end.month  - start.month;
    int day   = end.date   - start.date;
    int hour  = end.hour   - start.hour;
    int min   = end.minute - start.minute;
    int sec   = end.second - start.second;
    if(sec<0){sec+=60;min--;}
    if(min<0){min+=60;hour--;}
    if(hour<0){hour+=24;day--;}
    if(day<0){
        if(end.month==1){day+=get_month_days(end.year-1, 12);}
        else{day+=get_month_days(end.year,end.month-1);}
        month--;
    }
    if(month<0){month+=12;year--;}
    if(is_future){
        if(year>0){sprintf(out,"剩余%4.1f年",year+((float)month)/13);}
        else if(month>0){sprintf(out,"剩余%4.1f月",month+((float)day)/32);}
        else if(day>0){sprintf(out,"剩余%4.1f天",day+((float)hour)/24);}
        else if(hour>0){sprintf(out,"剩余%4.1f时",hour+((float)min)/60);}
        else{sprintf(out,"剩余%2d分钟",min);}
    }
    else{
        if(year>0){sprintf(out,"过期%4.1f年",year+((float)month)/13);}
        else if(month>0){sprintf(out,"过期%4.1f月",month+((float)day)/32);}
        else if(day>0){sprintf(out,"过期%4.1f天",day+((float)hour)/24);}
        else if(hour>0){sprintf(out,"过期%4.1f时",hour+((float)min)/60);}
        else{sprintf(out,"过期%2d分钟",min);}
    }
}
uint16_t get_warning_bar_color(Item_class* p_item,struct time_class* now){
    int32_t now_sec  = time_to_seconds(now);
    int32_t bad_sec  = time_to_seconds(&p_item->bad_time);
    int32_t prod_sec = time_to_seconds(&p_item->produce_time);
    int32_t total    = bad_sec - prod_sec;
    int32_t used     = now_sec - prod_sec;
    if(total<=0){return 0xC618;}
    int32_t progress=(used*1000)/total;
    if(progress<600){return 0x3F46;}
    else if(progress<800){return 0xFFE0;}
    else if(progress<1000){return 0xFD20;}
    else if(progress<1200){return 0xF800;}
    else{return 0x8800;}
}
