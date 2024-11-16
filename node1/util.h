#ifndef UTIL_H
#define UTIL_H

/* Clock speeds */
#define FOSC 4915200UL
#define F_CPU 4915200
#include <util/delay.h>

#define set_bit(reg, bit) (reg |= (1 << bit))   /* set bit */
#define clear_bit(reg, bit) (reg &= ~(1 << bit))    /* clear bit */
#define test_bit(reg, bit) (reg & (1 << bit))   /* read bit */

#endif