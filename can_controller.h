void can_reset();
void can_init();
void can_config();
uint8_t can_read(uint8_t address);
void can_write(uint8_t address, uint8_t data);
uint8_t can_read_status();
void can_bit_modify(uint8_t address, uint8_t mask, uint8_t data);