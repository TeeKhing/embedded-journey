#include <stdio.h>
#include <stdint.h>

void print_binary(uint8_t v) {
    for (int i = 7; i >= 0; i--) printf("%d", (v >> i) & 1);
    printf(" (0x%02X)", v);
}

int main(void) {
    uint8_t reg;

    /* Exercise 1: Set bit 3 of 0b10100000 */
    reg = 0b10100000;
    reg |= (1 << 3);
    printf("Ex1 Set bit 3:  "); print_binary(reg);
    printf(" (expected: 10101000)\n");

    /* Exercise 2: Clear bit 7 of 0b11110000 */
    reg = 0b11110000;
    reg &= ~(1 << 7);
    printf("Ex2 Clear bit 7:    "); print_binary(reg);
    printf(" (expected: 01110000\n");

    /* Exercise 3: Toggle bit 4 of 0b00001111 */
    reg = 0b00001111;
    reg ^= (1 << 4);
    printf("Ex3 Toggle bit 4: "); print_binary(reg);
    printf(" (expected: 00011111)\n");

    /* Exercise 4: Set bits 0 and 7 simultaneously */
    reg = 0b01000110;
    reg |= (1 << 0) | (1 << 7);
    printf("Ex4 Set 0 and 7: "); print_binary(reg);
    printf(" (expected: 11000111)\n");

    /* Exercise 5: Clear bits 2 and 5 simultaneously */
    reg = 0b11111111;
    reg &= ~((1 << 2) | (1 << 5));
    printf("Ex5 Clear 2,5:  "); print_binary(reg);
    printf(" (expected: 11011011)\n");

    /* Exercise 6: Check if bit 6 is set - print result */
    reg = 0b11000000;
    printf("Ex6 Bit 6 set?  %s (expected: YES)\n",
    (reg & (1 << 6)) ? "YES" : "NO");

    /* Exercise 7: Extract the lower nibble (bits 3:0) of 0xAB */
    reg = 0xAB;
    uint8_t lower = reg & 0x0F;
    printf("Ex7 Lower nibble: "); print_binary(lower);
    printf(" (expected: 00001011)\n");

    /* Exercise 8: Extract the upper nibble (bits 7:4) of 0xAB, shift down */
    reg = 0xAB;
    uint8_t upper = (reg >> 4) & 0x0F;
    printf("Ex8 Upper nibble: "); print_binary(upper);
    printf(" (expected: 00001010)\n");

    /* Exercise 9: Swap the upper and lower nibbles of 0xAB */
    reg = 0xAB;
    uint8_t swapped = ((reg & 0x0F) << 4) | ((reg >> 4) & 0x0F);
    printf("Ex9 Nibble swap: "); print_binary(swapped);
    printf(" (expected: 10111010)\n");

    /* Exercise 10: Count the number of set bits in a byte (population count) */
    reg = 0b10110101;
    uint8_t count = 0;
    uint8_t temp = reg;
    while (temp) {
        count += temp & 1;
        temp >>= 1;
    }
    printf("Ex10 Set bits in "); print_binary(reg);
    printf(":\n     Count = %u (expected: 5)\n", count);

    return 0;
}