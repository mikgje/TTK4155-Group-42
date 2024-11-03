#include <stdint.h>

typedef struct {
    uint32_t PWM_CMR;
    uint32_t PWM_CDTY;
    uint32_t PWM_CDTYUPD;
    uint32_t PWM_CPRD;
    uint32_t PWM_CPRDUPD;
    uint32_t PWM_CCNT;
    uint32_t PWM_DT;
    uint32_t PWM_DTUPD;
} PWM_CH;

typedef struct {
    uint32_t PWM_CLK;
    uint32_t PWM_ENA;
    uint32_t RESERVED0[55];
    uint32_t PWM_WPCR;
    uint32_t RESERVED1[70];
    PWM_CH PWM_CH0;
    PWM_CH PWM_CH1;
    PWM_CH PWM_CH2;
    PWM_CH PWM_CH3;
    PWM_CH PWM_CH4;
    PWM_CH PWM_CH5;
    PWM_CH PWM_CH6;
    PWM_CH PWM_CH7;
} PWM_REGS;

void spi_init();
void pwm_set_duty_cycle(uint32_t dc);
void interupt_timer(uint8_t time);
