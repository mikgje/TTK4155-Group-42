#include <stdint.h>
#include <float.h>
#include <stdlib.h>
#include "motor.h"
#include "sam.h"
#include "pwm.h"
#include "decoder.h"
#include "can.h"

#define PWM ((PWM_REGS*)0x40094000)

#define dead_band_center 20
#define dead_band_edge 3
#define joystick_y_center 161

#define max_position 5600

#define max_integral 50000

void motor_init(void) {
    /* Power on PIOB */
    PMC->PMC_PCER0 |= PMC_PCER0_PID13;
    /* Enable PIO to control the corresponding pin */
    PIOC->PIO_PER = PIO_PC23;
    /* Enable output for PC23 */
    PIOC->PIO_OER = PIO_PC23;
}

void motor_set_duty_cycle(int32_t duty_cycle) {
    if (duty_cycle > 20000) {
        duty_cycle = 20000;
    } else if (duty_cycle < 0) {
        duty_cycle = 0;
    }

    PWM->PWM_CH0.PWM_CDTY = duty_cycle;   
}

uint32_t motor_joystick_control_duty_cycle(uint32_t joystick_val) {
    uint32_t dc;
    if (joystick_val < dead_band_edge) {
        joystick_val = 0;
    } else if (joystick_val > (255 - dead_band_edge)) {
        joystick_val = 255;
    }

    if (joystick_val < (joystick_y_center - dead_band_center)) {
        dc = 20000 * (1 - ((float)joystick_val)/(255/2));
        if (dc > 20000) {
            dc = 20000;
        } else if (dc < 2000) {
            dc = 0;
        }
    } else if (joystick_val > (joystick_y_center + dead_band_center)) {
        dc = 20000 * (((float)joystick_val)/(255/2) - 1);
        if (dc > 20000) {
            dc = 20000;
        } else if (dc < 2000) {
            dc = 0;
        }
    } else {
        dc = 0;
    }
    return dc;
}

void motor_set_direction_and_speed(uint32_t direction, uint32_t duty_cycle) {
    if (direction) {
        PIOC->PIO_SODR = PIO_PC23;
    } else {
        PIOC->PIO_CODR = PIO_PC23;
    }
    motor_set_duty_cycle(duty_cycle);
}

void motor_joystick_control(CanMsg* rx_message) {
    uint32_t joystick_y, direction;
    joystick_y = rx_message->byte8.bytes[0];
    if (joystick_y < joystick_y_center) {
        direction = 1;
    } else {
        direction = 0;
    }
    motor_set_direction_and_speed(direction, motor_joystick_control_duty_cycle(joystick_y));
}

void motor_controller(struct controller* controller_ptr, CanMsg* rx_message) {
    uint32_t position_ref, position, direction;
    int32_t error, gain;
    position_ref = 0;
    position = 0;
    direction = 0;
    error = 0;
    gain = 0;

    //printf("Position, PositionRef, Error, ErrorInt, Gain, Direction: %u| %u| %d| %d| %d| %d|\r\n", position, position_ref, error, controller->error_integral, gain, direction);
    
    position_ref = rx_message->byte8.bytes[3] * max_position/255;
    //printf("Position: %d\r\n", position_ref);
    position = decoder_read();
    error = position_ref - position;
    //controller->error_integral += controller->last_error*controller->delta_time + ((error - controller->last_error)/2)*controller->delta_time;
    controller_ptr->error_integral += error;
    if (controller_ptr->error_integral > max_integral) {
        controller_ptr->error_integral = max_integral;
    } else if (controller_ptr->error_integral < -max_integral) {
        controller_ptr->error_integral = -max_integral;
    }
    gain = controller_ptr->proportional_gain*error + (controller_ptr->error_integral * controller_ptr->delta_time);
    //gain = controller->proportional_gain*error;

    if(gain < 0) {
        direction = 1;
    } else {
        direction = 0;
    }
    motor_set_direction_and_speed(direction, abs(gain));

    //printf("Position, PositionRef, Error, ErrorInt, Gain, Direction: %u| %u| %d| %f| %d| %u|\r\n", position, position_ref, error, controller_ptr->error_integral, gain, direction);

    controller_ptr->last_error = error;
}