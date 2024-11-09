/* Clock speed */
#define FOSC 4915200UL
#define F_CPU 4915200 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* 
 * Fuse configuration
 * avrdude -p m162 -c atmelice -U lfuse:w:0xC1:m -U hfuse:w:0x19:m
 */

#include "util.h"
#include "xmem.h"
#include "uart.h"
#include "adc.h"
#include "oled.h"
#include "spi.h"
#include "can_controller.h"

#define BAUD 9600UL
#define MYUBRR (FOSC/16/BAUD - 1)

#define set_bit(reg, bit) (reg |= (1 << bit))   /* set bit */
#define clear_bit(reg, bit) (reg &= ~(1 << bit))    /* clear bit */
#define test_bit(reg, bit) (reg & (1 << bit))   /* read bit */

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
((byte) & 0x80 ? '1' : '0'), \
((byte) & 0x40 ? '1' : '0'), \
((byte) & 0x20 ? '1' : '0'), \
((byte) & 0x10 ? '1' : '0'), \
((byte) & 0x08 ? '1' : '0'), \
((byte) & 0x04 ? '1' : '0'), \
((byte) & 0x02 ? '1' : '0'), \
((byte) & 0x01 ? '1' : '0') 

/* printf("Leading text "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte)); */

int main(void)
{
    // DDRA |= (1 << DDA0); //set as output
    // DDRA |= 0b11111111; // set all PORT A pins as outputs 
    // DDRE |= (1 << DDE1); //set as output

    printf("uart_init(): %d\r\n", uart_init(MYUBRR));
    printf("xmem_init(): %d\r\n", xmem_init());
    printf("pwm_init():  %d\r\n", pwm_init());
    printf("oled_init(): %d\r\n", oled_init());
    printf("spi_init():  %d\r\n", spi_init());
    printf("can_init():  %d\r\n", can_init());

    fdevopen((int(*) (char, FILE*))&uart_transmit, (int(*) (FILE*))&uart_receive);

   #define CNF1        0b00101010
    #define CNF2        0b00101001
    #define CNF3        0b00101000

    printf("CNF1: %d\n\r", can_read(CNF1));
    printf("CNF2: %d\n\r", can_read(CNF2));
    printf("CNF3: %d\n\r", can_read(CNF3));

    // SRAM_test();
    


    uint8_t* adc_values = malloc(6*sizeof(uint8_t));	
    //uint8_t* xy_saturation = malloc(4*sizeof(uint8_t)); // in order: x_min, x_max, y_min, y_max

    //joystick_configuration(xy_saturation, adc_values);

    oled_clear_screen();

    //uint8_t *menu_entries = (uint8_t *) malloc(menu_height*sizeof(uint8_t));
    uint8_t menu_height = 8;
    uint8_t menu_width = 128;
    //char **menu = malloc(menu_width*sizeof(char *));
    //for(uint8_t i = 0; i<menu_width; i++) menu[i] = malloc(menu_height*sizeof(char));

    //menu[0] = ">Option 1";
    //menu[1] = " Option 2";
    //menu[2] = " Option 3";
    //menu[3] = " Option 4";
    //menu[4] = " Option 5";
    //menu[5] = " Option 6";
    //menu[6] = " Option 7";
    //menu[7] = " Option 8";

    //oled_draw_menu(menu, menu_height, menu_width);

    can_config();
    can_configure_transmit();
    can_configure_receive();
    can_configure_filters_and_masks();

    printf("CNF1: %d\n\r", can_read(CNF1));
    printf("CNF2: %d\n\r", can_read(CNF2));
    printf("CNF3: %d\n\r", can_read(CNF3));

    //can_set_loopback();
    can_set_normal();

    //volatile struct can_message *can_tx_buffer = malloc(sizeof(struct can_message));
    struct can_message *can_tx_buffer = malloc(12*sizeof(uint8_t));
    //can_tx_buffer->buffer_start_address = 0b00110000;
    can_tx_buffer->buffer_start_address = 48;
    can_tx_buffer->message_id_high = 0;
    can_tx_buffer->message_id_low = 0;
    can_tx_buffer->data0 = 123;
    can_tx_buffer->data1 = 123;
    can_tx_buffer->data2 = 123;
    can_tx_buffer->data3 = 123;
    can_tx_buffer->data4 = 123;
    can_tx_buffer->data5 = 123;
    can_tx_buffer->data6 = 123;
    can_tx_buffer->data7 = 123;

    //volatile struct can_message *can_rx_buffer = malloc(sizeof(struct can_message));
    struct can_message *can_rx_buffer = malloc(12*sizeof(uint8_t));
    //can_rx_buffer->buffer_start_address = 0b01100000;
    can_rx_buffer->buffer_start_address = 96;
    can_tx_buffer->message_id_high = 0;
    can_rx_buffer->message_id_low = 0;
    can_rx_buffer->data0 = 0;
    can_rx_buffer->data1 = 0;
    can_rx_buffer->data2 = 0;
    can_rx_buffer->data3 = 0;
    can_rx_buffer->data4 = 0;
    can_rx_buffer->data5 = 0;
    can_rx_buffer->data6 = 0;
    can_rx_buffer->data7 = 0;

    while(1) {
        adc_transmit(adc_values, can_tx_buffer);
        printf("Joystick X: %d, Joystick Y: %d\r\n Slider Left: %d, Slider Right: %d\r\n Button Left: %d, Button Right: %d\r\n",
        can_tx_buffer->data1, can_tx_buffer->data0, can_tx_buffer->data2, can_tx_buffer->data3, can_tx_buffer->data4, can_tx_buffer->data5);
        //can_transmit_message(can_tx_buffer);
        /*
          can_receive_message(can_rx_buffer);
          printf("RX Buffer:%d %d %d %d |%d|%d|%d|%d|%d|%d|%d|%d|\r\n", can_rx_buffer->buffer_start_address, can_rx_buffer->message_id_high, can_rx_buffer->message_id_low, can_rx_buffer->data_length, can_rx_buffer->data0, can_rx_buffer->data1, can_rx_buffer->data2, can_rx_buffer->data3, can_rx_buffer->data4, can_rx_buffer->data5, can_rx_buffer->data6, can_rx_buffer->data7);
          printf("End of loop\r\n");
          printf("\r\n");
        */
  
        _delay_ms(20);
    }

    free(adc_values);
    //free(xy_saturation);
    //free(menu_entries);
    //free(menu);
    free(can_tx_buffer);
    free(can_rx_buffer);
}
