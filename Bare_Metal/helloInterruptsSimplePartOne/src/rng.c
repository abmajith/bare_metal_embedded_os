#include "rng.h"

void rng_init() {
	// Enable the RNG and clear the interrupt mask
	*RNG_STATUS = 0x40000;
	*RNG_CTRL |= 1;
	*RNG_INT_MASK |= 1;  

}

unsigned int rng_get_random() {
	// Wait until a random number is available
	while (!((*RNG_STATUS) >> 24)) { asm volatile("nop"); }

	return *RNG_DATA;

}
