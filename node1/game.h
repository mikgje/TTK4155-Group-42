#include <stdint.h>
#include "oled.h"
#include "can.h"

#ifndef GAME_H
#define GAME_H

struct game {
    uint8_t status;
};

void game_init(struct menu* menu);
void game(struct game* game, struct menu* menu, struct menu* play, struct menu* credits, uint8_t* adc_values, uint8_t* xy_saturation, struct can_message* tx_buffer, struct can_message* rx_buffer);

#endif