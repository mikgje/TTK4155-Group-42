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
#include "oled.h"
#include "spi.h"
#include "can_controller.h"
#include "can_transceiver.h"

#define BAUD 9600UL
#define MYUBRR (FOSC/16/BAUD - 1)

#define set_bit(reg, bit) (reg |= (1 << bit))    // set bit
#define clear_bit(reg, bit) (reg &= ~(1 << bit)) // clear bit
#define test_bit(reg, bit) (reg & (1 << bit))    // read bit
#define loop_until_bit_is_set(reg, bit) while(!test_bit(reg, bit))
#define loop_until_bit_is_clear(reg, bit) while(test_bit(reg, bit))

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

// printf("Leading text "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte));

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
    spi_init();
    can_init();

    // SRAM_test();
    


    uint8_t* adc_values = (uint8_t *) malloc(6*sizeof(uint8_t));	
    uint8_t* xy_saturation = (uint8_t *) malloc(4*sizeof(uint8_t)); // in order: x_min, x_max, y_min, y_max

    //joystick_configuration(xy_saturation, adc_values);

    oled_clear_screen();

    //uint8_t *menu_entries = (uint8_t *) malloc(menu_height*sizeof(uint8_t));
    uint8_t menu_height = 8;
    uint8_t menu_width = 128;
    char **menu = (char **) malloc(menu_width*sizeof(char *));
    for(uint8_t i = 0; i<menu_width; i++) menu[i] = (char *) malloc(menu_height*sizeof(char));

    menu[0] = ">Option 1";
    menu[1] = " Option 2";
    menu[2] = " Option 3";
    menu[3] = " Option 4";
    menu[4] = " Option 5";
    menu[5] = " Option 6";
    menu[6] = " Option 7";
    menu[7] = " Option 8";

    oled_draw_menu(menu, menu_height, menu_width);
    /*
    oled_set_position(0,0);
    oled_draw_word_large("Hello World");
    oled_set_position(0,1);
    oled_draw_word_large("Hello World");
    oled_set_position(0,2);
    oled_draw_word_large("Hello World");
    */


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

    can_config();

    can_configure_transmit();
    can_configure_receive();

    can_set_loopback();

    //volatile struct can_message *can_tx_buffer = (struct can_message *) malloc(sizeof(struct can_message));
    volatile struct can_message *can_tx_buffer = (struct can_message *) malloc(9*sizeof(uint8_t));
    //can_tx_buffer->buffer_start_address = 0b00110000;
    can_tx_buffer->buffer_start_address = 48;

    //volatile struct can_message *can_rx_buffer = (struct can_message *) malloc(sizeof(struct can_message));
    volatile struct can_message *can_rx_buffer = (struct can_message *) malloc(9*sizeof(uint8_t));
    //can_rx_buffer->buffer_start_address = 0b01100000;
    can_rx_buffer->buffer_start_address = 96;

    while(1) {
		//uint8_t value = ADC_BASE_ADR[0];
        //printf(value);
        //printf("%s", menu[0]);

		//USART_Receive();

        //adc_read(adc_values);
        /*
        This is one the format: Joystick X, Joystick Y, Slider Left, Slider Right, Button Left, Button Right
        */
        //printf("[%d, %d, %d, %d, %d, %d] Menu return: %d\r\n", adc_values[1], adc_values[0], adc_values[3], adc_values[2], adc_values[4], adc_values[5], oled_move_menu(menu, menu_height, menu_width, adc_values));
        //printf("Svart\r\n");
        /*
        printf("Value 1: %d\r\n", SPI_MasterReceive());
        spi_transmit_single(12);
        printf("Value 2: %d\r\n", SPI_MasterReceive());
        spi_transmit_single(13);
        printf("Value 3: %d\r\n", SPI_MasterReceive());
        spi_transmit_single(14);
        */
        /*
        */
        /*
        printf("Value 1: %d\r\n", can_read(0b00110110));
        can_write(0b00110110, 12);
        printf("Value 2: %d\r\n", can_read(0b00110110));
        can_write(0b00110110, 13);
        printf("Value 3: %d\r\n", can_read(0b00110110));
        can_write(0b00110110, 11);
        _delay_ms(1000);
        */
        
        //send_can_message(0b00110110, 0b00000000);
        //printf("TXB0CTRL: %d\r\n", can_read(0b00110000));
        //can_tx_buffer->buffer_start_address = 0b00110000;
        can_tx_buffer->buffer_start_address = 48;
        can_tx_buffer->data0 = 0;
        can_tx_buffer->data1 = 0;
        can_tx_buffer->data2 = 0;
        can_tx_buffer->data3 = 0;
        can_tx_buffer->data4 = 0;
        can_tx_buffer->data5 = 0;
        can_tx_buffer->data6 = 0;
        can_tx_buffer->data7 = 0;

        //can_rx_buffer->buffer_start_address = 0b01100000;

        printf("TX Buffer:%d %d%d%d%d%d%d%d%d\r\n", can_tx_buffer->buffer_start_address, can_tx_buffer->data0, can_tx_buffer->data1, can_tx_buffer->data2, can_tx_buffer->data3, can_tx_buffer->data4, can_tx_buffer->data5, can_tx_buffer->data6, can_tx_buffer->data7);
        can_rx_buffer->buffer_start_address = 96;
        printf("RX Buffer:%d %d%d%d%d%d%d%d%d\r\n", can_rx_buffer->buffer_start_address, can_rx_buffer->data0, can_rx_buffer->data1, can_rx_buffer->data2, can_rx_buffer->data3, can_rx_buffer->data4, can_rx_buffer->data5, can_rx_buffer->data6, can_rx_buffer->data7);
        //printf("TX Buffer %d\r\n", can_tx_buffer->data1);
        can_transmit_message(can_tx_buffer);
        _delay_ms(500); 
        printf("TX Buffer:%d %d%d%d%d%d%d%d%d\r\n", can_tx_buffer->buffer_start_address, can_tx_buffer->data0, can_tx_buffer->data1, can_tx_buffer->data2, can_tx_buffer->data3, can_tx_buffer->data4, can_tx_buffer->data5, can_tx_buffer->data6, can_tx_buffer->data7);
        printf("RX Buffer:%d %d%d%d%d%d%d%d%d\r\n", can_rx_buffer->buffer_start_address, can_rx_buffer->data0, can_rx_buffer->data1, can_rx_buffer->data2, can_rx_buffer->data3, can_rx_buffer->data4, can_rx_buffer->data5, can_rx_buffer->data6, can_rx_buffer->data7);
        //printf("RX Buffer %d\r\n", can_rx_buffer->data1);
        can_receive_message(can_rx_buffer);
        _delay_ms(1000);
        printf("\r\n");

        //send_can_message(0b00110110, 0b11111111);
        //can_tx_buffer->buffer_start_address = 0b00110000;
        can_tx_buffer->buffer_start_address = 48;
        can_tx_buffer->data0 = 9;
        can_tx_buffer->data1 = 9;
        can_tx_buffer->data2 = 9;
        can_tx_buffer->data3 = 9;
        can_tx_buffer->data4 = 9;
        can_tx_buffer->data5 = 9;
        can_tx_buffer->data6 = 9;
        can_tx_buffer->data7 = 9;

        //can_rx_buffer->buffer_start_address = 0b01100000;


        printf("TX Buffer:%d %d%d%d%d%d%d%d%d\r\n", can_tx_buffer->buffer_start_address, can_tx_buffer->data0, can_tx_buffer->data1, can_tx_buffer->data2, can_tx_buffer->data3, can_tx_buffer->data4, can_tx_buffer->data5, can_tx_buffer->data6, can_tx_buffer->data7);
        can_rx_buffer->buffer_start_address = 96;
        printf("RX Buffer:%d %d%d%d%d%d%d%d%d\r\n", can_rx_buffer->buffer_start_address, can_rx_buffer->data0, can_rx_buffer->data1, can_rx_buffer->data2, can_rx_buffer->data3, can_rx_buffer->data4, can_rx_buffer->data5, can_rx_buffer->data6, can_rx_buffer->data7);
        //printf("TX Buffer %d\r\n", can_rx_buffer->data2);
        can_transmit_message(can_tx_buffer);
        _delay_ms(500);
        can_receive_message(can_rx_buffer);
        printf("TX Buffer:%d %d%d%d%d%d%d%d%d\r\n", can_rx_buffer->buffer_start_address, can_tx_buffer->data0, can_tx_buffer->data1, can_tx_buffer->data2, can_tx_buffer->data3, can_tx_buffer->data4, can_tx_buffer->data5, can_tx_buffer->data6, can_tx_buffer->data7);
        printf("RX Buffer:%d %d%d%d%d%d%d%d%d\r\n", can_rx_buffer->buffer_start_address, can_rx_buffer->data0, can_rx_buffer->data1, can_rx_buffer->data2, can_rx_buffer->data3, can_rx_buffer->data4, can_rx_buffer->data5, can_rx_buffer->data6, can_rx_buffer->data7);
        //printf("RX Buffer %d\r\n", can_tx_buffer->data2);
        _delay_ms(1000);
        printf("\r\n");

        _delay_ms(1000);


        /*
        send_can_message(0b00000000);
        receive_can_message(0b01100110);
        _delay_ms(500);
        send_can_message(0b11111111);
        receive_can_message(0b01100110);
        _delay_ms(500);
        */
        /*
        send_can_message(0b00000000);
        printf("Status: %d\r\n", can_read_status());
        _delay_ms(500);
        send_can_message(0b11111111);
        printf("Status: %d\r\n", can_read_status());
        _delay_ms(500);
        */
    }

    free(adc_values);
    free(xy_saturation);
    //free(menu_entries);
    free(menu);
    free(can_tx_buffer);
    free(can_rx_buffer);
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