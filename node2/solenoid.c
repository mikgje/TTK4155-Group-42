#include "solenoid.h"
#include "sam.h"
#include "time.h"

void solenoid_init(void) {
    /* Power on PIOB */
    PMC->PMC_PCER0 |= PMC_PCER0_PID13;
    /* Enable PIO to control the corresponding pin */
    PIOC->PIO_PER = PIO_PC24;
    /* Enable output for PC24 */
    PIOC->PIO_OER = PIO_PC24;
}

void solenoid_fire(void) {
    /* The solenoid is active low, clear the pin to release the disc */
    PIOC->PIO_CODR = PIO_PC24;
}

void solenoid_reset(void) {
    /* The solenoid is active low, set the pin to retract the disc */
    PIOC->PIO_SODR = PIO_PC24;
}

void solenoid_controller(CanMsg* rx_message) {
    uint32_t button;
    button = rx_message->byte8.bytes[4];
    if (button) {
        solenoid_fire();
    }
}