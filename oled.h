#define FOSC 4915200UL // Clock Speed
#define F_CPU 4915200 // Clock Speed

#include <util/delay.h>
#include <stdint.h>
#include <string.h>

void oled_command(uint8_t command);
void oled_data(char data);
uint8_t oled_init();
void oled_set_column_address(uint8_t value);
void oled_set_page_address(uint8_t value);
void oled_set_position(uint8_t column, uint8_t page);
void oled_clear_screen();
void oled_clear_line(uint8_t page);

void oled_draw_character_large(char ascii_character);
void oled_draw_word_large(char *ascii_word);
void oled_draw_character_normal(char ascii_character);
void oled_draw_word_normal(char *ascii_word);
void oled_draw_character_small(char ascii_character);
void oled_draw_word_small(char *ascii_word);

void oled_draw_menu(uint8_t **menu, uint8_t menu_height, uint8_t menu_width);
uint8_t oled_move_menu(uint8_t **menu, uint8_t menu_height, uint8_t menu_width, uint8_t *adc_values);