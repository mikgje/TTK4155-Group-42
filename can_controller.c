#include <stdint.h>
#include <avr/io.h>
#include "can_controller.h"
#include "spi.h"

#define set_bit(reg, bit) (reg |= (1 << bit))   /* set bit */
#define clear_bit(reg, bit) (reg &= ~(1 << bit))    /* clear bit */
#define test_bit(reg, bit) (reg & (1 << bit))   /* read bit */
#define loop_until_bit_is_set(reg, bit) while (!test_bit(reg, bit))
#define loop_until_bit_is_clear(reg, bit) while (test_bit(reg, bit))

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

/* 
 * Value written to register must be one less than real value
 *
 * PropSeg + PS1 >= PS2
 * PropSeg + PS1 >= TDELAY
 * PS2 > SJW
 */

/* We choose this */
#define BRP 3
/* SJW is chosen as 0, might be possible to use less */
#define SJW 0
/* Phase segment PS1 must be [1,8] */
#define PS1 6
/* Phase segment PS2 must be [2,8] */
#define PS2 5
/* Propagation segment must be [1,8] */
#define PRSEG 0
/* Synchronization segment must be 1 */
#define SYSEG 0
/* Sample once */
#define SAM 0
/* Determine PS2 bit time length from CNF3 */
#define BTLMODE 1
/* If CANCTRL.CLKEN is 1: 1 for CLKOUT enabled for SoF signal, 0 for CLKOUT enabled for clockout function. If CANCTRL.CLKEN is 0, doesn't matter */
#define SOF 1
/* Wake up filter: 1 enabled, 0 disabled */
#define WAKFIL 0

/* Commands */
#define RESET       0b11000000
#define READ        0b00000011
#define WRITE       0b00000010
#define RTS_BASE    0b10000000
#define READ_STATUS 0b10100000
#define BIT_MODIFY  0b00000101

/* CAN controller registers */
#define CNF1        0b00101010
#define CNF2        0b00101001
#define CNF3        0b00101000
#define TXB0CTRL    0b00110000
#define TXB0SIDH    0b00110001
#define TXB0SIDL    0b00110010
#define TXB0DLC     0b00110101
#define RXB0CTRL    0b01100000
#define TXB0CANCTRL 0b00111111
#define RXB0CANCTRL 0b01101111
#define CANINTF     0b00101100
#define RXF0SIDH    0b00000000
#define RXF0SIDL    0b00000001
#define RXF1SIDH    0b00000100
#define RXF1SIDL    0b00000101
#define RXM0SIDH    0b00100000
#define RXM0SIDL    0b00100001

void can_reset(void) {
    /* Reset MCP2515 */
    clear_bit(SPI,PINSS);
    
    spi_transmit(RESET);
    
    set_bit(SPI, PINSS);
}

uint8_t can_init(void) {
    /* Set SS output */
    set_bit(DDRSPI, DDSS); 
    set_bit(SPCR, SPE);
    /* Enter Configuration mode */
    can_reset();
    if ((can_read(0b00001110) & 0b11100000) != 0b10000000) {
        printf("MCP2515 is not in config mode after reset\r\n");  
	
	return 1;
    } 
    
    return 0;
}

uint8_t can_read(uint8_t address) { 
    uint8_t retval;
    clear_bit(SPI, PINSS);
    
    /* Transmit instruction */
    spi_transmit(READ);
    /* Transmit address */
    spi_transmit(address);
    retval = spi_receive();
    
    set_bit(SPI, PINSS);

    return retval;
}

void can_write(uint8_t address, uint8_t data){
    clear_bit(SPI, PINSS);

    /* Transmit instruction */
    spi_transmit(WRITE);
    /* Transmit address */
    spi_transmit(address);
    /* Transmit data */
    spi_transmit(data);

    set_bit(SPI, PINSS);
}

void can_config(void) {
    uint8_t CNF1_data, CNF2_data, CNF3_data;
    /* Set values to default values */
    CNF1_data = 0;
    CNF2_data = 0;
    CNF3_data = 0;
    /* Update values */
    CNF1_data = (SJW << 6) | (BRP);
    CNF2_data = (BTLMODE << 7) | (SAM << 6) | (PS1 << 3) | (PRSEG);
    CNF3_data = (SOF << 7) | (WAKFIL << 6) & ~(0b111 << 3) | (PS2);
    /* Write values to their respective registers */
    can_write(CNF1, CNF1_data);
    can_write(CNF2, CNF2_data);
    can_write(CNF3, CNF3_data);
}

void can_request_to_send(uint8_t txb0, uint8_t txb1, uint8_t txb2) {
    /* RTS for specified TX buffer if value is 1 */
    spi_transmit((RTS_BASE) | (txb2 << 2) | (txb1 << 1) | (txb0));
}

uint8_t can_read_status(void) {
    uint8_t retval;
    clear_bit(SPI, PINSS);

    /* Transmit instruction */
    spi_transmit(READ_STATUS);
    /* Transmit junk byte and read shift register */
    retval = spi_receive();
    /* Transmit junk byte */
    spi_transmit(255);

    set_bit(SPI, PINSS);

    return retval;
}

