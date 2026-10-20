#ifndef __RNG_H_
#define __RNG_H_
#include "basic.h"
void Rng_init(void);
uint32_t Rng_get_int(void);
int Rng_get_range_int(int min,int max);
float Rng_get_range_float(float min,float max);
#endif
