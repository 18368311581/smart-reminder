#include "malloc_sram.h"
#include "ertc.h"
#include "basic.h"
#include "ff.h"
DWORD get_fattime(void){return get_time_fat_format();}
void *ff_memalloc(UINT size){return (void*)Malloc_sram_alloc(size);}
void ff_memfree (void* mf){Malloc_sram_free(mf);}
