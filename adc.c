#include "adc.h"



#define set_bit(reg, bit) (reg |= (1 << bit))
#define clear_bit(reg, bit) (reg &= ~(1 << bit))
#define test_bit(reg, bit) (reg & (1 << bit))
#define loop_until_bit_is_set(reg, bit) while(!test_bit(reg, bit))
#define loop_until_bit_is_clear(reg, bit) while(test_bit(reg, bit))

volatile char *ADC_BASE_ADR = (char *)0x1400; // Start address for the ADC

void pwm_init() {
    //configures PD5 as output and PB0, PB1 and PD4 as input
	set_bit(DDRD, DDD5);
	clear_bit(DDRD, DDD4);
	clear_bit(DDRB, DDB0);
	clear_bit(DDRB, DDB1);

	//set fast PWM mode, choose ICR1 as TOP
	set_bit(TCCR1A, WGM11);
	clear_bit(TCCR1A, WGM10);

	// clear on compare match, set on TOP reached
	set_bit(TCCR1A, COM1A1);
	clear_bit(TCCR1A, COM1A0);

    // sets PWM fast mode with ICR1 as TOP
    set_bit(TCCR1B, WGM13);
    set_bit(TCCR1B, WGM12);
    
    //configure clock source
    set_bit(TCCR1B, CS10);
    clear_bit(TCCR1B, CS11);
    clear_bit(TCCR1B, CS12);
    
    // register for top value
    ICR1 = 3;
    OCR1A = 1;
}

void adc_read(uint8_t* adc_values) {
	volatile char *ADC_BASE_ADR = (char *) 0x1400; // start address for the ADC
	ADC_BASE_ADR[0] = 0;
	
	loop_until_bit_is_set(PIND, PIND4);
	
	// gets values
	// consecutive rd pulses reads new registers
	for (uint8_t i = 0; i < 4; i++)
	{
		uint8_t value = ADC_BASE_ADR[0];
		adc_values[i] = value;
	}
	
	adc_values[4] = test_bit(PINB, PINB0);
	adc_values[5] = test_bit(PINB, PINB1);
}