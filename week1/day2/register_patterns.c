#include <stdio.h>
#include <stdint.h>

void print_binary(uint8_t value) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf(" (0x%02X)", value);
}

int main(void) {
    uint8_t reg = 0b10100010;

    printf("Initial register:   "); print_binary(reg); printf("\n\n");

    reg |= (1 << 3);
    printf("After SET bit 3:    "); print_binary(reg); printf("\n");

    reg &= ~(1 << 5);
    printf("After CLEAR bit 5:  "); print_binary(reg); printf("\n");

    reg ^= (1 << 1);
    printf("After TOGGLE bit 1: "); print_binary(reg); printf("\n");
    reg ^= (1 << 1);
    printf("Toggle bit 1 again: "); print_binary(reg); printf("\n\n");

    printf("Checking individual bits:\n");
    for (int i = 7; i >= 0; i--) {
        printf("    Bit %d is %s\n", i, 
            (reg & (1 << i)) ? "SET (1)" : "CLEAR (0)");
    }

    return 0;
}