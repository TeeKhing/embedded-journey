#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
 *  Simulating two hardware status registers.
 *  In real embedded code, these would be at hardware addresses:
 * volatile uint8_t *STATUS_REG = (volatile uint8_t *)0x40013800;
*/
volatile uint8_t UART_STATUS    = 0x00;
volatile bool    DATA_READY     = false;

void simulate_hardware_update(void) {
    UART_STATUS = 0x20;     // hardware sets bit 5 = TX ready
    DATA_READY = true;
}

void wait_for_tx_ready(void) {
    /*
     * Without volatile on UART_STATUS, an optimising compiler 
     * might read it once, cache it in a register, and loop forever.
     * With volatile it re-reads from memory every iteration.
     */
    while (!(UART_STATUS & (1 << 5))) {
        /* waiting for TX ready bit */
    }
    printf("TX ready - safe to send data\n");
}

int main(void) {
    printf("Simulating hardware register access with volatile\n\n");

    printf("UART_STATUS before: 0x%02X\n", UART_STATUS);
    printf("DATA_READY before:  %s\n\n", DATA_READY ? "true" : "false");

    simulate_hardware_update();

    printf("UART_STATUS after:  0x%02X\n", UART_STATUS);
    printf("DATA_READY after:   %s\n\n", DATA_READY ? "true" : "false");

    wait_for_tx_ready();

    printf("\n=== Demonstarting static local variable ===\n");

    for (int i = 0; i < 5; i++) {
        static uint32_t call_count = 0;
        static bool     first_call = true;

        call_count++;

        if (first_call) {
            printf("First call detected!\n");
            first_call = false;
        }

        printf("Loop iteration %d, total calls so far: %u\n", i, call_count);
    }

    return 0;
}