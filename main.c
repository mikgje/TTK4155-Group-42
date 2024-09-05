#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
//#include "avr-libc/include/avr/iom162.h"

#include "uart.h" 

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

    while (1) {
        USART_Transmit('a');
        _delay_ms(5000);
    }
    /*
    */
}

/*

avrdude -p m162 -c atmelice -U lfuse:w:0xC1:m 	-U hfuse:w:0x19:m
*/