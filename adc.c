#include "adc.h"


#define set_bit(reg, bit) (reg |= (1 << bit))    // set bit
#define clear_bit(reg, bit) (reg &= ~(1 << bit)) // clear bit
#define test_bit(reg, bit) (reg & (1 << bit))    // read bit
#define loop_until_bit_is_set(reg, bit) while(!test_bit(reg, bit))
#define loop_until_bit_is_clear(reg, bit) while(test_bit(reg, bit))

//volatile char *ADC_BASE_ADR = (char *)0x1400; // Start address for the ADC

uint8_t pwm_init() {
    //configures PD5 as output and PB0, PB1 and PD4 as input
	set_bit(DDRD, DDD5); // PWM CLK signal
	clear_bit(DDRD, DDD4); // Busy signal
	clear_bit(DDRB, DDB0); // set pin B0 as input
	clear_bit(DDRB, DDB1); // set pin B1 as input

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

	return 0;
}

void adc_read(uint8_t *adc_values) {
	volatile char *ADC_BASE_ADR = (char*)0x1400; // start address for the ADC
	ADC_BASE_ADR[0] = 0;
	
	loop_until_bit_is_set(PIND, PIND4);
	
	// gets values
	// consecutive rd pulses reads new registers
	for (uint8_t i = 0; i < 4; i++)
	{
		uint8_t value = ADC_BASE_ADR[0];
		adc_values[i] = value;
	}
	adc_values[4] = (bool) test_bit(PINB, PINB0);
	adc_values[5] = (bool) test_bit(PINB, PINB1);
}

void joystick_configuration(uint8_t *xy_saturation, uint8_t *adc_values) {
	printf("\r\nStarting configuration. Press any key to continue.\r\n");
	uart_receive();
	_delay_ms(500);
	printf("Hold joystick to the far left and then press any key.\r\n");
	uart_receive();
	adc_read(adc_values);
	xy_saturation[0] = adc_values[0];
	_delay_ms(500);
	printf("Hold joystick to the far right and then press any key.\r\n");
	uart_receive();
	adc_read(adc_values);
	xy_saturation[1] = adc_values[0];
	_delay_ms(500);
	printf("Hold joystick to the far top and then press any key.\r\n");
	uart_receive();
	adc_read(adc_values);
	xy_saturation[2] = adc_values[1];
	_delay_ms(500);
	printf("Hold joystick to the far bottom and then press any key.\r\n");
	uart_receive();
	adc_read(adc_values);
	xy_saturation[3] = adc_values[1];
	_delay_ms(500);
	printf("Configuration complete.\r\n");
}