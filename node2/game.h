#include <stdint.h>

#ifndef GAME_H
#define GAME_H

struct game {
    uint32_t points;
};

uint32_t game_counter(struct game* game_ptr, uint32_t* adc_cal_val);

#endif
