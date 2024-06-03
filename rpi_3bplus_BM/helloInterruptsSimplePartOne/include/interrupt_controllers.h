

#ifndef  INTERRUPT_CONTROLLERS_H 
#define  INTERRUPT_CONTROLLERS_H


#include <stdint.h>


void default_handler(void);
void irq_handler(void);
void fiq_handler(void);

void set_vbar_el1(uint64_t addr);


extern volatile __attribute__((aligned(2048)))  void (*vectors[16])(void);

#endif  // INTERRUPT_CONTROLLERS_H
