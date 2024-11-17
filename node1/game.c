#include <stdint.h>
#include <stdio.h>
#include "game.h"
#include "oled.h"
#include "adc.h"
#include "can.h"

void game_init(struct menu* menu) {
    /* Draw the main menu */
    oled_draw_menu(menu);
}

void game(struct game* game, struct menu* menu, struct menu* play, struct menu* credits, uint8_t* adc_values, uint8_t* xy_saturation, struct can_message* tx_buffer, struct can_message* rx_buffer) {
    switch (game->status) {
        case 0:
            /* Display start menu */
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
                    /* Display credits */
                    oled_clear_screen();
                    oled_draw_menu(credits);
                    break;
            }
            adc_read(adc_values);
            break;
        case 1:
            /* Retrieve points from node 2 */
            can_receive_message(rx_buffer);
            /* 48 is an offset, as 0 is at ASCII value 48 */
            play->option_struct.option2[8] = 48 + rx_buffer->data0;
            /* Display points */
            oled_draw_menu(play);
            /* If right button is pressed, go back to main menu */
            if(adc_values[5]) {
                game->status = 0;
                oled_clear_screen();
                game_init(menu);
            }
            /* While playing the game, the ADC values are transmitted to node 2 */
            adc_transmit(adc_values, xy_saturation, tx_buffer);
            break;
        case 2:
            /* 
             * Calibrate
             * No calibration has been implemented
             */
            if(adc_values[5]) {
                game->status = 0;
                oled_clear_screen();
                game_init(menu);
            }
            /* Update the ADC values */
            adc_read(adc_values);
            break;
        case 3:
            /* Credits are displayed while in this state */
            if(adc_values[5]) {
                game->status = 0;
                oled_clear_screen();
                game_init(menu);
            }
            /* Update the ADC values */
            adc_read(adc_values);
            break;
    }
}