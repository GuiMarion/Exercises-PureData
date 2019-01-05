#ifndef _RINGBUFFER_
# define _RINGBUFFER_


#include <stdint.h>

typedef struct _ringBuffer {
    uint32_t currentIndex;
    uint32_t sizeOfBuffer;
    double *data;
} ringBuffer;


void init_ringBuffer(ringBuffer* rb, uint32_t bufferSize);

void free_ringBuffer(ringBuffer* rb);

void write_ringBuffer(ringBuffer *rb, double *myData, int numsamples);

void print_ringBuffer(ringBuffer *rb);

#endif