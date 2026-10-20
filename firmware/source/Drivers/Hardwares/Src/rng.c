#include "rng.h"
static RNG_HandleTypeDef rng_handle;
void Rng_init(void){
    uint16_t retry=0;
    rng_handle.Instance=RNG;
    HAL_RNG_DeInit(&rng_handle);
    HAL_RNG_Init(&rng_handle);
    while(__HAL_RNG_GET_FLAG(&rng_handle,RNG_FLAG_DRDY)==RESET&&retry<10000){
        retry++;
        delay_us(10);
    }
}
uint32_t Rng_get_int(void){
    uint32_t randomnum;
    HAL_RNG_GenerateRandomNumber(&rng_handle,&randomnum);
    return randomnum;
}
int Rng_get_range_int(int min,int max){
    uint32_t randomnum;
    HAL_RNG_GenerateRandomNumber(&rng_handle,&randomnum);
    return randomnum%(max-min+1)+min;
}
float Rng_get_range_float(float min,float max){
    uint32_t randomnum;
    HAL_RNG_GenerateRandomNumber(&rng_handle,&randomnum);
    float u=(float)randomnum/4294967295.0f;
    float result=min+(max-min)*u;
    return result;
}
