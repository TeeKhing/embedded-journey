#include <stdio.h>

int main(void)
{
    int sensor_readings[5] = {10, 20, 30, 40, 50};
    int *ptr = sensor_readings;

    printf("ptr points to first element: %d\n", *ptr);

    ptr++;  // adds sizeof(int) instead of 1
    printf("After ptr++, value is: %d\n", *ptr);

    ptr += 2;   // adds sizeof(int) * 2 instead of 2
    printf("After ptr+=2, value is: %d\n", *ptr);

    printf("\n Iterating array with pointer:\n");
    ptr = sensor_readings;
    for (int i = 0; i < 5; i++)
    {
        printf(" sensor_readings[%d] = %d (address: %p)\n",
            i, *ptr, (void *)ptr);
            ptr++;
    }

    return 0;
}