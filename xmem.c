#include "xmem.h"
#include <stdint.h>
#include <avr/io.h>

#define BASE_ADDRESS 0x1000

uint8_t xmem_init(void) {
    /* Enable XMEM */
    MCUCR |= (1 << SRE);
    /* Mask bits / reduce bus width */
    SFIOR |= (1 << XMM0);

    return 0;
}

void xmem_write(uint8_t data, uint16_t addr) {
    volatile char* ext_mem;
    ext_mem = (char*)BASE_ADDRESS;
    ext_mem[addr] = data;
}

uint8_t xmem_read(uint16_t addr) {
    volatile char *ext_mem = (char*)BASE_ADDRESS;
    uint8_t ret_val = ext_mem[addr];
    return ret_val;
}
