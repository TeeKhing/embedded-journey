#include <stdio.h>

void celsius_to_fahrenheit(float *celsius, float *result)
{
    *result = (*celsius * 9.0f / 5.0f) + 32.0f;
}

int main(void)
{
    float temp_c = 100.0f;
    float temp_f = 0.0f;

    celsius_to_fahrenheit(&temp_c, &temp_f);

    printf("%.1f C = %.1f F\n", temp_c, temp_f);

    return 0;
}