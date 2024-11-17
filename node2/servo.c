#include "servo.h"
#include "pwm.h"

#define joystick_x_center 157
#define dead_band_center 20

void servo_control(CanMsg* rx_message) {
    uint32_t joystick_x, duty_cycle;
    /* Retrieve the joystick value */
    joystick_x = rx_message->byte8.bytes[1];
    /* Map the joystick value to the duty cycle */
    duty_cycle = 900 + joystick_x * ((2100 - 900)/255);
    /* Set a middle value if the joystick is inside the dead zone */
    if ((joystick_x > joystick_x_center - dead_band_center) && (joystick_x < joystick_x_center + dead_band_center)) {
        duty_cycle = 1500;
    }
    pwm_set_duty_cycle(duty_cycle);
}