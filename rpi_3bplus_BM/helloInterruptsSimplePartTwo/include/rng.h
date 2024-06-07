#ifndef RNG_H
#define RNG_H

#include "mmio.h"

#define RNG_CTRL        ((volatile unsigned int *)(RNG_BASE + 0x00))
#define RNG_STATUS      ((volatile unsigned int *)(RNG_BASE + 0x04))
#define RNG_DATA        ((volatile unsigned int *)(RNG_BASE + 0x08))
#define RNG_INT_MASK    ((volatile unsigned int *)(RNG_BASE + 0x10))

void rng_init();
unsigned int rng_get_random();

#endif // RNG_H
