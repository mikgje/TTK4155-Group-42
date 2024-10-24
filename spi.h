#include <stdint.h>

uint8_t spi_init(void);
void spi_transmit(uint8_t data);
uint8_t spi_receive(void);
