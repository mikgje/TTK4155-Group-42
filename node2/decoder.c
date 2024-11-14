#include "decoder.h"
#include "sam.h"

#define TC ((TC_REGS*)0x40080000)

/* TC channel control register */
#define CLKEN 1
#define CLKDIS 0
#define SWTRG 0
/* TC channel mode register */
#define TCCLKS 0
#define CLKI 0
#define BURST 0
#define LDBSTOP 0
#define LDBDIS 0
#define ETRGEDG 0
#define ABETRG 0
#define CPCTRG 0
#define WAVE 0
#define LDRA 0
#define LDRB 0
/* TC block control register */
#define SYNC 0
/* TC block mode register */
#define QDEN 1
#define POSEN 1
/* TC write protection mode register */
#define WPKEY 0x54494D


void init_decoder(void) {
    REG_PMC_PCER1 = PMC_PCER1_PID33; // Enable TC2 peripheral clock

    /* Disable write protection */
    TC->TC_WPMR = (WPKEY << 8) | (0);
    if (TC->TC_WPMR & (1)) {
        printf("ERROR: Failed to disable write protection");
    }

    TC->TC_CH0.TC_CCR = (SWTRG << 2) | (CLKDIS << 1) | (CLKEN);
    TC->TC_CH0.TC_CMR = (LDRB << 18) | (LDRA << 16) | (WAVE << 15) | (CPCTRG << 14) | (ABETRG << 10) | (ETRGEDG << 8) | (LDBDIS << 0) | (LDBSTOP << 6) | (BURST << 4) | (CLKI << 3) | (TCCLKS);
    TC->TC_BCR = (SYNC);
    TC->TC_BMR = (POSEN << 9) | (QDEN << 8);

    /* Enable write protection */
    TC->TC_WPMR = (WPKEY << 8) | (1);
    if (TC->TC_WPMR & (0)) {
        printf("ERROR: Failed to enable write protection");
    }

/*
    TC2->TC

*/
    /* Configure TC2 BMR register for quadrature decoding */
/*
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


*/
    /* Enable Channel 0 */
/*
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
*/
}

void read_motor_position(void) {
    /* Read position data */
    uint32_t position = TC2->TC_CHANNEL[0].TC_CV;

    /* Print or log values for verification */
    printf("Position: %lu\n", position);
}
