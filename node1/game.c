#include <stdint.h>
#include "game.h"
#include "oled.h"

void game_init(struct menu* menu) {
    oled_draw_menu(menu);
}

void game(struct game* game, struct menu* menu, struct menu* credits, uint8_t* adc_values, uint8_t* xy_saturation, struct can_message* tx_buffer) {
    switch (game->status) {
        case 0:
            printf("0");
            switch(oled_move_menu(menu, adc_values)) {
                case 1:
                    game->status = 1;
                    /* Start game */
                    oled_clear_screen();
                    break;
                case 2:
                    game->status = 2;
                    /* Calibrate */
                    break;
                case 3:
                    game->status = 3;
                    /* Show credits */
                    oled_clear_screen();
                    oled_draw_menu(credits);
                    break;
            }
            adc_read(adc_values);
            break;
        case 1:
            printf("1");
            /* Display points */
            if(adc_values[5]) {
                game->status = 0;
                oled_clear_screen();
                game_init(menu);
            }
            adc_transmit(adc_values, xy_saturation, tx_buffer);
            break;
        case 2:
            printf("2");
            /* Calibrate */
            if(adc_values[5]) {
                game->status = 0;
                oled_clear_screen();
                game_init(menu);
            }
            adc_read(adc_values);
            break;
        case 3:
            printf("3");
            if(adc_values[5]) {
                game->status = 0;
                oled_clear_screen();
                game_init(menu);
            }
            adc_read(adc_values);
            break;
    }
}