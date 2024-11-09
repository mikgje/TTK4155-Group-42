#include <stdint.h>
#include <avr/io.h>

#ifndef UART_H
#define UART_H

typedef struct __file FILE;

uint8_t uart_init(uint16_t ubrr);
uint8_t uart_transmit(uint8_t data, FILE* file);
uint8_t uart_receive(FILE* file);

#endif
