#include <stdint.h>
#include <stdio.h>
#include "game.h"
#include "time.h"
#include "adc.h"


uint32_t game_counter(struct game* game_ptr, uint32_t* adc_cal_val) {
    uint32_t val;
    val = adc_read();
    if (val < 0.5*(*adc_cal_val)) {
        game_ptr->points += 1;
        printf("Goal! Points: %d, ADC: %d, ADC cal: %d\r\n", game_ptr->points, val, *adc_cal_val);
        time_spinFor(msecs(10000));
    }

    return game_ptr->points;
}