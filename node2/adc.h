#include <stdint.h>

#ifndef ADC_H
#define ADC_H

typedef struct {
    uint32_t ADC_CR;
    uint32_t ADC_MR;
    uint32_t ADC_SEQR1;
    uint32_t ADC_SEQR2;
    uint32_t ADC_CHER;
    uint32_t ADC_CHDR;
    uint32_t ADC_CHSR;
    uint32_t RESERVED0[1];
    uint32_t ADC_LCDR;
    uint32_t ADC_IER;
    uint32_t ADC_IDR;
    uint32_t ADC_IMR;
    uint32_t ADC_ISR;
    uint32_t RESERVED1[2];
    uint32_t ADC_OVER;
    uint32_t ADC_EMR;
    uint32_t ADC_CWR;
    uint32_t ADC_CGR;
    uint32_t ADC_COR;
    uint32_t ADC_CDR0;
    uint32_t ADC_CDR1;
    uint32_t ADC_CDR2;
    uint32_t ADC_CDR3;
    uint32_t ADC_CDR4;
    uint32_t ADC_CDR5;
    uint32_t ADC_CDR6;
    uint32_t ADC_CDR7;
    uint32_t ADC_CDR8;
    uint32_t ADC_CDR9;
    uint32_t ADC_CDR10;
    uint32_t ADC_CDR11;
    uint32_t ADC_CDR12;
    uint32_t ADC_CDR13;
    uint32_t ADC_CDR14;
    uint32_t ADC_CDR15;
    uint32_t RESERVED2[1];
    uint32_t ADC_ACR;
    uint32_t RESERVED3[19];
    uint32_t ADC_WPMR;
    uint32_t ADC_WPSR; 
} ADC_REGS;

void adc_init(void);
uint32_t adc_read(void);
void adc_calibrate(uint32_t* adc_val);

#endif