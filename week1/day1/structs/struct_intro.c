#include <stdio.h>
#include <stdint.h>

typedef struct {
    float       temperature;
    float       humidity;
    uint32_t    timestamp_ms;
    uint8_t     sensor_id;
} SensorReading;

void print_reading(const SensorReading *reading)
{
    printf("Sensor ID   : %u\n", reading->sensor_id);
    printf("Timestamp   : %u ms\n", reading->timestamp_ms);
    printf("Temperature : %.2f C\n", reading->temperature);
    printf("Humidity    : %.2f %%\n", reading->humidity);
}

int main(void)
{
    SensorReading r1    = {
        .temperature    = 28.5f,
        .humidity       = 65.3f,
        .timestamp_ms   = 5000,
        .sensor_id      = 1,
    };

    SensorReading r2    = {
        .temperature    = 31.2f,
        .humidity       = 70.1f,
        .timestamp_ms   = 10000,
        .sensor_id      = 2
    };

    printf("=== Reding 1 ===\n");
    print_reading(&r1);

    printf("\n=== Reading 2 ===\n");
    print_reading(&r2);

    printf("\nSize of SensorReading struct: %zu bytes\n", sizeof(SensorReading));

    return 0;
}