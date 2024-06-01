#include "timer.h"


void delay_microseconds(unsigned int us)
{
	if (us) 
	{
		while(us--) { asm volatile("nop"); }
	}
}

unsigned long get_arm_system_timer()
{
  unsigned high, low;
  high = *SYS_TIMER_CHI;
  low = *SYS_TIMER_CLO;

  return ((unsigned long) high << 32) | low;
}


void delay_milliseconds(unsigned int ms)
{
	unsigned long t = get_arm_system_timer();
  if (t)
  {
    while (get_arm_system_timer() - t < ms) {}
  }
}
