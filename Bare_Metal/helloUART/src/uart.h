/*
* document all the functions defined in uart.c file
*/

#ifndef UART_H
#define UART_H


void uart_init();
void uart_putc(unsigned int c);
unsigned char uart_getc();
void uart_puts(char *s);


#endif // UART_H

