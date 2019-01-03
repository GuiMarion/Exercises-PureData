#include "duck~.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


void *duck_tilde_new(void){

	t_duck_tilde *x = (t_duck_tilde *)pd_new(duck_tilde_class);
	x->x_in2 = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
	x->x_out = outlet_new(&x->x_obj, &s_signal);

	return (void *)x;

}

void duck_tilde_free(t_duck_tilde *x){

	inlet_free(x->x_in2);
	outlet_free(x->x_out);

}

void duck_tilde_setup(void) {
	duck_tilde_class = class_new(gensym("duck~"),
        (t_newmethod)duck_tilde_new,
        (t_method)duck_tilde_free,
        sizeof(t_duck_tilde),
        CLASS_DEFAULT,
        A_DEFFLOAT, 0);


  class_addmethod(duck_tilde_class,
        (t_method)duck_tilde_dsp, gensym("dsp"), 0);

  CLASS_MAINSIGNALIN(duck_tilde_class, t_duck_tilde, f);
}

float absolute_value(float f){

	if (f < 0){
		return -f;
	}
	else{
		return f;
	}
}

t_int *duck_tilde_perform(t_int *w){

	/* here is a pointer to the t_sample arrays that hold the 2 input signals */
	t_sample  *in1 =    (t_sample *)(w[2]);
	t_sample  *in2 =    (t_sample *)(w[3]);
	/* here comes the signalblock that will hold the output signal */
	t_sample  *out =    (t_sample *)(w[4]);
	/* all signalblocks are of the same length */
	int          n =           (int)(w[5]);

	/* just a counter */

	float volume = 0;

	int i;
	for (i = 0; i<n; i ++){
		volume += absolute_value(in2[i]);
	}

	volume /= n;

	// We mutiply the signal by (1-volume)
	for(i=0; i<n; i++)
	{
		out[i]=in1[i]*(1-volume);
	}

	/* return a pointer to the dataspace for the next dsp-object */
	return (w+6);

}
void duck_tilde_dsp(t_duck_tilde *x, t_signal **sp){
	
	dsp_add(duck_tilde_perform, 5, x,
          sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);


}







