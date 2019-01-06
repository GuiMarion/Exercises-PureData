#include "mysynth1~.h"
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

#define AUTONORM 1
//TODO : implémenter autonorm

/*
				PART FOR THE MYSYNTH1 OBJECT
*/

void *mysynth1_tilde_new(t_symbol *s, int argc, t_atom *argv){

	t_mysynth1_tilde *x = (t_mysynth1_tilde *)pd_new(mysynth1_tilde_class);
	x->in2 = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
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


	init_ringBuffer(&x->buff1, buffSize);
	init_ringBuffer(&x->buff2, buffSize);
	post("buff size, %d", buffSize);

	// Declare inlets
	inlet_new(&x->x_obj, &x->x_obj.ob_pd,
        gensym("float"), gensym("set_threshold"));
	inlet_new(&x->x_obj, &x->x_obj.ob_pd,
        gensym("float"), gensym("set_bypass"));

	x->threshold = 0.5;
	x->bypass = 0;

	return (void *)x;

}

void mysynth1_tilde_set_threshold(t_mysynth1_tilde *x, t_floatarg f){

	x->threshold = f;

}
void mysynth1_tilde_set_bypass(t_mysynth1_tilde *x, t_floatarg f){

	x->bypass = f;
}

void mysynth1_tilde_free(t_mysynth1_tilde *x){

	outlet_free(x->x_out);
	free_ringBuffer(&x->buff1);
	free_ringBuffer(&x->buff2);

}

void mysynth1_tilde_setup(void) {
	mysynth1_tilde_class = class_new(gensym("mysynth1~"),
        (t_newmethod)mysynth1_tilde_new,
        (t_method)mysynth1_tilde_free,
        sizeof(t_mysynth1_tilde),
        CLASS_DEFAULT,
        A_GIMME, 0);


 	class_addmethod(mysynth1_tilde_class,
        (t_method)mysynth1_tilde_dsp, gensym("dsp"), 0);


	class_addmethod(mysynth1_tilde_class,
        (t_method)mysynth1_tilde_set_bypass, gensym("set_bypass"),
        A_DEFFLOAT, 0);

	class_addmethod(mysynth1_tilde_class,
        (t_method)mysynth1_tilde_set_threshold, gensym("set_threshold"),
        A_DEFFLOAT, 0);

 	CLASS_MAINSIGNALIN(mysynth1_tilde_class, t_mysynth1_tilde, f);
}

t_int *mysynth1_tilde_perform(t_int *w){

	/* We get the object in order to access the buffer */
  	t_mysynth1_tilde *x = (t_mysynth1_tilde *)(w[1]);
	/* here is a pointer to the t_sample arrays that hold the 2 input signals */
	t_sample  *in1 =    (t_sample *)(w[2]);
	/* here is a pointer to the t_sample arrays that hold the 2 input signals */
	t_sample  *in2 =    (t_sample *)(w[3]);
	/* here comes the signalblock that will hold the output signal */
	t_sample  *out =    (t_sample *)(w[4]);
	/* all signalblocks are of the same length */
	int          n =           (int)(w[5]);

	write_ringBuffer(&x->buff1, in1, n);
	write_ringBuffer(&x->buff2, in2, n);

	if (x->bypass == -1){

		post("%f \n", x->threshold);
	}

	// If bypass is true we copy the main signal and return
	if (x->bypass == 1){
		int i;
		for(i=0; i<n; i++)
		{
			out[i]=in1[i];
		}
	}

	// If the buffer is full, we process the signal
	else if (x->buff1.addedObjects >= (int)x->buff1.sizeOfBuffer){


		int T = x->buff1.sizeOfBuffer;
		int t;
		double fft_array1[T];
		double fft_array2[T];

		// We window the signals with a Blackman window
		for(t=0; t<T; t++){

			// first signal
			fft_array1[t] = x->buff1.data[t];
			fft_array1[t] *= 0.42 - 0.5*cos(2*PI*t/T) + 0.08*cos(4*PI*t/T);

			// second signal
			fft_array2[t] = x->buff2.data[t];
			fft_array2[t] *= 0.42 - 0.5*cos(2*PI*t/T) + 0.08*cos(4*PI*t/T);

		}

		// compute the fft for the first signal
		rdft(T, 1, fft_array1);

		// compute the fft for the second signal
		rdft(T, 1, fft_array2);

		float inGain = 0;
		int i =0;
		if (AUTONORM == 1){

			for (i = 0; i < T; i += 2){
				if (fabs(fft_array1[i] - fft_array1[i+1]) > inGain){
					inGain = fabs(fft_array1[i] - fft_array1[i+1]);
				}
			}
		}

		double a1, a2, b1, b2;
		double curGain;
		double tmp1[T];
		double tmp2[T];
		for (i = 0; i < T; i += 2){

			a1 = fft_array1[i];
			a2 = fft_array2[i];
			b1 = fft_array1[i+1];
			b2 = fft_array2[i+1];

			curGain = fabs(a2 - b2);

			if (curGain > x->threshold){
				tmp1[i] = fabs(a1 - b2) * curGain;

			}

			tmp1[i+1] = -atan2(b1, a2);
			tmp2[i] = tmp1[i] * cos(tmp1[i+1]);
			tmp2[i+1] = -tmp1[i] * sin(tmp1[i+1]);


		}

		double outGain = 0;
		double finalGain = 1;
		if (AUTONORM ==1){

			for (i = 0; i < T; i +=2){
				if (fabs(tmp2[i] - tmp2[i+1]) > outGain){
					outGain = fabs(tmp2[i] - tmp2[i+1]);
				}
			}

			finalGain = inGain / outGain;
		}

		/*
		As pd works with 64 sample long buffers, 
		we have to decimate the signal, for know we don't use 
		high-pass filter but this can causes some aliasing.
		*/

		double outTemp[n];
		for(i = 0; i < n; i++)
		{
			outTemp[i] = tmp2[i*(T/n)];
			//outTemp[i] = log(fft_array1[i*(T/n)]) * fft_array2[i*(T/n)] / log(fft_array2[i*(T/n)]);
			//outTemp[i] *= 0.42 - 0.5*cos(2*PI*i/T) + 0.08*cos(4*PI*i/T);

		}

		// We do the inverse fft
		rdft(T, -1, outTemp);

		for (i = 0; i < n; i++){

			out[i] = outTemp[i] * finalGain;

		}


	}

	// If the buffer is not full we return some 0
	else{
		int i;
		for(i=0; i<n; i++)
		{
			out[i]=0.0;
		}

	}
	/* return a pointer to the dataspace for the next dsp-object */
	return (w+6);

}
void mysynth1_tilde_dsp(t_mysynth1_tilde *x, t_signal **sp){
	
	dsp_add(mysynth1_tilde_perform, 5, x,
          sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);


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

