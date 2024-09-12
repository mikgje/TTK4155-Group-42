#define FOSC 4915200UL // Clock Speed
#define F_CPU 4915200 // Clock Speed

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
// #include "avr-libc/include/avr/iom162.h"

#include "xmem.h"
#include "uart.h"
#define BAUD 9600UL
#define MYUBRR (FOSC/16/BAUD - 1)

char *test = "TEST";


// void latch

int main()
{
    // DDRA |= (1 << DDA0); //set as output
    DDRA |= 0b11111111; // set all PORT A pins as outputs 
    DDRE |= (1 << DDE1); //set as output

    USART_Init(MYUBRR);
    fdevopen(USART_Transmit, USART_Receive);
    /* UART - Don't delete
    */

    xmem_init();
    SRAM_test();
    /*
    while (1)
    {
        //USART_Transmit(USART_Receive());
        //printf("%s ", test);

        PORTA |= (1 << PA0);//GIVE SIGNAL THAT GOES TO LED
        PORTA |= 0b10101010;
        PORTE |= (1 << PE1);//GIVE SIGNAL SO SE IS ON AND GIVES OUTPUT
        _delay_ms(1000);
        PORTA &= !(1 << PA0);
        PORTA = 0;
        _delay_ms(1000);
    }
     */
}

/* Fuse configuration
avrdude -p m162 -c atmelice -U lfuse:w:0xC1:m 	-U hfuse:w:0x19:m
*/