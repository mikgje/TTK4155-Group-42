#include "adc.h"
#include "sam.h"

/* We will use channel 0 - AD0 on I/O line PA2, peripheral X1 as described in table 43-3 */

#define ADC ((ADC_REGS*)0x400C0000)

/* ADC control register */
#define START 1 /* Start conversion */

/* ADC mode register */
#define TRG_EN 0 /* Disable hardware trigger*/
#define LOWRES 0 /* 12-bit resolution */
#define SLEEP 0 /* Normal mode: the ADC core and reference voltage circuitry are kept ON between conversions */
#define FREERUN 1 /* Free run mode: no triggers */
#define PRESCAL 0 /* ADCClock = MCK/2 */
#define STARTUP 0 /* Start-up time is SUT0: 0 periods of ADCClock */
#define SETTLING 0 /* Analog settling time is AST3: 3 periods of ADCClock */
#define ANACH 0 /* No analog change on channel switching */
#define USEQ 0 /* Normal mode: channels are in numerical order */

/* ADC channel gain register */
#define GAIN0 0b01

/* ADC channel offset register */
#define OFF0 0 /* No offset for channel 0 */
#define DIFF0 0 /* Channel 0 is used in single-ended mode */

/* ADC write protect mode register */
#define WPKEY 0x414443



void adc_init(void) {
    /* Power on ADC */
    PMC->PMC_PCER1 |= PMC_PCER1_PID37;
    /* Set PA2 as peripheral B */
    PIOA->PIO_ABSR |= PIO_ABSR_P2;

    /* Disable write protect */
    ADC->ADC_WPMR = (WPKEY << 8) | (0);
    if (ADC->ADC_WPSR & (1)) {
        printf("ERROR: A write protect violation has occured");
    }

    /* Configure ADC mode */
    ADC->ADC_MR = (USEQ << 31) | (ANACH << 23) | (SETTLING << 20) | (STARTUP << 16) | (PRESCAL << 8) | (FREERUN << 7) | (SLEEP << 5) | (LOWRES << 4) | (TRG_EN);
    /* Enable ADC channel 0 */
    ADC->ADC_CHER = (1);
    if (!(ADC->ADC_CHSR & (1))) {
        printf("ERROR: Failed to enable ADC channel 0");
    }
    /* Configure channel 0 */
    ADC->ADC_CGR = GAIN0;
    ADC->ADC_COR = (DIFF0 << 16) | (OFF0);
    
    /* Enable write protect */
    ADC->ADC_WPMR = (WPKEY << 8) | (1);
}

uint32_t adc_read(void) {
    ADC->ADC_CR = (START << 1);
    /* Read data, 12 lowest bits, from the ADC last converted data register */
    return (ADC->ADC_LCDR & 0b111111111111);
}