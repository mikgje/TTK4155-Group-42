void can_reset();
void can_init();
void can_config();
uint8_t can_read(uint8_t address);
void can_write(uint8_t address, uint8_t data);
uint8_t can_read_status();
void can_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
void can_configure_transmit();
void can_set_loopback(void);

struct can_message {
    volatile uint8_t buffer_start_address;
    volatile uint8_t data0;
    volatile uint8_t data1;
    volatile uint8_t data2;
    volatile uint8_t data3;
    volatile uint8_t data4;
    volatile uint8_t data5;
    volatile uint8_t data6;
    volatile uint8_t data7;
};

void can_send_message(struct can_message *tx_buffer);
void can_receive_message(struct can_message *rx_buffer);
