#include "quadrature_decoder.h"
#include "sam.h"

void init_quadrature_decoder() {
    REG_PMC_PCER0 = PMC_PCER1_PID33; // Enable TC2 peripheral clock

    /* Configure TC2 BMR register for quadrature decoding */
    TC2->TC_BMR = TC_BMR_QDEN         // Enable Quadrature Decoder
            | TC_BMR_POSEN            // Enable position measurement
            | (0 << 10)               // Disable speed measurement
            | (0 << 11)               // Full quadrature decoding logic active
            | (0 << 12)               // Edge detected on PHA only
            | (0 << 13)               // PHA is not inverted
            | (0 << 14)               // PHB is not inverted
            | (0 << 15)               // IDX is not inverted
            | (0 << 16)               // No swap between PHA and PHB
            | (0 << 17)               // IDX pin is TIOA1
            | (0 << 24);              // No filter applied


    /* Enable Channel 0 */
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

void read_motor_position() {
    /* Read position data */
    uint32_t position = TC2->TC_CHANNEL[0].TC_CV;

    /* Print or log values for verification */
    printf("Position: %lu\n", position);
}
