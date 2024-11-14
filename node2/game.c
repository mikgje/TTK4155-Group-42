#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "time.h"
#include "adc.h"

void game_counter(struct game* game_ptr) {
    uint32_t val;
    val = adc_read();
    //printf("ADC: %d\r\n", val);
    if (val < 3300) {
        game_ptr->points += 1;
        printf("Goal! Points: %d\n\r", game_ptr->points);
        time_spinFor(msecs(10000));
    }
}