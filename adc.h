#define FOSC 4915200UL // Clock Speed
#define F_CPU 4915200 // Clock Speed

#include <avr/io.h>
#include <util/delay.h>

#include <stdint.h>
#include <stdbool.h>

#include "xmem.h"


volatile char *ADC_BASE_ADR;

void pwm_init();
void adc_read(uint8_t *adc_values);
void joystick_configuration(uint8_t *xy_saturation, uint8_t *adc_values);