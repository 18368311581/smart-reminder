#include "page_setting.h"
#include "page_home.h"
#include "page_entering.h"
#include "page_deleted.h"
#include "page_warning.h"
#include "page_other.h"
#define dark_time_per_20ms  50*40
#define sleep_time_per_20ms 50*200
FATFS sd_card_disk;
static uint32_t sleep_count=0;
static uint8_t sleep_flag=0;
static void system_fatal_error(const char* str){
    __disable_irq();
    Lcd_clear(0xFFFF);
    Lcd_draw_str(20,100,"FATAL ERROR",32,0xF800,1,0);
    Lcd_draw_str(20,160,(char*)str,24,0x0000,1,0);
    while(1){__NOP();}
}
void sleep_process(void){
    uint8_t touch_count=0;TIM2_Stop();Item_data_update();
    Setting_save_checkly();Item_data_save_checkly();Lcd_turn_off();
    while(1){
        if(Touch_try_now()){
            touch_count+=1;
            if(touch_count==2){
                Lcd_turn_on();TIM2_Start();Lcd_reset();return;
            }
        }
        else{touch_count=0;}
        sleep_delay_ms(750);
    }
}
void close_check(uint16_t x,uint16_t y){
    if(sys_close_button_get(x,y)){
         sleep_process();sleep_flag=0;sleep_count=0;
    }
}
static void chinese_fonts_check(){
    if(chinese_fonts_init()){Ui_message_box("Font error",1);chinese_fonts_rewrite();}
}
static void file_system_init(void){
    FRESULT res;BYTE workbuf[FF_MAX_SS];
    res=f_mount(&sd_card_disk,"0:",1);
    if(res==FR_OK){}
    else if(res==FR_NO_FILESYSTEM){
        Ui_message_box("filesys fail",1);
        Ui_message_box("start format",1);
        MKFS_PARM mkfs_opt={0};
        mkfs_opt.fmt=FM_FAT32,
        res=f_mkfs("0:",&mkfs_opt,workbuf,sizeof(workbuf));
        if(res!=FR_OK){
            Ui_message_box("format fail",1);
            system_fatal_error("FILE SYSTEM FAIL");
        }
        res=f_mount(&sd_card_disk,"0:",1);
        if(res!=FR_OK){
            Ui_message_box("format fail",1);
            system_fatal_error("FILE SYSTEM FAIL");
        }
        Ui_message_box("format success",1);
    }
    else{
        char str[50];
        sprintf(str,"mount fail:%02X",res);
        Ui_message_box(str,1);
        system_fatal_error("FILE SYSTEM FAIL");
    }
    res=f_mkdir("0:/smart_remd_stat");
    if(res!=FR_OK&&res!=FR_EXIST){
        char str[50];
        sprintf(str,"mkdir fail:%02X",res);
        Ui_message_box(str,1);
        system_fatal_error("FILE SYSTEM FAIL");
    }
}
void Start_up(void){
    HAL_Init();Clock_init();Lcd_init();Icon_show_main_stp();
    Camera_init();Ertc_init();Norflash_init();Sram_init();
    Gpioc_set_sd_card_mode();SD_card_init();Malloc_sram_init();
    file_system_init();Ths_init();Touch_init();Rng_init();
    chinese_fonts_check();TIM2_Init();TIM3_Init();Time_is_normal_check();
    Item_data_init();Setting_load();Item_data_load();TIM2_Start();
    Icons_init_set(4);Icons_draw();Page_warning_draw();
}
void Smart_remd_runtime_20ms(void){
    if(Touch_get()){
        sleep_count=0;if(sleep_flag){Lcd_reset();sleep_flag=0;}
        if(Icon_index_now==0&&Touch_get_y[0]>=704&&Touch_get_x[0]>=80){is_setting_show=0;}
        if(32<=Touch_get_y[0]&&Touch_get_y[0]<704){
            if(Icon_index_now==0){Page_setting_process(Touch_get_x[0],Touch_get_y[0]);}
            else if(Icon_index_now==1){Page_home_process(Touch_get_x[0],Touch_get_y[0]);}
            else if(Icon_index_now==2){Page_entering_process(Touch_get_x[0],Touch_get_y[0]);}
            else if(Icon_index_now==3){Page_deleted_process(Touch_get_x[0],Touch_get_y[0]);}
            else if(Icon_index_now==4){Page_warning_process(Touch_get_x[0],Touch_get_y[0]);}
            else if(Icon_index_now==5){Page_other_process(Touch_get_x[0],Touch_get_y[0]);close_check(Touch_get_x[0],Touch_get_y[0]);}
        }
        else if(Touch_get_y[0]>=704&&sys_count_icon==0&&Icons_touch_get(Touch_get_x[0],Touch_get_y[0])){
            if(Icon_index_now==0){is_setting_show=1;Page_setting_draw();}
            else if(Icon_index_now==1){Page_home_draw();}
            else if(Icon_index_now==2){Page_entering_draw();}
            else if(Icon_index_now==3){Page_deleted_draw();}
            else if(Icon_index_now==4){Page_warning_draw();}
            else if(Icon_index_now==5){Page_other_draw();}
        }
    }
    else{sleep_count+=1;}
    if(sleep_count>dark_time_per_20ms){
        if(sleep_flag==0){Lcd_set_back_light(5);sleep_flag=1;}
        else if(sleep_count>sleep_time_per_20ms){sleep_process();sleep_flag=0;sleep_count=0;}
    }
    if(sys_count_icon>0){sys_count_icon-=1;}
    Random_ui_task();
    if(second_signal){
        second_signal=0;
        SET_PRO_func();
        CLK_SHW_func();
    }
}
void Smart_remd_runtime_200ms(void){
    Ths_ui_task();
}
void Smart_remd_runtime_30s(void){
    Time_update();
}
void Smart_remd_runtime_120s(void){
    page_deleted_show_update();
    page_home_show_update();
    page_warning_show_update();
    Setting_save_checkly();
    Item_data_save_checkly();
}
int main(void){
    Start_up();int i_200ms=0,i_30s=0,i_120s=0;while(1){
        Smart_remd_runtime_20ms();
        if(i_200ms==10  ){i_200ms=0;Smart_remd_runtime_200ms();}
        if(i_30s  ==1500){i_30s=0;  Smart_remd_runtime_30s();  }
        if(i_120s ==6000){i_120s=0; Smart_remd_runtime_120s(); }
        i_200ms++;i_30s++;i_120s++;delay_ms(20);
    }
}
