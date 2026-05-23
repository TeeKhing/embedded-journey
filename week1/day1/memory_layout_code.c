#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t    global_counter = 0;     // .data section
uint8_t     receive_buffer[64];     // .bss section (auto zero-filled)

void demonstrate_stack_and_static(void)
{
    uint8_t local_var = 100;        // stack - gone when function returns
    static uint8_t persistent = 0;  // .bss - survives function return

    persistent++;

    printf("    local_var address:      %p (stack)\n", (void *)&local_var);
    printf("    persistent address:     %p (.bss)\n", (void *)&persistent);
    printf("    persistent value:       %u\n", persistent);
}

int main(void)
{
    printf("=== Memory Section Addresses ===\n");
    printf("global_counter:     %p (.data)\n", (void *)&global_counter);
    printf("receive_buffer:     %p (.bss)\n", (void *)receive_buffer);

    printf("\n=== Stack vs Static ===\n");
    demonstrate_stack_and_static();
    demonstrate_stack_and_static();
    demonstrate_stack_and_static();

    printf("\n=== Heap Allocation ===\n");
    uint8_t *heap_buf = (uint8_t *)malloc(128);
    if (heap_buf == NULL)
    {
        printf("malloc failed!\n");
        return 1;
    }
    printf("heap_buf address:   %p (heap)\n", (void *)heap_buf);
    free(heap_buf);
    heap_buf = NULL;    // always NULL after to prevent use-after-free

    printf("\n=== SIZES ===\n");
    printf("sizeof(uint8_t):    %zu\n", sizeof(uint8_t));
    printf("sizeof(uint16_t):   %zu\n", sizeof(uint16_t));
    printf("sizeof(uint32_t):   %zu\n", sizeof(uint32_t));
    printf("sizeof(float):      %zu\n", sizeof(float));
    printf("sizeof(double):     %zu\n", sizeof(double));

    return 0;
}