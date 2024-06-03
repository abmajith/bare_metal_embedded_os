#include "mmio.h"
#include "gpio.h"
#include "rng.h"
#include "uart0.h"
#include "mailbox.h"
#include "power.h"
#include "timer.h"
#include "privilege.h"
#include "interrupt_controllers.h"

void convert_hex(unsigned int d);
void kernel_main(void);

void convert_hex (unsigned int d)
{
  unsigned int n;
  int c;
  for (c = 28; c >=0; c -=4)
  {
    n = (d >> c) & 0xF;
    n += n >9 ? 0x37 : 0x30;
    uart0_putc(n);
  }
}


void kernel_main(void)
{
  uart0_init(); // init debugging serial port
	rng_init();   // init the random number generation
	
	set_vbar_el1((uint64_t)vectors);

  uart0_puts("Hello, Folks!\n This is Bare Metal Embedded DEV (using ARM64 GCC ToolChain)\n");

  // set up our mail box message buffer
  mbox_buf[0] = 8*4;                  // size of our message buffer
  mbox_buf[1] = MAILBOX_M_REQUEST;    // request message
  mbox_buf[2] = MAILBOX_M_GETSERIAL;  // request to get serial number
  mbox_buf[3] = 8;                    // Value buffer size (bytes)
  mbox_buf[4] = 8;                    // Request size (bytes)
  mbox_buf[5] = 0;                    //
  mbox_buf[6] = 0;                    // End tag
  mbox_buf[7] = MAILBOX_M_LAST;       // pad the message with proper boundary close


  if (mailbox_call(MAILBOX_M_CH_PROP))
  {
    uart0_puts("Serial number is ");
    convert_hex(mbox_buf[5]);
    convert_hex(mbox_buf[6]);
    uart0_puts("\n");
  }
  else
  {
    uart0_puts("Unable to query Serial number!\n");
  }
	
	
	// testing my random numbers
	unsigned int random_number = rng_get_random();
	uart0_puts("Generated random number is : ");
	
	convert_hex(random_number);
	
	uart0_putc('\n');
	uart0_putc('\r');
	random_number = rng_get_random();
	uart0_puts("another one : ");
	convert_hex(random_number);
	uart0_putc('\n');
	uart0_putc('\r');
	
	unsigned int el = get_current_el();

	uart0_puts("Current Exception Level: ");
	uart0_putc('0' + el);  // Assuming EL is between 0 and 3
	uart0_puts("\n");
	uart0_putc('\r');

	

	// testing my timer delay pooling type
	uart0_puts("Shutting down the raspberry Pi 3B plus board...\r\n");
	delay_milliseconds(0x10);

	// testing my power managment functions
	/*
  // both were tested it works
  uart0_puts("watch dog reset...\n");
  watchdog_reset(0x10000);
  uart0_puts("reset...\n");
  reset();
  */
	power_off();


  while(1) { uart0_putc(uart0_getc()); } // infinite loop
}

