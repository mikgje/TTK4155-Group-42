#include "oled.h"

#define BASE_OLED_COMMAND_ADDRESS 0x1000
#define BASE_OLED_DATA_ADDRESS 0x1200

void oled_command(uint8_t command) {
    volatile char *oled_command_mem = (char*)BASE_OLED_COMMAND_ADDRESS;
    oled_mem[0] = command;
}

void oled_data(char data) {
    volatile char *oled_data_mem = (char*)BASE_OLED_DATA_ADDRESS;
    oled_mem[0] = data;
}

void oled_init() {
    // Command
    oled_command(0xae); // display off
    oled_command(0xa1); // segment remap
    oled_command(0xda); // common pads hardware: alternative
    oled_command(0x12);
    oled_command(0xc8); // common output scan direction:com63~com0
    oled_command(0xa8); // multiplex ration mode:63
    oled_command(0x3f);
    oled_command(0xd5); // display divide ratio/osc. freq. mode
    oled_command(0x80);
    oled_command(0x81); // contrast control
    oled_command(0x50);
    oled_command(0xd9); // set pre-charge period
    oled_command(0x21);
    oled_command(0x20); // set Memory Addressing Mode
    oled_command(0x02);
    oled_command(0xdb); // VCOM deselect level mode
    oled_command(0x30);
    oled_command(0xad); // master configuration
    oled_command(0x00);
    oled_command(0xa4); // out follows RAM content
    oled_command(0xa6); // set normal display
    oled_command(0xaf); // display on
}

