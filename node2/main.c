#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <stdarg.h>
#include "can.h"
#include "time.h"
#include "uart.h"
#include "sam.h"
#include "servo.h"
#include "pwm.h"
#include "adc.h"
#include "game.h"
#include "decoder.h"
#include "motor.h"
#include "solenoid.h"

#define BAUDRATE 9600
#define F_CPU 84000000

/* Approximate time width in ms */
#define delta 20

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"

/* Use sam/sam3x/include/pio/pio_sam3x8e.h to find PIO definitions */

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer++++++++++++++++++++++++++++++++++

    //Uncomment after including uart above
    uart_init(F_CPU, BAUDRATE);
    printf("\rHello World\r\n");

    can_init((CanInit){.brp = 41, .phase1 = 5, .phase2 = 6, .propag = 1}, 0);

    pwm_init();
    adc_init();
    decoder_init();
    motor_init();
    solenoid_init();

    CanMsg tx_message = (CanMsg) {
       .id = 1,
       .length = 8,
       .byte8.bytes[0] = 0,
       .byte8.bytes[1] = 0,
       .byte8.bytes[2] = 0,
       .byte8.bytes[3] = 0,
       .byte8.bytes[4] = 0,
       .byte8.bytes[5] = 0,
       .byte8.bytes[6] = 0,
       .byte8.bytes[7] = 0,
   };

    CanMsg* rx_message = malloc(sizeof(CanMsg));
    struct game* game_ptr = malloc(sizeof(struct game));
        game_ptr->points = 0;

    uint32_t* adc_calibration_value = malloc(sizeof(uint32_t));
    adc_calibrate(adc_calibration_value);

    int32_t proportional_gain;
    float integral_gain;
    int32_t last_error;
    float error_integral;
    float delta_time;
    struct controller *controller_ptr = malloc(2*sizeof(int32_t) + 3*sizeof(float));
        controller_ptr->proportional_gain = 5;
        controller_ptr->integral_gain = 500;
        controller_ptr->last_error = 0;
        controller_ptr->error_integral = 0;
        controller_ptr->delta_time = ((float)(delta))/1000;

    while (1)
    {   
        solenoid_reset();
        can_rx(rx_message);
        servo_control(rx_message);
        tx_message.byte8.bytes[0] = game_counter(game_ptr, adc_calibration_value);
        can_tx(tx_message);
        motor_controller(controller_ptr, rx_message);
        solenoid_control(rx_message);
        time_spinFor(msecs(delta));
    }
}