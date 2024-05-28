/*
* document all the functions defined in uart.c file
*/

#ifndef UART0_H
#define UART0_H


void uart0_init();
void uart0_putc(unsigned int c);
unsigned char uart0_getc();
void uart0_puts(char *s);


#endif // UART0_H

