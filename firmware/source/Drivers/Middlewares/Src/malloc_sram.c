#include "malloc_sram.h"
static __align(64) uint8_t memory[MALLOC_SRAM_SIZE_KB*1024] __attribute__((at(0x68000000)));
static __align(64) uint16_t memory_table[MALLOC_SRAM_SIZE_KB*1024/MALLOC_SRAM_BLOCK_SIZE_B] __attribute__((at(0x68000000+MALLOC_SRAM_SIZE_KB*1024)));
static const uint32_t memory_block_size =  MALLOC_SRAM_BLOCK_SIZE_B;
static const uint32_t memory_size       =  MALLOC_SRAM_SIZE_KB*1024;
static const uint32_t memory_table_size =  memory_size/memory_block_size;
void Malloc_sram_init(void){
    Malloc_sram_set(memory_table,0,memory_table_size*2);
}
float Malloc_sram_get_usage(void){
    float used=0;
    for(uint32_t i=0;i<memory_table_size;i++){
        if(memory_table[i])used+=1;
    }
    used/=memory_table_size;
    return used;
}
void Malloc_sram_copy(void *destiny_address,void *source_adddress,uint32_t data_len){
    uint8_t *xdes=destiny_address;
    uint8_t *xsrc=source_adddress;
    while(data_len--)*xdes++=*xsrc++;
}
void Malloc_sram_set(void *start_address,uint8_t data,uint32_t data_len){
    uint8_t *xs=start_address;
    while(data_len--)*xs++=data;
}
static uint32_t mem_malloc(uint32_t size){
    signed long offset=0;
    uint32_t nmemb;
    uint32_t cmemb=0;
    if(size==0)return 0xFFFFFFFF;
    nmemb=size/memory_block_size;
    if(size%memory_block_size)nmemb++;
    for(offset=memory_table_size-1;offset>=0;offset--){
        if (!memory_table[offset]){
            cmemb++;
        }
        else{
            cmemb=0;
        }
        if(cmemb==nmemb){
            for(uint32_t i=0;i<nmemb;i++){
                memory_table[offset+i]=nmemb;
            }
            return (offset*memory_block_size);
        }
    }
    return 0xFFFFFFFF;
}
static uint8_t mem_free(uint32_t offset){
    if(offset<memory_size){
        int index=offset/memory_block_size;
        int nmemb=memory_table[index];
        for(uint32_t i=0;i<nmemb;i++){
            memory_table[index+i]=0;
        }
        return 0;
    }
    else{
        return 2;
    }
}
void Malloc_sram_free(void* address){
    if(address==NULL)return;
    mem_free((uint32_t)address-(uint32_t)memory);
}
void* Malloc_sram_alloc(uint32_t size){
    uint32_t offset=mem_malloc(size);
    if(offset==0xFFFFFFFF){
        return NULL;
    }
    else{
        return (void*)((uint32_t)memory+offset);
    }
}
void* Malloc_sram_realloc(void *address,uint32_t size){
    uint32_t offset=mem_malloc(size);
    if(offset==0xFFFFFFFF){
        return NULL;
    }
    else{
        Malloc_sram_copy((void *)((uint32_t)memory+offset),address,size);
        Malloc_sram_free(address);
        return (void *)((uint32_t)memory+offset);
    }
}
