/*
* For setting up the mail box request
*/


#ifndef MAILBOX_H
#define MAILBOX_H

extern volatile unsigned int __attribute__((aligned(16))) mbox_buf[36];

int mailbox_call(unsigned char channel);

#endif // MAILBOX_H
