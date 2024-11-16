#include <stdint.h>

#ifndef SPI_H
#define SPI_H

uint8_t spi_init(void);
void spi_transmit(uint8_t data);
uint8_t spi_receive(void);

#endif