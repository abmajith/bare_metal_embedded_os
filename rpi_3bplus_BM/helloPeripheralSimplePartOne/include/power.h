#ifndef POWER_H
#define POWER_H

#include "mmio.h"

#define PM_BASE        (MMIO_BASE + 0x00100000)  // Base address for Power Management
#define PM_RSTC        ((volatile unsigned int*)(PM_BASE + 0x1C))
#define PM_RSTS        ((volatile unsigned int*)(PM_BASE + 0x20))
#define PM_WDOG        ((volatile unsigned int*)(PM_BASE + 0x24))

// Useful macros
#define PM_WDOG_MAGIC   0x5A000000
#define PM_RSTC_FULLRST 0x00000020

void power_off();
void reset();
void watchdog_reset(unsigned int ticks);

#endif // POWER_H
