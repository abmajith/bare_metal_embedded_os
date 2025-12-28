#include "timer.h"
#include "uart0.h"
#include "interrupt_controllers.h"

const unsigned int interval_timer1 = 2000000;
unsigned int curVal_timer1 = 0;


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

void timer_init ( void )
{
	curVal_timer1 = *SYS_TIMER_CLO;
	curVal_timer1 += interval_timer1;
	*SYS_TIMER_C1 = curVal_timer1; // setting the future timer to raise the interrrupts
}

void handle_timer_interrupt ( void )
{
	*SYS_TIMER_CS = SYSTEM_TIMER_CS_IRQ_1;
	curVal_timer1 += interval_timer1; // update the next time stamp to raise the interrupt
	*SYS_TIMER_C1 = curVal_timer1; // setting the future timer to raise the interrrupts

	uart0_printf("Timer Interrupt 1 received %u\r\n", *SYS_TIMER_C1); 
	/* currently we just generate the timer interrupt 
		regularly and print them on the screen thats all! */
	enable_interrupt();
}
