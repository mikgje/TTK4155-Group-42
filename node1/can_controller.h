void can_reset(void);
uint8_t can_init(void);
uint8_t can_read(uint8_t address);
void can_write(uint8_t address, uint8_t data);
void can_config(void);
void can_request_to_send(uint8_t txb0, uint8_t txb1, uint8_t txb2);
uint8_t can_read_status(void);
void can_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
void can_configure_transmit(void);
void can_configure_receive(void);
void can_configure_filters_and_masks(void);
void can_set_loopback(void);
void can_set_normal(void);

struct can_message {
    volatile uint8_t buffer_start_address, message_id_high, message_id_low, data_length;
    volatile uint8_t data0, data1, data2, data3, data4, data5, data6, data7;
};

void can_transmit_message(struct can_message* tx_buffer);
void can_receive_message(struct can_message* rx_buffer);