/* Refer to MCP2515 table 4-2 and figure 12-1 for usage of mask */
void can_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
    clear_bit(SPI, PINSS);

    /* Send bit modify command*/
    spi_transmit(BIT_MODIFY);
    /* Transmit address*/
    spi_transmit(address);
    /* Transmit mask*/
    spi_transmit(mask);
    /* Transmit data*/
    spi_transmit(data);

    set_bit(SPI, PINSS);
}

void can_configure_transmit(void) {
    /* Set TX buffer 0 to have highest message priority */
    can_bit_modify(TXB0CTRL, 0b00000011, 0b00000011);
    /* Set TX buffer 0's ID's high byte as 0 */
    can_write(TXB0SIDH, 0b00000000);
    /* Set TX buffer 0's ID's low byte as 0 and disable extended identifier */ 
    can_write(TXB0SIDL, 0b00000000);
    /* Set TX buffer 0 as a data frame consisting of 8 bytes */
    can_write(TXB0DLC, 0b00001000); 
}

void can_configure_receive(void) {
    /* Receive only valid messages with standard identifiers and that meet the filter criteria */
    can_bit_modify(RXB0CTRL, 0b01100000, 0b01100000);
    /* NB: Currently set to accept all messages */
}

/* Refer to MCP2515 table 4-2 and figure 12-1 for usage of mask */
void can_configure_filters_and_masks(void) {
    /* Filter RXF0 accepts all message IDs, applied to only standard frames */
    can_write(RXF0SIDH, 0b11111111);
    can_write(RXF0SIDL, 0b11100000);
    /* Filter RXF1 accepts all message IDs, applied to only standard frames */
    can_write(RXF1SIDH, 0b11111111);
    can_write(RXF1SIDL, 0b11100000);
    /* Mask RXM0 accepts all message IDs, applied to only standard frames */
    can_write(RXM0SIDH, 0b11111111);
    can_write(RXM0SIDL, 0b11100000);
}

void can_set_loopback(void){
    /* Set CANCTRL relevant register to 010, i.e. Loopback mode */
    /* Set TXB0's CANCTRL to Loopback mode */
    can_bit_modify(TXB0CANCTRL, 0b11100000, 0b01000000);
    /* Set RXB0's CANCTRL to Loopback mode */
    can_bit_modify(RXB0CANCTRL, 0b11100000, 0b01000000);
}

void can_transmit_message(struct can_message* tx_buffer) {
    /* Configure control register, message ID and data length */
    can_configure_transmit();
    /* Write to TXB0's data registers */ 
    can_write((tx_buffer->buffer_start_address)+6, tx_buffer->data0);
    can_write((tx_buffer->buffer_start_address)+7, tx_buffer->data1);
    can_write((tx_buffer->buffer_start_address)+8, tx_buffer->data2);
    can_write((tx_buffer->buffer_start_address)+9, tx_buffer->data3);
    can_write((tx_buffer->buffer_start_address)+10, tx_buffer->data4);
    can_write((tx_buffer->buffer_start_address)+11, tx_buffer->data5);
    can_write((tx_buffer->buffer_start_address)+12, tx_buffer->data6);
    can_write((tx_buffer->buffer_start_address)+13, tx_buffer->data7);
    /* Request to send TXB0 */
    /* Set TXB0CTRL.TXREQ = 1 */
    can_bit_modify(TXB0CTRL, 0b00001000, 0b00001000);
    /* Mark only TXB0 for RTS */
    can_request_to_send(1, 0, 0);
}

void can_receive_message(struct can_message* rx_buffer) {
    /* Configure control register */
    can_configure_receive();
    /* Read RXB0's ID registers */
    rx_buffer->message_id_high = can_read((rx_buffer->buffer_start_address)+1);
    rx_buffer->message_id_low = can_read((rx_buffer->buffer_start_address)+2);
    /* Read RXB0's data length */
    rx_buffer->data_length = can_read((rx_buffer->buffer_start_address)+5);
    /* Read RXB0' data registers */
    rx_buffer->data0 = can_read((rx_buffer->buffer_start_address)+6);
    rx_buffer->data1 = can_read((rx_buffer->buffer_start_address)+7);
    rx_buffer->data2 = can_read((rx_buffer->buffer_start_address)+8);
    rx_buffer->data3 = can_read((rx_buffer->buffer_start_address)+9);
    rx_buffer->data4 = can_read((rx_buffer->buffer_start_address)+10);
    rx_buffer->data5 = can_read((rx_buffer->buffer_start_address)+11);
    rx_buffer->data6 = can_read((rx_buffer->buffer_start_address)+12);
    rx_buffer->data7 = can_read((rx_buffer->buffer_start_address)+13);
    /* Mark RXB0 available for new buffer */
    can_bit_modify(CANINTF, 0b00000001, 0b00000000);
}
