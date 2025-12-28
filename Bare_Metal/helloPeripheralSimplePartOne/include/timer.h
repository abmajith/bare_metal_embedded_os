/*
*/


#ifndef TIMER_H
#define TIMER_H

#include "mmio.h"


#define SYS_TIMER_CS    ((volatile unsigned int*)(SYS_TIMER_BASE + 0x00))
#define SYS_TIMER_CLO   ((volatile unsigned int*)(SYS_TIMER_BASE + 0x04))
#define SYS_TIMER_CHI   ((volatile unsigned int*)(SYS_TIMER_BASE + 0x08))
#define SYS_TIMER_C0    ((volatile unsigned int*)(SYS_TIMER_BASE + 0x0C))
#define SYS_TIMER_C1    ((volatile unsigned int*)(SYS_TIMER_BASE + 0x10))
#define SYS_TIMER_C2    ((volatile unsigned int*)(SYS_TIMER_BASE + 0x14))
#define SYS_TIMER_C3    ((volatile unsigned int*)(SYS_TIMER_BASE + 0x18))



// polling based delay inefficient usage of processor
void delay_microseconds(unsigned int us);

unsigned long get_arm_system_timer();
void delay_milliseconds(unsigned int ms);


#endif // TIMER_H

