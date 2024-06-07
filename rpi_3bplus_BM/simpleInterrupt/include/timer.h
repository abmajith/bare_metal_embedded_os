#ifndef TIMER_H
#define TIMER_H

#include "mmio.h"


#define SYS_TIMER_BASE  (MMIO_BASE + 0x00003000)  // System Timer


#define SYS_TIMER_CS    ((volatile unsigned int*)(SYS_TIMER_BASE + 0x00))
#define SYS_TIMER_CLO   ((volatile unsigned int*)(SYS_TIMER_BASE + 0x04))
#define SYS_TIMER_CHI   ((volatile unsigned int*)(SYS_TIMER_BASE + 0x08))
#define SYS_TIMER_C0    ((volatile unsigned int*)(SYS_TIMER_BASE + 0x0C))
#define SYS_TIMER_C1    ((volatile unsigned int*)(SYS_TIMER_BASE + 0x10))
#define SYS_TIMER_C2    ((volatile unsigned int*)(SYS_TIMER_BASE + 0x14))
#define SYS_TIMER_C3    ((volatile unsigned int*)(SYS_TIMER_BASE + 0x18))

#define SYSTEM_TIMER_CS_IRQ_1  (1 << 1)


// For generating pure system timer interrupt
void timer_init(void);
void handle_timer_interrupt(void);

#endif // TIMER_H
