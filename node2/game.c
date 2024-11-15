#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "time.h"
#include "adc.h"

void game_counter(struct game* game_ptr, uint32_t* adc_cal_val) {
    uint32_t val;
    val = adc_read();
    //printf("ADC: %d, ADC cal: %d\r\n", val, *adc_cal_val);
    if (val < 0.5*(*adc_cal_val)) {
        game_ptr->points += 1;
        printf("Goal! Points: %d, ADC: %d, ADC cal: %d\r\n", game_ptr->points, val, *adc_cal_val);
        time_spinFor(msecs(10000));
    }
}