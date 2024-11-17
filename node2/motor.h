#include <stdint.h>
#include <float.h>
#include "can.h"

#ifndef MOTOR_H
#define MOTOR_H

void motor_init(void);
void motor_set_duty_cycle(int32_t duty_cycle);
uint32_t motor_joystick_control_duty_cycle(uint32_t joystick_val);
void motor_set_direction_and_speed(uint32_t direction, uint32_t duty_cycle);
void motor_joystick_control(CanMsg* rx_message);

struct controller {
    int32_t proportional_gain;
    float integral_gain;
    int32_t last_error;
    float error_integral;
    float delta_time;
};

void motor_controller(struct controller* controller, CanMsg* rx_message);

#endif