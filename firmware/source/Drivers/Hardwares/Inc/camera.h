#ifndef __CAMERA_H_
#define __CAMERA_H_
#include "basic.h"
#define Camera_SCL_GPIO_Port               GPIOD
#define Camera_SCL_GPIO_Pin                GPIO_PIN_6
#define Camera_SDA_GPIO_Port               GPIOD
#define Camera_SDA_GPIO_Pin                GPIO_PIN_7
#define Camera_PWDN_GPIO_Port              GPIOG
#define Camera_PWDN_GPIO_Pin               GPIO_PIN_9
#define Camera_RESET_GPIO_Port             GPIOG
#define Camera_RESET_GPIO_Pin              GPIO_PIN_15
#define Camera_VSYNC_GPIO_Port             GPIOB
#define Camera_VSYNC_GPIO_Pin              GPIO_PIN_7
#define Camera_HREF_GPIO_Port              GPIOA
#define Camera_HREF_GPIO_Pin               GPIO_PIN_4
#define Camera_PCLK_GPIO_Port              GPIOA
#define Camera_PCLK_GPIO_Pin               GPIO_PIN_6
#define Camera_D0_GPIO_Port                GPIOC
#define Camera_D0_GPIO_Pin                 GPIO_PIN_6
#define Camera_D1_GPIO_Port                GPIOC
#define Camera_D1_GPIO_Pin                 GPIO_PIN_7
#define Camera_D2_GPIO_Port                GPIOC
#define Camera_D2_GPIO_Pin                 GPIO_PIN_8
#define Camera_D3_GPIO_Port                GPIOC
#define Camera_D3_GPIO_Pin                 GPIO_PIN_9
#define Camera_D4_GPIO_Port                GPIOC
#define Camera_D4_GPIO_Pin                 GPIO_PIN_11
#define Camera_D5_GPIO_Port                GPIOB
#define Camera_D5_GPIO_Pin                 GPIO_PIN_6
#define Camera_D6_GPIO_Port                GPIOE
#define Camera_D6_GPIO_Pin                 GPIO_PIN_5
#define Camera_D7_GPIO_Port                GPIOE
#define Camera_D7_GPIO_Pin                 GPIO_PIN_6
#define Camera_FLASH_GPIO_Port             GPIOA
#define Camera_FLASH_GPIO_Pin              GPIO_PIN_8
#define Camera_light_mode_auto             0
#define Camera_light_mode_sunny            1
#define Camera_light_mode_cloudy           2
#define Camera_light_mode_office           3
#define Camera_light_mode_home             4
#define Camera_special_effect_normal       0
#define Camera_special_effect_negative     1
#define Camera_special_effect_blackwhite   2
#define Camera_special_effect_redish       3
#define Camera_special_effect_greenish     4
#define Camera_special_effect_bluish       5
#define Camera_special_effect_antique      6
#define Camera_identification              "OV2640"
extern DCMI_HandleTypeDef camera_dcmi_handle;
extern DMA_HandleTypeDef camera_dma_dcmi_handle;
void Camera_pwdn_on(void);
void Camera_pwdn_off(void);
void Camera_init(void);
void Camera_set_flash_on(void);
void Camera_set_flash_off(void);
void Camera_set_jpeg_mode(void);
void Camera_set_rgb_mode(void);
void Camera_set_auto_exposure_level(uint8_t level);
void Camera_set_light_mode(uint8_t mode);
void Camera_set_color_saturation(uint8_t saturation);
void Camera_set_brightness(uint8_t brightness);
void Camera_set_contrast(uint8_t contrast);
void Camera_set_special_effects(uint8_t special_effects);
void Camera_set_window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
void Camera_set_output_size(uint16_t width,uint16_t height);
void Camera_set_image_window(uint16_t offx,uint16_t offy,uint16_t width,uint16_t height);
void Camera_set_image_size(uint16_t width, uint16_t height);
#endif
