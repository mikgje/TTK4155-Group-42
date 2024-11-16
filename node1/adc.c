#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include "adc.h"
#include "util.h"
#include "uart.h"
#include "xmem.h"
#include "can.h"

uint8_t pwm_init(void) {
    /* Configure PD5 as output and PB0, PB1 and PD4 as input */
    /* PD5 is used as a PWM CLK signal */
    set_bit(DDRD, DDD5);
    /* PD4 is used as a busy signal */
    clear_bit(DDRD, DDD4);
    /* Set PB0 and PB1 as input */
    clear_bit(DDRB, DDB0);  
    clear_bit(DDRB, DDB1);
    /* Set fast PWM mode, choose ICR1 as TOP */
    set_bit(TCCR1A, WGM11);
    clear_bit(TCCR1A, WGM10);
    /* Clear output on compare match, set output on TOP reached */
    set_bit(TCCR1A, COM1A1);
    clear_bit(TCCR1A, COM1A0);
    /* Set fast PWM mode with ICR1 as TOP */
    set_bit(TCCR1B, WGM13);
    set_bit(TCCR1B, WGM12);
    /* Configure clock source */
    set_bit(TCCR1B, CS10);
    clear_bit(TCCR1B, CS11);
    clear_bit(TCCR1B, CS12);
    /* Set the register for top value */
    ICR1 = 3;
    OCR1A = 1;

    return 0;
}

void adc_read(uint8_t* adc_values) {
    volatile char* ADC_BASE_ADR;
    uint8_t value;
    /* Set start address for the ADC */
    ADC_BASE_ADR = (char*) 0x1400;
    ADC_BASE_ADR[0] = 0;
    /* Wait until PD4 is set  */
    loop_until_bit_is_set(PIND, PIND4);
    /* Get values, consecutive rd pulses reads new registers */
    for (uint8_t i = 0; i < 4; ++i) {
        value = ADC_BASE_ADR[0];
        adc_values[i] = value;
    }
    /* Read button values inputted on PB0 and PB1 */
    adc_values[4] = test_bit(PINB, PINB0);
    adc_values[5] = test_bit(PINB, PINB1);
}

void joystick_configuration(uint8_t* xy_saturation, uint8_t* adc_values) {
    printf("\r\nStarting configuration. Press any key to continue.\r\n");
    uart_receive(NULL);
    _delay_ms(500);
    printf("Hold joystick to the far left and then press any key.\r\n");
    uart_receive(NULL);
    adc_read(adc_values);
    _delay_ms(500);
    xy_saturation[0] = adc_values[1];
    printf("%d ", adc_values[1]);
    _delay_ms(500);
    printf("Hold joystick to the far right and then press any key.\r\n");
    uart_receive(NULL);
    adc_read(adc_values);
    _delay_ms(500);
    xy_saturation[1] = adc_values[1];
    printf("%d ", adc_values[1]);
    _delay_ms(500);
    printf("Hold joystick to the far bottom and then press any key.\r\n");
    uart_receive(NULL);
    adc_read(adc_values);
    _delay_ms(500);
    xy_saturation[2] = adc_values[0];
    printf("%d ", adc_values[0]);
    _delay_ms(500);
    printf("Hold joystick to the far top and then press any key.\r\n");
    uart_receive(NULL);
    adc_read(adc_values);
    _delay_ms(500);
    xy_saturation[3] = adc_values[0];
    printf("%d ", adc_values[0]);
    _delay_ms(500);
    printf("Configuration complete.\r\n");
}

void adc_transmit(uint8_t* adc_values, uint8_t* xy_saturation, struct can_message* tx_buffer) {
    adc_read(adc_values);
    tx_buffer->data0 = adc_values[0];
    tx_buffer->data1 = adc_values[1];
    tx_buffer->data2 = adc_values[2];
    tx_buffer->data3 = adc_values[3];
    tx_buffer->data4 = adc_values[4];
    tx_buffer->data5 = adc_values[5];
    tx_buffer->data6 = xy_saturation[2];
    tx_buffer->data7 = xy_saturation[3];
    can_transmit_message(tx_buffer);
}
