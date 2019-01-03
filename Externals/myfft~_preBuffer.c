#include "myfft~.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


void *myfft_tilde_new(void){

	t_myfft_tilde *x = (t_myfft_tilde *)pd_new(myfft_tilde_class);
	x->x_out = outlet_new(&x->x_obj, &s_signal);
	x->buffer_size = 8192;
	x->buffer = malloc(x->buffer_size*sizeof(t_float)); // We maintain a buffer of size 8192 for the fft
	x->position = 0;

	int i;
	for (i=0; i< x->buffer_size; i++){
		x->buffer[i] = 0.0;
	}

	return (void *)x;

}

void myfft_tilde_free(t_myfft_tilde *x){

	outlet_free(x->x_out);
	free(x->buffer);

}

void myfft_tilde_setup(void) {
	myfft_tilde_class = class_new(gensym("myfft~"),
        (t_newmethod)myfft_tilde_new,
        (t_method)myfft_tilde_free,
        sizeof(t_myfft_tilde),
        CLASS_DEFAULT,
        A_DEFFLOAT, 0);


  class_addmethod(myfft_tilde_class,
        (t_method)myfft_tilde_dsp, gensym("dsp"), 0);

  CLASS_MAINSIGNALIN(myfft_tilde_class, t_myfft_tilde, f);
}

t_int *myfft_tilde_perform(t_int *w){

	t_myfft_tilde *x = (t_myfft_tilde *)(w[1]);
	/* here is a pointer to the t_sample arrays that hold the 2 input signals */
	t_sample  *in1 =    (t_sample *)(w[2]);
	/* here comes the signalblock that will hold the output signal */
	t_sample  *out =    (t_sample *)(w[3]);
	/* all signalblocks are of the same length */
	int          n =           (int)(w[4]);

	// char array[10];
	// sprintf(array, "%d", x->position);
	// post("position:");
	// post(array);

	// sprintf(array, "%d", x->buffer_size);
	// post("buffer_size:");
	// post(array);

	if (x->position + n > x->buffer_size){

		char array[100];
		sprintf(array, "%f", x->buffer[0]);
		post(array);

		/*COMPUTE FFT*/

		x->position = 0;

	}
	int i;
	for(i=0; i<n; i++)
	{
		out[i]=in1[i];
		x->buffer[x->position] = in1[i];
		x->position ++;
	}

	/* return a pointer to the dataspace for the next dsp-object */
	return (w+5);

}
void myfft_tilde_dsp(t_myfft_tilde *x, t_signal **sp){
	
	dsp_add(myfft_tilde_perform, 4, x,
          sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);


}







