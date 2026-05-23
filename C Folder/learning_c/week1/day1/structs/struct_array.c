#include <stdio.h>
#include <stdint.h>

typedef struct {
    float       temperature;
    uint32_t    timestamp_ms;
} TempLog;

void find_max_temp(const TempLog *log, uint8_t count, float *max_temp, uint32_t *max_time)
{
    *max_temp = log[0].temperature;
    *max_time = log[0].timestamp_ms;

    for (uint8_t i = 1; i < count; i++)
    {
        if (log[i].temperature > *max_temp)
        {
            *max_temp = log[i].temperature;
            *max_time = log[i].timestamp_ms;
        }
    }
}

int main(void)
{
    TempLog readings[5] = {
        {.temperature = 27.1f, .timestamp_ms = 1000},
        {.temperature = 29.5f, .timestamp_ms = 2000},
        {.temperature = 35.8f, .timestamp_ms = 3000},
        {.temperature = 34.2f, .timestamp_ms = 4000},
        {.temperature = 30.0f, .timestamp_ms = 5000},
    };

    float       peak_temp;
    uint32_t    peak_time;

    find_max_temp(readings, 5, &peak_temp, &peak_time);

    printf("Peak temperature: %.1f C at %u ms\n", peak_temp, peak_time);

    return 0;
}