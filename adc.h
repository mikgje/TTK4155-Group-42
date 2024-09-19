#include <stdint.h>
#include <avr/io.h>

#include "xmem.h"

volatile char *ADC_BASE_ADR;

void pwm_init();
void adc_read(uint8_t* adc_values);