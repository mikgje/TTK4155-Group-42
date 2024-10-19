#include "uart.h"
#include <avr/io.h>
#include <stdint.h>

void uart_init(unsigned int ubrr)
{
    /* Set baud rate */
    UBRR0H = (unsigned char) (ubrr >> 8);
    UBRR0L = (unsigned char) ubrr;
    /* Enable receiver and transmitter */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);
}

void uart_transmit(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)));
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

unsigned char uart_receive( void )
{
/* Wait for data to be received */
//while ( !(UCSR0A) & (1<<RXC0));
while( !(UCSR0A & (1<<RXC0)) );
/* Get and return received data from buffer */
return UDR0;
}