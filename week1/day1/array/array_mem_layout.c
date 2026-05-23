#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint8_t sensor_buffer[8] = {0xAA, 0xBB, 0x01, 0x02, 0x03, 0x04, 0x05, 0xFF};

    printf("Array name as address:  %p\n", (void *)sensor_buffer);
    printf("Address of [0]:         %p\n", (void *)&sensor_buffer[0]);

    printf("\nBuffer contents:\n");
    for (int i = 0; i < 8; i++)
    {
        printf(" [%d] addr=%p   value=0x%02X\n", 
            i, (void *)&sensor_buffer[i], sensor_buffer[i]);
    }

    return 0;
}