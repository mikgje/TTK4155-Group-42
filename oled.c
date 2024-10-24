#include <util/delay.h>
#include <stdint.h>
//#include <string.h>
#include "oled.h"
#include "fonts.h"

#define BASE_OLED_COMMAND_ADDRESS 0x1000
#define BASE_OLED_DATA_ADDRESS 0x1200

void oled_command(uint8_t command) {
    volatile char* oled_command_mem;
    oled_command_mem = (char*) BASE_OLED_COMMAND_ADDRESS;
    oled_command_mem[0] = command;
}

void oled_data(char data) {
    volatile char* oled_data_mem;
    oled_data_mem = (char*) BASE_OLED_DATA_ADDRESS;
    oled_data_mem[0] = data;
}

uint8_t oled_init(void) {
    /* Commands */
    oled_command(0xae); /* Display off */
    oled_command(0xa1); /* Segment remap */
    oled_command(0xda); /* Common pads hardware: alternative */
    oled_command(0x12);
    oled_command(0xc8); /* Common output scan direction:com63~com0 */
    oled_command(0xa8); /* Multiplex ration mode:63 */
    oled_command(0x3f);
    oled_command(0xd5); /* Display divide ratio/osc. freq. mode */
    oled_command(0x80);
    oled_command(0x81); /* Contrast control */
    oled_command(0x50);
    oled_command(0xd9); /* Set pre-charge period */
    oled_command(0x21);
    oled_command(0x20); /* Set Memory Addressing Mode */
    oled_command(0x02);
    oled_command(0xdb); /* VCOM deselect level mode */
    oled_command(0x30);
    oled_command(0xad); /* Master configuration */
    oled_command(0x00);
    oled_command(0xa4); /* Out follows RAM content */
    oled_command(0xa6); /* Set normal display */
    oled_command(0xaf); /* Display on */

    return 0;
}

void oled_set_column_address(uint8_t value) {
    uint8_t higher_nibble, lower_nibble;
    /* Lower and higher nibble value can be [0,15] */
    higher_nibble = (value >> 4);
    lower_nibble = ((value << 4) >> 4);
    oled_command(lower_nibble);
    oled_command(0b00010000 + higher_nibble);
}

/* Value can be [0,7] */
void oled_set_page_address(uint8_t value) { 
    oled_command(0b10110000 + value);
}

void oled_set_position(uint8_t column, uint8_t page) {
    oled_set_column_address(column);
    oled_set_page_address(page);
}

void oled_clear_screen(void) {
    oled_set_column_address(0);
    for (uint8_t i = 0; i<=7; ++i) {
        oled_set_page_address(i);
        for (uint8_t i = 0; i<=127; ++i) {
            oled_data(0);
        }
    }
}

void oled_clear_line(uint8_t page) {
    oled_set_page_address(page);
    for (uint8_t i = 0; i<= 127; ++i) {
        oled_data(0);
    }
}

void oled_draw_character_large(char ascii_character) {
    for (uint8_t i = 0; i<8; ++i) {
        oled_data(pgm_read_byte(&(font8[ascii_character - 32][i])));
    }
}

void oled_draw_word_large(char* ascii_word) {
    for (uint8_t i = 0; i<strlen(ascii_word); ++i) {
        oled_draw_character_large(ascii_word[i]);
    }
}

void oled_draw_character_normal(char ascii_character) {
    for (uint8_t i = 0; i<5; ++i) {
        oled_data(pgm_read_byte(&(font5[ascii_character - 32][i])));
    }
}

void oled_draw_word_normal(char* ascii_word) {
    for (uint8_t i = 0; i<strlen(ascii_word); ++i) {
        oled_draw_character_normal(ascii_word[i]);
    }
}

void oled_draw_character_small(char ascii_character) {
    for (uint8_t i = 0; i<4; ++i) {
        oled_data(pgm_read_byte(&(font4[ascii_character - 32][i])));
    }
}

void oled_draw_word_small(char* ascii_word) {
    for (uint8_t i = 0; i<strlen(ascii_word); ++i) {
        oled_draw_character_small(ascii_word[i]);
    }
}

void oled_draw_menu(uint8_t** menu, uint8_t menu_height, uint8_t menu_width) {
    for (uint8_t i = 0; i<menu_height; ++i) {
        oled_set_position(0, i);
        oled_draw_word_large(menu[i]);
    }
}

uint8_t oled_move_menu(uint8_t** menu, uint8_t menu_height, uint8_t menu_width, uint8_t* adc_values) {
    uint8_t current_position = 0;
    for (uint8_t i = 0; i<menu_height; ++i) {
        if (menu[i][0] == '>') {
            current_position = i;
        }
    }
    //printf("Position: %d. Joy: %d\r\n", current_position, adc_values[0]);
    if (adc_values[0] > 180) {
        if (current_position != 0) {
            //printf("Valid position: %d. J: %d\r\n", current_position, adc_values[0]);
            menu[current_position][0] = ' ';
            current_position -= 1;
            menu[current_position][0] = '>';
        }
    } else if (adc_values[0] < 130) {
        if (current_position != 7) {
            //printf("Valid position: %d. J: %d\r\n", current_position, adc_values[0]);
            menu[current_position][0] = ' ';
            current_position += 1;
            menu[current_position][0] = '>';
        }
    }
    oled_draw_menu(menu, menu_height, menu_width);
    _delay_ms(50);
    if (adc_values[4] == 1) {
        return current_position;
    } else {
        return 255;
    }
}
