#include "mmio.h"
#include "uart0.h"
#include "mailbox.h"

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
	uart0_init();

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
  while(1) 
	{
		uart0_putc(uart0_getc());
	}
}
