#ifndef __huart_h
#define __huart_h

#include <stdio.h>

void uart_init(void);
char uart_getchar(FILE *stream);
int uart_putchar(char c, FILE *stream);

#endif