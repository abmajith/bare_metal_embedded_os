#include "mailbox.h"
#include "framebuffer.h"
#include "timer.h"
#include "uart0.h"

unsigned int fb_width, fb_height, fb_pitch, fp_isrgb;
unsigned char *fb_addr;


int framebuffer_init()
{
	delay_milliseconds(1000);
	mbox_buf[0] = 35 * 4;
	mbox_buf[1] = MAILBOX_M_REQUEST;
	mbox_buf[2] = 0x00048003; // Set physical width/height
	mbox_buf[3] = 8;          // size of the unsigned int parameter
	mbox_buf[4] = 8;
	mbox_buf[5] = FRAMEBUFFER_WIDTH;
	mbox_buf[6] = FRAMEBUFFER_HEIGHT;

	mbox_buf[7] = 0x00048004; // Set virtual width/height
	mbox_buf[8] = 8;
	mbox_buf[9] = 8;
	mbox_buf[10] = FRAMEBUFFER_WIDTH;
	mbox_buf[11] = FRAMEBUFFER_HEIGHT;

	mbox_buf[12] = 0x00048009;  // set virtual offset
	mbox_buf[13] = 8;   
	mbox_buf[14] = 8;   
	mbox_buf[15] = 0;   // x offset
	mbox_buf[16] = 0;   // y offset
	
	mbox_buf[17] = 0x00048005; // Set depth
	mbox_buf[18] = 4;
	mbox_buf[19] = 4;
	mbox_buf[20] = FRAMEBUFFER_DEPTH;

	mbox_buf[21] = 0x00040001; // Allocate buffer
	mbox_buf[22] = 8;
	mbox_buf[23] = 8;
	mbox_buf[24] = 4096; // Alignment
	mbox_buf[25] = 0; // Will be filled by the GPU

	mbox_buf[26] = 0x00048009; // Get pitch
	mbox_buf[27] = 4;
	mbox_buf[28] = 4;
	mbox_buf[29] = 0; // Will be filled by the GPU

	mbox_buf[30] = 0x48006; // setting up pixel order
	mbox_buf[31] = 4;
	mbox_buf[32] = 4;
	mbox_buf[33] = 1;  // RGB format

	mbox_buf[34] = MAILBOX_M_LAST;

	if (mailbox_call(MAILBOX_M_CH_PROP) && mbox_buf[20] == FRAMEBUFFER_DEPTH && mbox_buf[24] != 0 )
	{
		fb_width  = mbox_buf[5];
		fb_height = mbox_buf[6];
		fb_pitch  = mbox_buf[29];
		fp_isrgb  = mbox_buf[33];
		fb_addr   = (void*) ((unsigned long)mbox_buf[24]);
		return 0;
	}
	else 
	{
		uart0_printf("Unable to set frame buffer");
		return 1; 
	}
}
