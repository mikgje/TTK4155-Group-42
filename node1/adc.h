uint8_t pwm_init();
void adc_read(uint8_t* adc_values);
void joystick_configuration(uint8_t* xy_saturation, uint8_t* adc_values);
struct can_message;
void adc_transmit(uint8_t* adc_values, uint8_t* xy_saturation, struct can_message* tx_buffer);