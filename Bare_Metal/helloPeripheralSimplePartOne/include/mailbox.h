/*
*/



#ifndef MAILBOX_H
#define MAILBOX_H

#include "mmio.h"



#define MAILBOX_READ    ((volatile unsigned int *)(MAILBOX_BASE + 0x0))
#define MAILBOX_POLL    ((volatile unsigned int *)(MAILBOX_BASE + 0x10))
#define MAILBOX_SENDER  ((volatile unsigned int *)(MAILBOX_BASE + 0x14))
#define MAILBOX_STATUS  ((volatile unsigned int *)(MAILBOX_BASE + 0x18))
#define MAILBOX_CONFIG  ((volatile unsigned int *)(MAILBOX_BASE + 0x1C))
#define MAILBOX_WRITE   ((volatile unsigned int *)(MAILBOX_BASE + 0x20))



// some mail box message type (return message)
#define MAILBOX_RESPONSE   0x80000000
#define MAILBOX_FULL       0x80000000
#define MAILBOX_EMPTY      0x40000000


// some mail box channels type

#define MAILBOX_M_CH_POWER    0 // for power management
#define MAILBOX_M_CH_FB       1 // for frame buffer
#define MAILBOX_M_CH_VUART    2 // 
#define MAILBOX_M_CH_VCHIQ    3 // for interrupt controller
#define MAILBOX_M_CH_LEDS     4
#define MAILBOX_M_CH_BTNS     5
#define MAILBOX_M_CH_TOUCH    6
#define MAILBOX_M_CH_COUNT    7 // for counter
#define MAILBOX_M_CH_PROP     8 // for property


// some mail box message request type (sending message)
#define MAILBOX_M_GETSERIAL   0x10004
#define MAILBOX_M_SETCLKRATE  0x38002
#define MAILBOX_M_GETCLKRATE  0x30002
#define MAILBOX_M_SETPOWER    0x28001
#define MAILBOX_M_LAST        0
#define MAILBOX_M_REQUEST     0

extern volatile unsigned int __attribute__((aligned(16))) mbox_buf[36];

int mailbox_call(unsigned char channel);


#endif // MAILBOX_H

