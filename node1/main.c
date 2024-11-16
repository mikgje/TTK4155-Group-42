#include <avr/io.h>
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
#include "can.h"
#include "game.h"

#define BAUD 9600UL
#define MYUBRR (FOSC/16/BAUD - 1)

#define GAME_NAME "Ping Pong"
#define OPTION_1 "Play game"
#define OPTION_2 "Calibrate"
#define OPTION_3 "Credits"

#define CREDITS_NAME "Group 42"
#define NAME_1 "Mikael Gjersoee"
#define NAME_2 "Ole Helmers"
#define NAME_3 ""

int main(void) {
    uart_init(MYUBRR);
    xmem_init();
    pwm_init();
    oled_init();
    spi_init();
    can_init();

    fdevopen((int(*) (char, FILE*))&uart_transmit, (int(*) (FILE*))&uart_receive);

    uint8_t* adc_values = malloc(6*sizeof(uint8_t));
    uint8_t* xy_saturation = malloc(4*sizeof(uint8_t)); /* in order: x_min, x_max, y_min, y_max */

    oled_clear_screen();

    char* menu_option0 = malloc(sizeof(GAME_NAME));
    char* menu_option1 = malloc(sizeof(OPTION_1));
    char* menu_option2 = malloc(sizeof(OPTION_2));
    char* menu_option3 = malloc(sizeof(OPTION_3));
    menu_option0 = GAME_NAME;
    menu_option1 = OPTION_1;
    menu_option2 = OPTION_2;
    menu_option3 = OPTION_3;
    struct menu* menu_ptr = malloc(sizeof(struct menu));
    menu_ptr->option_struct.option0 = menu_option0;
    menu_ptr->option_struct.option1 = menu_option1;
    menu_ptr->option_struct.option2 = menu_option2;
    menu_ptr->option_struct.option3 = menu_option3;
    menu_ptr->current_position = 1;

    char* credits_option0 = malloc(sizeof(CREDITS_NAME));
    char* credits_option1 = malloc(sizeof(NAME_1));
    char* credits_option2 = malloc(sizeof(NAME_2));
    char* credits_option3 = malloc(sizeof(NAME_3));
    credits_option0 = CREDITS_NAME;
    credits_option1 = NAME_1;
    credits_option2 = NAME_2;
    credits_option3 = NAME_3;
    struct menu* credits_ptr = malloc(sizeof(struct menu));
    credits_ptr->option_struct.option0 = credits_option0;
    credits_ptr->option_struct.option1 = credits_option1;
    credits_ptr->option_struct.option2 = credits_option2;
    credits_ptr->option_struct.option3 = credits_option3;
    credits_ptr->current_position = 1;

    can_config();
    can_configure_transmit();
    can_configure_receive();
    can_configure_filters_and_masks();

    //can_set_loopback();
    can_set_normal();

    struct can_message *can_tx_buffer = malloc(12*sizeof(uint8_t));
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

    struct can_message *can_rx_buffer = malloc(12*sizeof(uint8_t));
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

    struct game* game_ptr = malloc(sizeof(struct game));
    game_ptr->status = 0;

    game_init(menu_ptr);
    while(1) {
        game(game_ptr, menu_ptr, credits_ptr, adc_values, xy_saturation, can_tx_buffer);
        _delay_ms(20);
    }
}
