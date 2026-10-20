#ifndef __DATA_BASE_H_
#define __DATA_BASE_H_
#include "app.h"
#define data_base_max_name_len 38
#define data_base_password 0x12463E47
extern const uint32_t data_base_address;
extern const uint32_t data_base_capacity;
__packed struct time_class{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t date;
    uint8_t month;
    uint8_t year;
};
__packed typedef struct{
    struct time_class produce_time;
    struct time_class bad_time;
    uint32_t save_pic_index;
    float number;
    uint8_t status;
    char name_str[data_base_max_name_len+1];
}Item_class;
__packed typedef struct{
    uint32_t magic;
    uint32_t version;
    uint32_t count;
    uint32_t crc;
}db_header_t;
__packed struct item_data_base{
    Item_class* items;
    uint16_t* index_sorted;
    int32_t* aux_value;
    uint16_t count;
    uint16_t is_sorted;
    uint16_t sorted_count;
    uint16_t capacity;
    char sort_name_str[data_base_max_name_len+1];
};
extern struct item_data_base item_data;
extern char search_str[data_base_max_name_len+1];
uint8_t time_is_equal(struct time_class *t1,struct time_class *t2);
int32_t time_to_seconds(struct time_class* t);
void Item_data_init(void);
void Item_data_load(void);
void Item_data_save(void);
void Item_data_save_checkly(void);
void Item_data_set_flag(void);
void Item_data_backup(void);
void Item_data_remove(uint16_t index);
void Item_data_add(Item_class* item);
void Item_data_delete(uint16_t index);
void Item_data_restore(uint16_t index);
void time_class_get_now(struct time_class* input);
void Item_data_update(void);
void Item_data_sort(uint8_t sort_mode,uint8_t is_big_first,uint8_t is_delete);
void Item_data_warning_sort(uint8_t sort_type,uint8_t is_big_first,uint8_t filter,struct time_class* now);
void time_diff_str(struct time_class* target,struct time_class* now,char *out);
uint16_t get_warning_bar_color(Item_class* p_item,struct time_class* now);
void time_diff_printf_warning(struct time_class* target,struct time_class* now,char *out);
#endif
