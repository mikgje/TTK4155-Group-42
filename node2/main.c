#include <stdio.h>
#include <stdarg.h>
#include "can.h"
#include "time.h"
#include "uart.h"
#include "sam.h"
#include "servo.h"
#include "pwm.h"

#define BAUDRATE 9600
#define F_CPU 84000000

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

    CanMsg m = (CanMsg) {
       .id = 1,
       .length = 8,
       .byte8.bytes[0] = 1,
       .byte8.bytes[1] = 2,
       .byte8.bytes[2] = 3,
       .byte8.bytes[3] = 42,
       .byte8.bytes[4] = 19,
       .byte8.bytes[5] = 100,
       .byte8.bytes[6] = 200,
       .byte8.bytes[7] = 53,
   };

    CanMsg* rx_message = malloc(sizeof(CanMsg));

    while (1)
    {   
        /*
        adc_receive(rx_message);
        pwm_set_duty_cycle(900);
        time_spinFor(msecs(5000));
        pwm_set_duty_cycle(2100);
        time_spinFor(msecs(5000));
        */
        servo_control(rx_message);
        time_spinFor(msecs(100));
    }
    
}

/*
 * MCK = 84MHz
 * CAN baudrate = 125 kbit/s -> bit time = 8us
 * Delay of the bus driver: 50ns
 * Delay of the receiver: 30ns
 * Delay of the bus line (20m): 110ns
 *
 * The total number of time quanta in a bit time must be comprised between 8 and 25. Bit time has been chosen in MCP2515 as 16.
 * TQ = T_CSC = 500ns -> for node 2: BRP = 500ns*84MHz - 1 = 41
 */


// /* Power on PIOB */
// PMC->PMC_PCER0 |= PMC_PCER0_PID12;
// /* Enable PIO to control the corresponding pin */
// PIOB->PIO_PER = PIO_PB13;
// /* Enable output for P13 */
// PIOB->PIO_OER = PIO_PB13;

//     /* Set output data register */
// PIOB->PIO_SODR = PIO_PB13;
// time_spinFor(msecs(200));
// /* Clear output data register */
// PIOB->PIO_CODR = PIO_PB13; 
// time_spinFor(msecs(200));