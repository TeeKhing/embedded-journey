#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main(void) {
    printf("=== Fixed-Width Type Sizes ===\n");
    printf("uint8_t:    %zu byte(s)\n", sizeof(uint8_t));
    printf("uint16_t:   %zu byte(s)\n", sizeof(uint16_t));
    printf("uint32_t:   %zu byte(s)\n", sizeof(uint32_t));
    printf("uint64_t:   %zu byte(s)\n", sizeof(uint64_t));
    printf("int8_t:     %zu byte(s)\n", sizeof(int8_t));
    printf("bool:       %zu byte(s)\n", sizeof(bool));
    printf("int:        %zu byte(s)\n", sizeof(int));
    printf("long:       %zu byte(s)\n", sizeof(long));

    printf("\n=== Value Ranges ===\n");
    uint8_t     max_u8  = 255;
    uint16_t    max_u16 = 65535;
    int8_t      max_s8  = 127;
    int8_t      min_s8  = -128;

    printf("uint8_t     max: %u\n", max_u8);
    printf("uint16_t    max: %u\n", max_u16);
    printf("int8_t      max: %d\n", max_s8);
    printf("int8_t      min: %d\n", min_s8);

    printf("\n=== Overflow Demonstration ===\n");
    uint8_t counter = 254;
    printf("counter starts at:  %u\n", counter);
    counter++;
    printf("after ++:           %u\n", counter);    // 255
    counter++;
    printf("after ++:           %u\n", counter);    // wraps to 0!
    counter++;
    printf("after ++:           %u\n", counter);    // 1

    printf("\n=== Signed Overflow ===\n");
    int8_t signed_val = 126;
    printf("signed_val starts at:   %d\n", signed_val);
    signed_val++;
    printf("after ++:               %d\n", signed_val);     // 127
    signed_val++;
    printf("after ++:               %d\n", signed_val);     // wraps to -128!
    
    return 0;
}