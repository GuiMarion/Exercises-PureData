#include "ringBuffer.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void init_ringBuffer(ringBuffer* rb, uint32_t bufferSize){
	// Initialize the ring buffer
	//ringBuffer *myRingBuffer = (ringBuffer *)calloc(1, sizeof(ringBuffer));

	rb->data = malloc(sizeof(double) * bufferSize);
	rb->sizeOfBuffer = bufferSize;
	rb->currentIndex = 0;
}

void free_ringBuffer(ringBuffer* rb){

	free(rb->data);
}

void write_ringBuffer(ringBuffer *rb, double *myData, int numsamples) {
    // -1 for our binary modulo in a moment
    int buffLen = rb->sizeOfBuffer - 1;
    int lastWrittenSample = rb->currentIndex;

    int idx;
    for (int i=0; i < numsamples; ++i) {
        // modulo will automagically wrap around our index
        idx = (i + lastWrittenSample) & buffLen; 
        rb->data[idx] = myData[i];
    }

    // Update the current index of our ring buffer.
    rb->currentIndex += numsamples;
    rb->currentIndex &= rb->sizeOfBuffer - 1;
}

void print_ringBuffer(ringBuffer *rb){

	int i;
	for (i = 0; i < rb->sizeOfBuffer; i ++){

		printf("%f ", rb->data[i]);
	}
}


