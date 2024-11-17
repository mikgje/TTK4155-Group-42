#include <stdint.h>
#include <stdio.h>
#include "decoder.h"
#include "sam.h"

#define TC2 ((TC_REGS*)0x40088000)

/* TC channel control register */
#define CLKEN 1
#define SWTRG 1 /* Maybe possible 0? */

/* TC channel mode register */
#define TCCLKS 5

/* TC block mode register */
#define QDEN 1
#define POSEN 1
#define EDGPHA 1 /* Maybe possible 0? */

/* TC write protection mode register */
#define WPKEY 0x54494D


void decoder_init(void) {
    /* Enable TC2 peripheral clock */
    REG_PMC_PCER1 = PMC_PCER1_PID33; 

    /* Set PC25 and PC26 as peripheral B */
    PIOC->PIO_ABSR |= PIO_ABSR_P26;
    PIOC->PIO_ABSR |= PIO_ABSR_P25;
    /* Disable PIO, enable peripheral, to control the corresponding pin */
    PIOC->PIO_PDR |= PIO_PDR_P25;
    PIOC->PIO_PDR |= PIO_PDR_P26;

    /* Disable write protection */
    TC2->TC_WPMR = (WPKEY << 8) | (0);
    if (TC2->TC_WPMR & (1)) {
        printf("ERROR: Failed to disable write protection");
    }

    TC2->TC_CH0.TC_CCR = (SWTRG << 2) | (CLKEN);
    TC2->TC_CH0.TC_CMR = (TCCLKS);
    TC2->TC_BMR = (EDGPHA << 12) | (POSEN << 9) | (QDEN << 8);

    /* Enable write protection */
    TC2->TC_WPMR = (WPKEY << 8) | (1);
    if (TC2->TC_WPMR & (0)) {
        printf("ERROR: Failed to enable write protection");
    }

}

uint32_t decoder_read(void) {
    /* Read position data */
    return TC2->TC_CH0.TC_CV;
}
