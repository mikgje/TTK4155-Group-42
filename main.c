#define FOSC 4915200UL // Clock Speed
#define F_CPU 4915200 // Clock Speed

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
// #include "avr-libc/include/avr/iom162.h"

#include "uart.h"
#define BAUD 9600UL
#define MYUBRR (FOSC/16/BAUD - 1)

char *ole = "Ole";

/*
DDRA |=(1 << PA0); //set as output
DDRA |=(1 << PE1);//set as output

PORTA |=(1 << PA0);//GIVE SIGNAL THAT GOES TO LED
PORTA |=(1 << PE1);//GIVE SIGNAL SO SE IS ON AND GIVES OUTPUT
*/


void latch

int main()
{
    /*
    DDRA = 0x01;
    while (1)
    {
        PORTA = 1;
        _delay_ms(1000);
        PORTA = 0;
        _delay_ms(1000);
    }
    return 0;
    */

    USART_Init(MYUBRR);
    fdevopen(USART_Transmit, USART_Receive);

    while (1)
    {
        //USART_Transmit(USART_Receive());
        //printf("%s ", ole);
        _delay_ms(1000);
    }
    /*
     */
}

/*

avrdude -p m162 -c atmelice -U lfuse:w:0xC1:m 	-U hfuse:w:0x19:m
*/