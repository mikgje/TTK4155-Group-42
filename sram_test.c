#include "sram_test.h"
#include <stdint.h>
#include <stdlib.h>

void sram_test(void) {
    volatile char* ext_ram;
    uint16_t ext_ram_size, write_errors, retrieval_errors, seed;
    /* start address for the SRAM */
    ext_ram = (char *) 0x1800;
    ext_ram_size = 0x800;
    write_errors = 0;
    retrieval_errors = 0;
    printf("Starting SRAM test...\n");
    /* rand() stores some internal state, so calling this function in a loop will
     * yield different seeds each time (unless srand() is called before this function)
     */
    seed = rand();
    /* write phase: immediately check that the correct value was stored */
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; ++i) {
        uint8_t some_value, retrieved_value;
        some_value = rand();
        ext_ram[i] = some_value;
        retrieved_value = ext_ram[i];
        if (retrieved_value != some_value) {
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retrieved_value, some_value);
            ++write_errors;
        }
    }
    /* retrieval phase: Check that no values were changed during or after the write phase */
    srand(seed);
    /* reset the PRNG to the state it had before the write phase */
    for (uint16_t i = 0; i < ext_ram_size; ++i) {
        uint8_t some_value, retrieved_value;
        some_value = rand();
        retrieved_value = ext_ram[i];
        if (retrieved_value != some_value) {
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retrieved_value, some_value);
            ++retrieval_errors;
        }
    }
    printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}
