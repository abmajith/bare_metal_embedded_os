
#include "uart0.h"
#include "interrupt_controller.h"
#include "timer.h"



const unsigned int interval_timer1 = 10000000;
unsigned int curVal_timer1 = 0;


void timer_init(void) {
    curVal_timer1 = *SYS_TIMER_CLO;
    curVal_timer1 += interval_timer1;
    *SYS_TIMER_C1 = curVal_timer1; // Setting the future timer to raise the interrupts
}

void handle_timer_interrupt(void) 
{
    curVal_timer1 += interval_timer1; // Update the next timestamp to raise the interrupt
    *SYS_TIMER_C1 = curVal_timer1;    // Set the future timer to raise the interrupts
    *SYS_TIMER_CS = SYSTEM_TIMER_CS_IRQ_1;  // Clear the timer interrupt flag by writing 1 to the respective bit
    uart0_printf("Timer Interrupt 1 received %u\r\n", *SYS_TIMER_C1 ); 
		// Currently we just generate the timer interrupt regularly and print them on the screen
		enable_interrupt();
}

