#ifndef UTIL_H
#define UTIL_H

#define set_bit(reg, bit) (reg |= (1 << bit))   /* set bit */
#define clear_bit(reg, bit) (reg &= ~(1 << bit))    /* clear bit */
#define test_bit(reg, bit) (reg & (1 << bit))   /* read bit */

#endif