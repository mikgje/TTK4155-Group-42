#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "spi.h"

#define set_bit(reg, bit) (reg |= (1 << bit))    // set bit
#define clear_bit(reg, bit) (reg &= ~(1 << bit)) // clear bit
#define test_bit(reg, bit) (reg & (1 << bit))    // read bit
#define loop_until_bit_is_set(reg, bit) while (!test_bit(reg, bit))
#define loop_until_bit_is_clear(reg, bit) while (test_bit(reg, bit))

#define DDRSPI DDRB
#define DDSCK DDB7
#define DDMISO DDB6
#define DDMOSI DDB5
#define DDSS DDB4
#define SPI PORTB
#define PINSCK PINB7
#define PINMISO PINB6
#define PINMOSI PINB5
#define PINSS PINB4

uint8_t spi_init(void) {
    /* Set MOSI and SCK output, all others input */
    set_bit(DDRSPI, DDMOSI);
    set_bit(DDRSPI, DDSCK);
    /* Enable SPI, Master, set clock rate fck/16 */
    set_bit(SPCR, MSTR);
    set_bit(SPCR, SPR0);
    /* Set data order msb transferred first */
    clear_bit(SPCR, DORD);

    return 0;
}


void spi_transmit(uint8_t data) {
    /* Start transmission */
    SPDR = data;
    /* Wait for transmission to complete */
    while (!(SPSR & (1<<SPIF)));
}

uint8_t spi_receive(void) {
    /* Transmit junk byte */
    spi_transmit(255);
    
    return SPDR;
}
