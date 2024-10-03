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

#define set_bit(reg, bit) (reg |= (1 << bit))    // set bit
#define clear_bit(reg, bit) (reg &= ~(1 << bit)) // clear bit
#define test_bit(reg, bit) (reg & (1 << bit))    // read bit
#define loop_until_bit_is_set(reg, bit) while(!test_bit(reg, bit))
#define loop_until_bit_is_clear(reg, bit) while(test_bit(reg, bit))

//char *test = "TEST";

int main()
{
    // DDRA |= (1 << DDA0); //set as output
    // DDRA |= 0b11111111; // set all PORT A pins as outputs 
    // DDRE |= (1 << DDE1); //set as output

    USART_Init(MYUBRR);
    fdevopen(USART_Transmit, USART_Receive);

    xmem_init();
    pwm_init();
    oled_init();
    // SRAM_test();
    


    uint8_t* adc_values = (uint8_t *) malloc(6*sizeof(uint8_t));	
    uint8_t* xy_saturation = (uint8_t *) malloc(4*sizeof(uint8_t)); // in order: x_min, x_max, y_min, y_max

    //joystick_configuration(xy_saturation, adc_values);

    oled_clear_screen();

    /*
    oled_set_column_address(0);
    oled_set_page_address(0);
    oled_data(0xFF);
    oled_set_column_address(64);
    oled_set_page_address(4);
    oled_data(0xFF);
    oled_set_column_address(127);
    oled_set_page_address(7);
    oled_data(0xFF);
    */
    
    while(1) {
		//uint8_t value = ADC_BASE_ADR[0];
        //printf(value);
        
		//USART_Receive();
		//printf("\nADC Values\n");
        //adc_read(adc_values);
		//printf("\r Joystick X: %d, Joystick Y: %d\r\n Slider Left: %d, Slider Right: %d\r\n Button Left: %d, Button Right: %d\r\n",adc_values[1], adc_values[0], adc_values[2], adc_values[3], adc_values[4], adc_values[5]);
        //printf("[%d, %d, %d, %d, %d, %d]\r\n", adc_values[1], adc_values[0], adc_values[3], adc_values[2], adc_values[5], adc_values[4]);
        
        //oled_data(0xFF);
        //_delay_ms(1000);
        //oled_data(0);
        //_delay_ms(1000);
    }

    free(adc_values);
    free(xy_saturation);
}

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

/* Fuse configuration
avrdude -p m162 -c atmelice -U lfuse:w:0xC1:m 	-U hfuse:w:0x19:m
*/