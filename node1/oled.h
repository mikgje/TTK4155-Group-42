#include <stdint.h>

#ifndef OLED_H
#define OLED_H

void oled_command(uint8_t command);
void oled_data(char data);
uint8_t oled_init(void);
void oled_set_column_address(uint8_t value);
void oled_set_page_address(uint8_t value);
void oled_set_position(uint8_t column, uint8_t page);
void oled_clear_screen(void);
void oled_clear_line(uint8_t page);

void oled_draw_character_large(char ascii_character);
void oled_draw_word_large(char* ascii_word);
void oled_draw_character_normal(char ascii_character);
void oled_draw_word_normal(char* ascii_word);
void oled_draw_character_small(char ascii_character);
void oled_draw_word_small(char* ascii_word);

typedef struct {
    char* option0;
    char* option1;
    char* option2;
    char* option3;
} options;

struct menu {
    union {
        options option_struct;
        char* option_array[4];
    };
    uint8_t current_position;
};

void oled_draw_menu(struct menu* menu_ptr);
//void oled_draw_menu(void);
uint8_t oled_move_menu(struct menu* menu_ptr, uint8_t* adc_values);

#endif