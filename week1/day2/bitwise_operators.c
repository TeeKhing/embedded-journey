#include <stdio.h>
#include <stdint.h>

void print_binary(uint8_t value) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i == 4) printf(" ");
    }
    printf("    (0x%02X = %u)", value, value);
}

int main(void) {
    uint8_t a = 0b10110110;
    uint8_t b = 0b01101011;

    printf("a = "); print_binary(a); printf("\n");
    printf("b = "); print_binary(b); printf("\n\n");

    printf("a & b  = "); print_binary(a & b);   printf("  (AND)\n");
    printf("a | b  = "); print_binary(a | b);   printf("  (OR)\n");
    printf("a ^ b  = "); print_binary(a ^ b);   printf("  (XOR)\n");
    printf("~a     = "); print_binary(~a);      printf("  (NOT)\n");
    printf("a << 2 = "); print_binary(a << 2);  printf("  (left shift 2)\n");
    printf("a >> 2 = "); print_binary(a >> 2);  printf("  (right shift 2)\n");

    return 0;
}