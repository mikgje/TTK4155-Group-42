#define FOSC 4915200UL // Clock Speed
#define F_CPU 4915200 // Clock Speed

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
// #include "avr-libc/include/avr/iom162.h"

#include "xmem.h"
#include "uart.h"
#include "adc.h"
#define BAUD 9600UL
#define MYUBRR (FOSC/16/BAUD - 1)

char *test = "TEST";

int main()
{
    // DDRA |= (1 << DDA0); //set as output
    // DDRA |= 0b11111111; // set all PORT A pins as outputs 
    // DDRE |= (1 << DDE1); //set as output

    USART_Init(MYUBRR);
    fdevopen(USART_Transmit, USART_Receive);

    xmem_init();
    pwm_init();
    // SRAM_test();
    
    uint8_t* adc_values = (uint8_t *) malloc(6*sizeof(uint8_t));	

    while(1) {
		//uint8_t value = ADC_BASE_ADR[0];
        //printf(value);

		USART_Receive();
		printf("\nADC Values\r\n");
        adc_read(adc_values);
		printf("[%4u, %4u, %4u, %4u, %4u, %4u]\r\n",adc_values[0], adc_values[1], adc_values[2], adc_values[3], adc_values[4], adc_values[5]);
        
    }

    free(adc_values);
	adc_values = NULL;

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