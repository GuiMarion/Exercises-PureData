#include "myfft~.h"

#include <math.h>
#include <stdio.h>

#include "fft4g_h.c"

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PI 3.14159265358979323846

void *myfft_tilde_new(void){

	t_myfft_tilde *x = (t_myfft_tilde *)pd_new(myfft_tilde_class);
	x->x_out = outlet_new(&x->x_obj, &s_signal);

	return (void *)x;

}

void myfft_tilde_free(t_myfft_tilde *x){

	outlet_free(x->x_out);

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

	/* here is a pointer to the t_sample arrays that hold the 2 input signals */
	t_sample  *in1 =    (t_sample *)(w[2]);
	/* here comes the signalblock that will hold the output signal */
	t_sample  *out =    (t_sample *)(w[3]);
	/* all signalblocks are of the same length */
	int          n =           (int)(w[4]);

	int T = 64;
	int t;
	double fft_array[64];

	for(t=0; t<T; t++){

		fft_array[t] = 0.42 - 0.5*cos(2*PI*t/T) + 0.08*cos(4*PI*t/T);
		fft_array[t] *= in1[t];

	}

	rdft(T, 1, fft_array);

	int i;
	for(i=0; i<n; i++)
	{
		out[i]=fft_array[i];
		//out[i] = in1[i];
	}

	/* return a pointer to the dataspace for the next dsp-object */
	return (w+5);

}
void myfft_tilde_dsp(t_myfft_tilde *x, t_signal **sp){
	
	dsp_add(myfft_tilde_perform, 4, x,
          sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);


}







