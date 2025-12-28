
#include "mmio.h"
#include "power.h"
#include "gpio.h"
#include "mailbox.h"

void power_off()
{
	unsigned long r;
  
  for (r = 0; r < 16; r++)
  { 
    mbox_buf[0] = 8 * 4;
    mbox_buf[1] = MAILBOX_M_REQUEST;
    mbox_buf[2] = MAILBOX_M_SETPOWER;
    mbox_buf[3] = 8;
    mbox_buf[4] = 8;
    mbox_buf[5] = (unsigned int) r;   // device id
    mbox_buf[6] = 0;                  // bit 0: off, bit 1: no wait
    mbox_buf[7] = MAILBOX_M_LAST;
    mailbox_call(MAILBOX_M_CH_PROP);
  }
  
  *GPFSEL0 = 0;
  *GPFSEL1 = 0;
  *GPFSEL2 = 0;
  *GPFSEL3 = 0;
  *GPFSEL4 = 0;
  *GPFSEL5 = 0;
  
  *GPPUD   = 0;

	for (r = 0; r < 150; r++) {asm volatile("nop");}
  *GPPUDCLK0 = 0xFFFFFFFF;
  *GPPUDCLK1 = 0xFFFFFFFF;
  for (r = 0; r < 150; r++) {asm volatile("nop");}
  *GPPUDCLK0 = 0;
  *GPPUDCLK1 = 0;

  r = *PM_RSTS;
  r &= ~0xFFFFFAAA;
  r |= 0x555;
  *PM_RSTS = PM_WDOG_MAGIC | r;
  *PM_WDOG = PM_WDOG_MAGIC | 10;
  *PM_RSTC = PM_WDOG_MAGIC | PM_RSTC_FULLRST;
}


void reset()
{
	unsigned int r;

  r = *PM_RSTS;
  r &= ~0xFFFFFAAA;
  *PM_RSTS = PM_WDOG_MAGIC | r;
  *PM_WDOG = PM_WDOG_MAGIC | 10;
  *PM_RSTC = PM_WDOG_MAGIC | PM_RSTC_FULLRST;
}


void watchdog_reset(unsigned int ticks)
{
	unsigned int r;
	
	r = *PM_RSTS;
  r &= ~0xFFFFFAAA;
  *PM_RSTS = PM_WDOG_MAGIC | r;
  *PM_WDOG = PM_WDOG_MAGIC | ticks;
	*PM_RSTC = PM_WDOG_MAGIC | PM_RSTC_FULLRST;
}
