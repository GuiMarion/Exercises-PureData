#include "myfft~.h"
#include "fft4g_h.c"

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PI 3.14159265358979323846

// in order to silence unused mandatory parameters
#define UNUSED(x) (void)(x)

/*
				PART FOR THE MYFFT OBJECT
*/


void *myfft_tilde_new(t_symbol *s, int argc, t_atom *argv){

	t_myfft_tilde *x = (t_myfft_tilde *)pd_new(myfft_tilde_class);
	x->x_out = outlet_new(&x->x_obj, &s_signal);
	UNUSED(s);

	int buffSize = 8192;

	if (argc == 1){

		buffSize = (int) atom_getfloat(argv);

		// We use it if and only if it's a usable power of 2
		if (buffSize == 64 || buffSize == 128 || buffSize == 256 || buffSize == 512 ||
			buffSize == 1024 || buffSize == 2048 || buffSize == 4096 || buffSize == 8192 ||
			buffSize == 16384 || buffSize == 32768 || buffSize == 65536 || buffSize == 131072){

		}
		else{

			// if it's not the case, we use the default value
			buffSize = 8192;
		}

	}


	init_ringBuffer(&x->buff, buffSize);
	post("buff size, %d", buffSize);


	return (void *)x;

}

void myfft_tilde_free(t_myfft_tilde *x){

	outlet_free(x->x_out);
	free_ringBuffer(&x->buff);

}

void myfft_tilde_setup(void) {
	myfft_tilde_class = class_new(gensym("myfft~"),
        (t_newmethod)myfft_tilde_new,
        (t_method)myfft_tilde_free,
        sizeof(t_myfft_tilde),
        CLASS_DEFAULT,
        A_GIMME, 0);


  class_addmethod(myfft_tilde_class,
        (t_method)myfft_tilde_dsp, gensym("dsp"), 0);

  CLASS_MAINSIGNALIN(myfft_tilde_class, t_myfft_tilde, f);
}

t_int *myfft_tilde_perform(t_int *w){

	/* We get the object in order to access the buffer */
  	t_myfft_tilde *x = (t_myfft_tilde *)(w[1]);
	/* here is a pointer to the t_sample arrays that hold the 2 input signals */
	t_sample  *in1 =    (t_sample *)(w[2]);
	/* here comes the signalblock that will hold the output signal */
	t_sample  *out =    (t_sample *)(w[3]);
	/* all signalblocks are of the same length */
	int          n =           (int)(w[4]);

	write_ringBuffer(&x->buff, in1, n);

	// If the buffer is full, we process the signal
	if (x->buff.addedObjects >= (int)x->buff.sizeOfBuffer){

		int T = x->buff.sizeOfBuffer;
		int t;
		double fft_array[T];

		// We window the signal with a Blackman window
		for(t=0; t<T; t++){

			fft_array[t] = 0.42 - 0.5*cos(2*PI*t/T) + 0.08*cos(4*PI*t/T);
			fft_array[t] *= x->buff.data[t];

		}

		rdft(T, 1, fft_array);

		/*
		As pd works with 64 sample long buffers, 
		we have to decimate the signal, for know we don't use 
		high-pass filter but this can causes some aliasing.
		*/
		double max = 0;
		int i;
		for(i=0; i<n; i++)
		{
			out[i]=fabs(fft_array[i*(T/n)]);
			if (out[i] > max){
				max = out[i];
			}
			//out[i] = in1[i];
		}

		// We normalize the data for plotting
		for (i = 0; i < n; i ++){

			out[i] /= max;
		}

	}

	else{
		int i;
		for(i=0; i<n; i++)
		{
			out[i]=0.0;
		}

	}
	/* return a pointer to the dataspace for the next dsp-object */
	return (w+5);

}
void myfft_tilde_dsp(t_myfft_tilde *x, t_signal **sp){
	
	dsp_add(myfft_tilde_perform, 4, x,
          sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);


}


/*
			PART FOR RINGBUFFER DEFINITION, a proper class file is placed in ringBuffer/
*/


void init_ringBuffer(ringBuffer* rb, uint32_t bufferSize){
	// Initialize the ring buffer
	//ringBuffer *myRingBuffer = (ringBuffer *)calloc(1, sizeof(ringBuffer));

	rb->data = malloc(sizeof(t_sample) * bufferSize);
	rb->sizeOfBuffer = bufferSize;
	rb->currentIndex = 0;
	rb->addedObjects = 0;
}

void free_ringBuffer(ringBuffer* rb){

	free(rb->data);
}

void write_ringBuffer(ringBuffer *rb, t_sample *myData, int numsamples) {
    // -1 for our binary modulo in a moment
    int buffLen = rb->sizeOfBuffer - 1;
    int lastWrittenSample = rb->currentIndex;

    int idx;
    for (int i=0; i < numsamples; ++i) {
        // modulo will automagically wrap around our index
        idx = (i + lastWrittenSample) & buffLen; 
        rb->data[idx] = myData[i];
    }

    if (rb->addedObjects < (int)rb->sizeOfBuffer){
   		rb->addedObjects += numsamples;
   	}

    // Update the current index of our ring buffer.
    rb->currentIndex += numsamples;
    rb->currentIndex &= rb->sizeOfBuffer - 1;
}

void print_array(double *data, int n){

	int i;
	for (i = 0; i < n; i ++){

		post("%f ", data[i]);
	}

	post("\n");
}

void print_ringBuffer(ringBuffer *rb){

	int i;
	for (i = 0; i < (int)rb->sizeOfBuffer; i ++){

		post("%f ", rb->data[i]);
	}
}







