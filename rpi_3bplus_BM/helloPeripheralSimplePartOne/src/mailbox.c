
#include "mailbox.h"


volatile unsigned int __attribute__((aligned(16))) mbox_buf[36]; // provide some space for mailbox communications



int mailbox_call(unsigned char channel)
{
  unsigned int r = (((unsigned int)((unsigned long)&mbox_buf) & ~0xF) | (channel & 0xF));
  // wait for the mail box
  do{asm volatile("nop");}while (*MAILBOX_STATUS & MAILBOX_FULL);

  /* sending aligned mailbox memory address, so that gpu firmware will read
  * and perform actions accordingly
  */

  *MAILBOX_WRITE = r;
  while (1)
  {
    // wait until mailbox is not empty
    do{asm volatile("nop");}while (*MAILBOX_STATUS & MAILBOX_EMPTY);

    if (r == *MAILBOX_READ)
    {
      return mbox_buf[1] == MAILBOX_RESPONSE; // got the acknowledgement, gpu firmware read our mailbox message
    }
  }
  return 0;
}

