#include "photo.h"
void Gpioc_set_camera_mode(void){
    GPIO_InitTypeDef gpio_init_struct;
    Camera_pwdn_off();
    gpio_init_struct.Pin       = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11;
    gpio_init_struct.Mode      = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull      = GPIO_PULLUP;
    gpio_init_struct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init_struct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOC,&gpio_init_struct);
}
void Gpioc_set_sd_card_mode(void){
    GPIO_InitTypeDef gpio_init_struct;
    Camera_pwdn_on();
    gpio_init_struct.Pin       = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11;
    gpio_init_struct.Mode      = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull      = GPIO_PULLUP;
    gpio_init_struct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init_struct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOC,&gpio_init_struct);
}
static const uint32_t cam_buf_size=15370;
static uint16_t cam_buf_a[cam_buf_size] __attribute__((aligned(4)));
static uint16_t cam_buf_b[cam_buf_size] __attribute__((aligned(4)));
static uint16_t lcd_chunk_lines;
static uint16_t lcd_start_x;
static uint16_t lcd_delta_x;
static uint16_t lcd_start_y;
static uint16_t lcd_delta_y;
static volatile uint16_t lcd_now_y_line;
static const uint32_t jpeg_buf_size=150*1024;
static const uint8_t jpeg_delay_frame=10;
static uint8_t* p_jpeg_data_buf;
static uint8_t g_ov_mode=0;
static volatile uint32_t g_jpeg_data_len;
static volatile uint32_t g_jpeg_scan_index;
static volatile uint8_t g_jpeg_frame_count;
static volatile uint8_t  g_jpeg_done;
static volatile uint8_t g_jpeg_capture_enable;
static uint32_t p_dcmi_line_buf_0[1024];
static uint32_t p_dcmi_line_buf_1[1024];
static void camera_dma_m0_cplt_callback(DMA_HandleTypeDef *hdma){
    int32_t lx=lcd_now_y_line-lcd_start_y+lcd_chunk_lines-lcd_delta_y;
    if(lx<0){Lcd_set_point_list(lcd_start_x,lcd_now_y_line,lcd_delta_x,lcd_chunk_lines,cam_buf_a);lcd_now_y_line+=lcd_chunk_lines;}
    else if(lx==0){Lcd_set_point_list(lcd_start_x,lcd_now_y_line,lcd_delta_x,lcd_chunk_lines,cam_buf_a);lcd_now_y_line=lcd_start_y;}
    else{Lcd_set_point_list(lcd_start_x,lcd_now_y_line,lcd_delta_x,lcd_chunk_lines-lx,cam_buf_a);
        uint16_t l=(lcd_chunk_lines-lx)*lcd_delta_x;lcd_now_y_line=lx+lcd_start_y;
        Lcd_set_point_list(lcd_start_x,lcd_start_y,lcd_delta_x,lx,cam_buf_a+l);}
}
static void camera_dma_m1_cplt_callback(DMA_HandleTypeDef *hdma){
    int32_t lx=lcd_now_y_line-lcd_start_y+lcd_chunk_lines-lcd_delta_y;
    if(lx<0){Lcd_set_point_list(lcd_start_x,lcd_now_y_line,lcd_delta_x,lcd_chunk_lines,cam_buf_b);lcd_now_y_line+=lcd_chunk_lines;}
    else if(lx==0){Lcd_set_point_list(lcd_start_x,lcd_now_y_line,lcd_delta_x,lcd_chunk_lines,cam_buf_b);lcd_now_y_line=lcd_start_y;}
    else{Lcd_set_point_list(lcd_start_x,lcd_now_y_line,lcd_delta_x,lcd_chunk_lines-lx,cam_buf_b);
        uint16_t l=(lcd_chunk_lines-lx)*lcd_delta_x;lcd_now_y_line=lx+lcd_start_y;
        Lcd_set_point_list(lcd_start_x,lcd_start_y,lcd_delta_x,lx,cam_buf_b+l);}
}
static void camera_dma_error_cplt_callback(DMA_HandleTypeDef *hdma){}
void DMA2_Stream1_IRQHandler(void){
    HAL_DMA_IRQHandler(&camera_dma_dcmi_handle);
}
void DCMI_IRQHandler(void){
    HAL_DCMI_IRQHandler(&camera_dcmi_handle);
}
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi){
    __HAL_DCMI_CLEAR_FLAG(&camera_dcmi_handle,DCMI_FLAG_FRAMERI);
    __HAL_DCMI_ENABLE_IT(&camera_dcmi_handle,DCMI_IT_FRAME);
    if(g_ov_mode){
        g_jpeg_frame_count+=1;
        if(g_jpeg_frame_count==jpeg_delay_frame){
            g_jpeg_data_len=0;
            g_jpeg_scan_index=0;
            g_jpeg_done=0;
            memset(p_dcmi_line_buf_0,0,4096);
            memset(p_dcmi_line_buf_1,0,4096);
            g_jpeg_capture_enable=1;
        }
    }
}
static void camera_jpeg_check_end(void){
    while(g_jpeg_scan_index+1<g_jpeg_data_len){
        if(p_jpeg_data_buf[g_jpeg_scan_index  ]==0xFF&&
           p_jpeg_data_buf[g_jpeg_scan_index+1]==0xD9){
            g_jpeg_data_len=g_jpeg_scan_index+2;
            g_jpeg_done=1;
            return;
        }
        g_jpeg_scan_index++;
    }
}
static void camera_jpeg_dma_m0_cplt_callback(DMA_HandleTypeDef *hdma){
    if(g_ov_mode&&g_jpeg_capture_enable&&(!g_jpeg_done)){
        memcpy(p_jpeg_data_buf+g_jpeg_data_len,p_dcmi_line_buf_0,4096);
        g_jpeg_data_len+=4096;
        camera_jpeg_check_end();
    }
}
static void camera_jpeg_dma_m1_cplt_callback(DMA_HandleTypeDef *hdma){
    if(g_ov_mode&&g_jpeg_capture_enable&&(!g_jpeg_done)){
        memcpy(p_jpeg_data_buf+g_jpeg_data_len,p_dcmi_line_buf_1,4096);
        g_jpeg_data_len+=4096;
        camera_jpeg_check_end();
    }
}
static void camera_jpeg_dma_error_cplt_callback(DMA_HandleTypeDef *hdma){g_jpeg_done=1;}
static void camera_save_jpeg_size(const TCHAR* file_path,uint16_t offx,uint16_t offy,uint16_t width,uint16_t height,uint16_t x,uint16_t y){
    g_ov_mode=1;
    FIL* f_jpg=(FIL*)Malloc_sram_alloc(sizeof(FIL));
    uint8_t* databuf=(uint8_t*)Malloc_sram_alloc(4096);
    p_jpeg_data_buf=Malloc_sram_alloc(jpeg_buf_size);
    Gpioc_set_camera_mode();
    Camera_set_jpeg_mode();
    Camera_set_flash_on();
    HAL_NVIC_SetPriority(DCMI_IRQn,2,2);
    HAL_NVIC_EnableIRQ(DCMI_IRQn);
    DCMI->IER=0x0;
    __HAL_DCMI_ENABLE_IT(&camera_dcmi_handle,DCMI_IT_FRAME);
    __HAL_DCMI_ENABLE(&camera_dcmi_handle);
    __HAL_LINKDMA(&camera_dcmi_handle,DMA_Handle,camera_dma_dcmi_handle);
    __HAL_DMA_DISABLE_IT(&camera_dma_dcmi_handle,DMA_IT_TC);
    camera_dma_dcmi_handle.Instance                 = DMA2_Stream1;
    camera_dma_dcmi_handle.Init.Channel             = DMA_CHANNEL_1;
    camera_dma_dcmi_handle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    camera_dma_dcmi_handle.Init.PeriphInc           = DMA_PINC_DISABLE;
    camera_dma_dcmi_handle.Init.MemInc              = DMA_MINC_ENABLE;
    camera_dma_dcmi_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    camera_dma_dcmi_handle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    camera_dma_dcmi_handle.Init.Mode                = DMA_CIRCULAR;
    camera_dma_dcmi_handle.Init.Priority            = DMA_PRIORITY_HIGH;
    camera_dma_dcmi_handle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
    camera_dma_dcmi_handle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_HALFFULL;
    camera_dma_dcmi_handle.Init.MemBurst            = DMA_MBURST_SINGLE;
    camera_dma_dcmi_handle.Init.PeriphBurst         = DMA_PBURST_SINGLE;
    HAL_DMA_DeInit(&camera_dma_dcmi_handle);
    HAL_DMA_Init(&camera_dma_dcmi_handle);
    __HAL_UNLOCK(&camera_dma_dcmi_handle);
    HAL_DMA_RegisterCallback(&camera_dma_dcmi_handle,HAL_DMA_XFER_CPLT_CB_ID,camera_jpeg_dma_m0_cplt_callback);
    HAL_DMA_RegisterCallback(&camera_dma_dcmi_handle,HAL_DMA_XFER_M1CPLT_CB_ID,camera_jpeg_dma_m1_cplt_callback);
    HAL_DMA_RegisterCallback(&camera_dma_dcmi_handle,HAL_DMA_XFER_ERROR_CB_ID,camera_jpeg_dma_error_cplt_callback);
    HAL_NVIC_SetPriority(DMA2_Stream1_IRQn,2,3);
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
    HAL_DMAEx_MultiBufferStart_IT(&camera_dma_dcmi_handle,(uint32_t)&DCMI->DR,(uint32_t)p_dcmi_line_buf_0,(uint32_t)p_dcmi_line_buf_1,1024);
    Camera_set_image_window(offx,offy,width,height);delay_ms(1);
    Camera_set_output_size(x,y);delay_ms(1);
    g_jpeg_done=0;g_jpeg_frame_count=0;
    g_jpeg_capture_enable=0;
    DCMI->CR|=DCMI_CR_CAPTURE;
    while(1){if(g_jpeg_done){break;}}
    DCMI->CR&=~DCMI_CR_CAPTURE;
    while(DCMI->CR&DCMI_CR_CAPTURE){__NOP();}
    __HAL_DCMI_DISABLE(&camera_dcmi_handle);
    __HAL_DCMI_DISABLE_IT(&camera_dcmi_handle,DCMI_IT_FRAME);
    __HAL_DMA_DISABLE(&camera_dma_dcmi_handle);
    while(camera_dma_dcmi_handle.Instance->CR&DMA_SxCR_EN){__NOP();}
    HAL_DMA_Abort(&camera_dma_dcmi_handle);
    __HAL_DMA_DISABLE_IT(&camera_dma_dcmi_handle,DMA_IT_TC|DMA_IT_HT|DMA_IT_TE|DMA_IT_DME|DMA_IT_FE);
    __HAL_DMA_CLEAR_FLAG(&camera_dma_dcmi_handle,DMA_FLAG_TCIF1_5|DMA_FLAG_HTIF1_5|DMA_FLAG_TEIF1_5|DMA_FLAG_DMEIF1_5|DMA_FLAG_FEIF1_5);
    g_ov_mode=0;Camera_set_flash_off();Gpioc_set_sd_card_mode();
    uint32_t remain=g_jpeg_data_len,src_off=0;
    uint16_t datasize=0;UINT bwr=0;
    if(f_open(f_jpg,file_path,FA_WRITE|FA_CREATE_NEW)==FR_OK){
        while(remain>0){
            datasize=(remain>4096)?4096:(uint16_t)remain;
            memcpy(databuf,p_jpeg_data_buf+src_off,datasize);
            if(f_write(f_jpg,databuf,datasize,&bwr)!=FR_OK){break;}
            src_off+=datasize;remain-=datasize;
        }
        f_close(f_jpg);
    }
    Malloc_sram_free(f_jpg);
    Malloc_sram_free(p_jpeg_data_buf);
    Malloc_sram_free(databuf);
}
void Camera_draw_lcd_start(uint16_t x,uint16_t y,uint16_t dx,uint16_t dy){
    Gpioc_set_camera_mode();
    Camera_set_rgb_mode();
    uint16_t crop_x=0,crop_y=0,crop_w=1600,crop_h=1200;
    int32_t k=(int32_t)dx*1200-(int32_t)dy*1600;
    if(k>0){crop_h=((uint32_t)1600*dy)/dx;crop_y=(1200-crop_h)/2;}
    else if(k<0){crop_w=((uint32_t)1200*dx)/dy;crop_x=(1600-crop_w)/2;}
    Camera_set_image_window(crop_x,crop_y,crop_w,crop_h);
    Camera_set_output_size(dx,dy);
    lcd_chunk_lines=cam_buf_size/dx;
    lcd_start_x=x;
    lcd_delta_x=dx;
    lcd_start_y=y;
    lcd_delta_y=dy;
    lcd_now_y_line=y;
    lcd_now_y_line=y;
    if((lcd_chunk_lines%2==1)&&(dx%2==1)){lcd_chunk_lines-=1;}
    __HAL_DCMI_ENABLE(&camera_dcmi_handle);
    __HAL_LINKDMA(&camera_dcmi_handle,DMA_Handle,camera_dma_dcmi_handle);
    __HAL_DMA_DISABLE_IT(&camera_dma_dcmi_handle,DMA_IT_TC);
    camera_dma_dcmi_handle.Instance                 = DMA2_Stream1;
    camera_dma_dcmi_handle.Init.Channel             = DMA_CHANNEL_1;
    camera_dma_dcmi_handle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    camera_dma_dcmi_handle.Init.PeriphInc           = DMA_PINC_DISABLE;
    camera_dma_dcmi_handle.Init.MemInc              = DMA_MINC_ENABLE;
    camera_dma_dcmi_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    camera_dma_dcmi_handle.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    camera_dma_dcmi_handle.Init.Mode                = DMA_CIRCULAR;
    camera_dma_dcmi_handle.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
    camera_dma_dcmi_handle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
    camera_dma_dcmi_handle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    camera_dma_dcmi_handle.Init.MemBurst            = DMA_MBURST_SINGLE;
    camera_dma_dcmi_handle.Init.PeriphBurst         = DMA_PBURST_SINGLE;
    HAL_DMA_DeInit(&camera_dma_dcmi_handle);
    HAL_DMA_Init(&camera_dma_dcmi_handle);
    HAL_DMA_RegisterCallback(&camera_dma_dcmi_handle,HAL_DMA_XFER_CPLT_CB_ID  ,camera_dma_m0_cplt_callback);
    HAL_DMA_RegisterCallback(&camera_dma_dcmi_handle,HAL_DMA_XFER_M1CPLT_CB_ID,camera_dma_m1_cplt_callback);
    HAL_DMA_RegisterCallback(&camera_dma_dcmi_handle,HAL_DMA_XFER_ERROR_CB_ID ,camera_dma_error_cplt_callback);
    HAL_NVIC_SetPriority(DMA2_Stream1_IRQn,2,3);
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
    HAL_DMAEx_MultiBufferStart_IT(&camera_dma_dcmi_handle,(uint32_t)&DCMI->DR,(uint32_t)cam_buf_a,(uint32_t)cam_buf_b,lcd_chunk_lines*dx/2);
    delay_ms(1);
    DCMI->CR|=DCMI_CR_CAPTURE;
}
void Camera_draw_lcd_stop(void){
    DCMI->CR&=~(DCMI_CR_CAPTURE);
    while(DCMI->CR&DCMI_CR_CAPTURE){__NOP();}
    __HAL_DCMI_DISABLE(&camera_dcmi_handle);
    __HAL_DMA_DISABLE(&camera_dma_dcmi_handle);
    while(camera_dma_dcmi_handle.Instance->CR&DMA_SxCR_EN){}
    HAL_DMA_Abort(&camera_dma_dcmi_handle);
    __HAL_DMA_DISABLE_IT(&camera_dma_dcmi_handle,DMA_IT_TC|DMA_IT_HT|DMA_IT_TE|DMA_IT_DME|DMA_IT_FE);
    __HAL_DMA_CLEAR_FLAG(&camera_dma_dcmi_handle,DMA_FLAG_TCIF1_5|DMA_FLAG_HTIF1_5|DMA_FLAG_TEIF1_5|DMA_FLAG_DMEIF1_5|DMA_FLAG_FEIF1_5);
    Camera_set_flash_off();
    Gpioc_set_sd_card_mode();
    Lcd_set_window(0,0,480,800);
}
void Camera_photo_save_jpeg(uint32_t pic_name){
    char path[50];sprintf(path,"0:/smart_remd_stat/%010u.jpg",pic_name);
    camera_save_jpeg_size(path,200,0,1200,1200,480,480);
    Camera_set_rgb_mode();
}
