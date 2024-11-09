#include <stdio.h>
#include <stdint.h>

#ifndef GAME_H
#define GAME_H

struct game {
    uint32_t points;
};

void game_counter(struct game* game_ptr);

#endif
