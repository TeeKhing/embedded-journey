#include <stdio.h>
#include <stdint.h>

void compute_average(uint16_t *data, uint8_t length, uint16_t *result)
{
    uint32_t sum = 0;
    for (uint8_t i = 0; i < length; i++)
    {
        sum += data[i];
    }
    *result =(uint16_t)(sum / length);
}

int main(void)
{
    uint16_t adc_readings[6] = {1023, 987, 1010, 990, 1005, 1001};
    uint16_t average = 0;

    compute_average(adc_readings, 6, &average);

    printf("Average ADC reading: %u\n", average);

    return 0;
}