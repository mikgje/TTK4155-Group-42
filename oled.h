#include <stdint.h>

void oled_command(uint8_t command);
void oled_data(char data);
void oled_init();
void oled_set_column_address(uint8_t value);
void oled_set_page_address(uint8_t value);
void oled_clear_screen();