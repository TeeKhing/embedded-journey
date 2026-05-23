#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define CAPACITY 6

// provides type structure for Buffer Details
typedef struct {
    uint16_t    data[CAPACITY];
    uint8_t     head;
    uint8_t     tail;
    uint8_t     count;

} RBufferTrack;

// initializes the buffer
void buffer_init(RBufferTrack *ring_buffer) 
{
    ring_buffer->head   = 0;
    ring_buffer->tail   = 0;
    ring_buffer->count  = 0;
}

// checks if the buffer is empty
uint8_t buffer_is_empty(RBufferTrack *ring_buffer) 
{
    return (ring_buffer->count == 0);
}

// checks if the buffer is full
uint8_t buffer_is_full(RBufferTrack *ring_buffer) 
{
    return (ring_buffer->count == CAPACITY);
}

// adds/appends a new data value to the tail index of buffer
uint8_t buffer_write(RBufferTrack *ring_buffer, uint16_t value)
{
    if (!buffer_is_full(ring_buffer))
        {
            ring_buffer->data[ring_buffer->tail] = value;
            ring_buffer->tail = (ring_buffer->tail + 1) % CAPACITY;
            ring_buffer->count++;

            return 1;
        }

    return 0;
}

// reads the data the head is pointing to
uint8_t buffer_read(RBufferTrack *ring_buffer, uint16_t *value)
{
    if (!buffer_is_empty(ring_buffer))
    {
        *value = ring_buffer->data[ring_buffer->head];
        ring_buffer->head = (ring_buffer->head + 1) % CAPACITY;
        ring_buffer->count--;

        return 1;
    }

    return 0;
}

int main(void)
{
    RBufferTrack circle;
    uint16_t output;
    buffer_init(&circle);
    
    buffer_write(&circle, 20);
    buffer_write(&circle, 32);
    buffer_write(&circle, 21);
    buffer_write(&circle, 19);
    buffer_write(&circle, 20);

    uint8_t total = circle.count;
    for (uint8_t i = 0; i < total; i++)
    {
        uint8_t index = circle.head;
        buffer_read(&circle, &output);
        printf("Value at index %u: %u\n", index, output);
    }

    return 0;
}