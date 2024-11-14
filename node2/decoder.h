#include <stdint.h>

#ifndef DECODER_H
#define DECODER_H

void decoder_init(void);
uint32_t decoder_read(void);

typedef struct {
    uint32_t TC_CCR;
    uint32_t TC_CMR;
    uint32_t TC_SMMR;
    uint32_t RESERVED0[1];
    uint32_t TC_CV;
    uint32_t TC_RA;
    uint32_t TC_RB;
    uint32_t TC_RC;
    uint32_t TC_SR;
    uint32_t TC_IER;
    uint32_t TC_IDR;
    uint32_t TC_IMR;
    uint32_t RESERVED1[4];
} TC_CH;

typedef struct {
    TC_CH TC_CH0;
    TC_CH TC_CH1;
    TC_CH TC_CH2;
    uint32_t TC_BCR;
    uint32_t TC_BMR;
    uint32_t TC_QIER;
    uint32_t TC_QIDR;
    uint32_t TC_QIMR;
    uint32_t TC_QISR;
    uint32_t TC_FMR;
    uint32_t RESERVED0[2];
    uint32_t TC_WPMR;
} TC_REGS;

#endif