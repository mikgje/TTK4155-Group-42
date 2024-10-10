#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "spi.h"

#define set_bit(reg, bit) (reg |= (1 << bit))    // set bit
#define clear_bit(reg, bit) (reg &= ~(1 << bit)) // clear bit
#define test_bit(reg, bit) (reg & (1 << bit))    // read bit
#define loop_until_bit_is_set(reg, bit) while(!test_bit(reg, bit))
#define loop_until_bit_is_clear(reg, bit) while(test_bit(reg, bit))

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

void spi_init() {
    /*set MOSI and SCK output, all others input*/
    //DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
    set_bit(DDRSPI, DDMOSI);
    set_bit(DDRSPI, DDSCK);
    /*enable SPI, Master, set clock rate fck/16*/
    //SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
    set_bit(SPCR, MSTR);
    set_bit(SPCR, SPR0);
    /*set data order msb transferred first*/
    clear_bit(SPCR, DORD);
}


void spi_transmit(uint8_t cData) {
    /*start transmission*/
    SPDR = cData;
    /*wait for transmission to complete*/
    while(!(SPSR & (1<<SPIF)));
}

uint8_t spi_receive() {
    /*transmit junk byte*/
    spi_transmit(255);
    return SPDR;
}

uint8_t spi_read() {
    return SPDR;
}