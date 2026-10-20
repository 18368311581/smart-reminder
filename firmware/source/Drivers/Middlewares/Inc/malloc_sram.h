#ifndef __MALLOC_SRAM_H_
#define __MALLOC_SRAM_H_
#include "basic.h"
#define MALLOC_SRAM_SIZE_KB      960
#define MALLOC_SRAM_BLOCK_SIZE_B 32
void Malloc_sram_init(void);
float Malloc_sram_get_usage(void);
void Malloc_sram_set(void* start_address,uint8_t data,uint32_t data_len);
void Malloc_sram_copy(void* destiny_address,void* source_adddress,uint32_t data_len);
void Malloc_sram_free(void* address);
void* Malloc_sram_alloc(uint32_t size);
void* Malloc_sram_realloc(void *address,uint32_t size);
#endif
