#include <stdint.h>
#include <avr/io.h>

#include "can_controller.h"
#include "spi.h"

#define set_bit(reg, bit) (reg |= (1 << bit))    // set bit
#define clear_bit(reg, bit) (reg &= ~(1 << bit)) // clear bit
#define test_bit(reg, bit) (reg & (1 << bit))    // read bit
#define loop_until_bit_is_set(reg, bit) while(!test_bit(reg, bit))
#define loop_until_bit_is_clear(reg, bit) while(test_bit(reg, bit))

#define DDRSPI DDRB
#define DDSCK DDB7
#define DDMISO DDB6
#define DDMOSI DDB5
#define DDSS DDB4
#define SPI PORTB
#define PINSCK PINB7
#define PINMISO PINB6
#define PINMOSI PINB5
#define PINSS PINB4

/*value written to register must be one less than real value*/
/*
PropSeg + PS1 >= PS2
PropSeg + PS1 >= TDELAY
PS2 > SJW
*/
/*we choose this*/
#define BRP 3
/*sjw is chosen as 0, might be possible to use less*/
#define SJW 0
/*phase segment ps1  must be [1,8]*/
#define PS1 6
/*phase segment ps2 must be [2,8]*/
#define PS2 5
/*propagation segment must be [1,8]*/
#define PRSEG 0
/*synchronization segment must be 1*/
#define SYSEG 0
/*sample once*/
#define SAM 0
/*determine ps2 bit time length from cnf3*/
#define BTLMODE 1
/*if canctrl.clken is 1, 1 for clkout enabled for sof signal, 0 for clkout enabled for clockout function. if canctrl.clken is 0, doesn't matter*/
#define SOF 1
/*wake up filter, 1 enabled, 0 disabled*/
#define WAKFIL 0

void can_reset() {
    /*reset mcp2515*/
    clear_bit(SPI,PINSS);
    spi_transmit(0b11000000);
    set_bit(SPI, PINSS);
}

void can_init() {
    /*set SS output*/
    set_bit(DDRSPI, DDSS); 
    set_bit(SPCR, SPE);

    can_reset();
}

uint8_t can_read(uint8_t address) { 
    clear_bit(SPI, PINSS);

    /*transmit instruction*/
    spi_transmit(0b00000011);
    /*transmit address*/
    spi_transmit(address);
    uint8_t retval = spi_receive();

    set_bit(SPI, PINSS);
    
    return retval;
}

void can_write(uint8_t address, uint8_t data){
    clear_bit(SPI, PINSS);

    /*transmit instruction*/
    spi_transmit(0b00000010);
    /*transmit address*/
    spi_transmit(address);
    /*transmit data*/
    spi_transmit(data);

    set_bit(SPI, PINSS);
}

void can_config() {
    uint8_t CNF1_ADR = 0b00101010;
    uint8_t CNF2_ADR = 0b00101001;
    uint8_t CNF3_ADR = 0b00101000;
    uint8_t CNF1 = 0;
    uint8_t CNF2 = 0;
    uint8_t CNF3 = 0;

    CNF1 = (SJW << 6) | (BRP);
    CNF2 = (BTLMODE << 7) | (SAM << 6) | (PS1 << 3) | (PRSEG);
    CNF3 = (SOF << 7) | (WAKFIL << 6) & ~(0b111 << 3) | (PS2);

    can_write(CNF1_ADR, CNF1);
    can_write(CNF2_ADR, CNF2);
    can_write(CNF3_ADR, CNF3);
}

void can_request_to_send(uint8_t txb0, uint8_t txb1, uint8_t txb2) {
    /*rts for specified tx buffer if value is 1*/
    spi_transmit((0b10000000) | (txb2 << 2) | (txb1 << 1) | (txb0));
}

uint8_t can_read_status() {
    clear_bit(SPI, PINSS);

    /*transmit instruction*/
    spi_transmit(0b10100000);
    /*transmit junk byte*/
    uint8_t retval = spi_receive();
    /*transmit junk byte*/
    spi_transmit(255);

    set_bit(SPI, PINSS);

    return retval;
}

/*refer to mcp2515 table 4-2 and figure 12-1 for usage of mask*/
void can_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
    clear_bit(SPI, PINSS);

    /* send bit modify command*/
    spi_transmit(0b00000101);
    /*transmit address*/
    spi_transmit(address);
    /*transmit mask*/
    spi_transmit(mask);
    /*transmit data*/
    spi_transmit(data);

    set_bit(SPI, PINSS);
}