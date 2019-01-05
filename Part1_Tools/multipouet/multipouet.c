#include "multipouet.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

void multipouet_bang(t_multipouet *x){

	x->counter += x->step;

	if (x->counter > x->end){
		x->counter = x->start;
		outlet_bang(x->b_out);

	} else if (x->counter < x->start) {
		x->counter = x->end;
		outlet_bang(x->b_out);
  	}

  	/*
  	char** list = malloc(x->counter * sizeof(char*));
  	char string[] = "pouet";

  	int i;
  	for (i = 0; i < x->counter; i++){

  		list[i] = string;
  	}

	outlet_list(x->f_out, gensym("list"), x->counter, list);

	free(list);
	*/

	outlet_float(x->f_out, x->counter);
}

void *multipouet_new(int argc, t_atom *argv){
	t_multipouet *x;

	x = (t_multipouet *)pd_new(multipouet_class);
	t_float initialStep = 1;
	t_int initialStart = 0;
	t_int initialEnd = 20;

	if (argc == 1){

		initialStart = atom_getfloat(argv);
	}

	else if (argc == 2){
		initialStart = atom_getfloat(argv);
		initialEnd = atom_getfloat(argv+1);

	}

	else if (argc == 3){
		initialStart = atom_getfloat(argv);
		initialEnd = atom_getfloat(argv+1);
		initialStep = atom_getfloat(argv+2);

	}

	// We put the values into the object
	x->counter = initialStart;
	x->start = initialStart;
	x->end = initialEnd;
	x->step = initialStep;

	// Declare inlets
	inlet_new(&x->x_obj, &x->x_obj.ob_pd,
        gensym("float"), gensym("set_step"));
	inlet_new(&x->x_obj, &x->x_obj.ob_pd,
        gensym("list"), gensym("bound"));

	// Declare outlets
	x->f_out = outlet_new(&x->x_obj, &s_float);
	x->b_out = outlet_new(&x->x_obj, &s_bang);

	return (void*)x;

}

void multipouet_setup(void){
	multipouet_class = class_new(gensym("multipouet"),
			(t_newmethod)multipouet_new,
			0, sizeof(t_multipouet),
			CLASS_DEFAULT,
			A_GIMME, 0);

	class_addbang(multipouet_class, multipouet_bang);

	class_addmethod(multipouet_class,
        (t_method)multipouet_reset,
        gensym("reset"), 0);

	class_addmethod(multipouet_class,
        (t_method)multipouet_set, gensym("set"),
        A_DEFFLOAT, 0);

	class_addmethod(multipouet_class,
        (t_method)multipouet_setStep, gensym("set_step"),
        A_DEFFLOAT, 0);

	class_addmethod(multipouet_class,
        (t_method)multipouet_bound, gensym("bound"),
        A_DEFFLOAT, A_DEFFLOAT, 0);

	class_sethelpsymbol(multipouet_class, gensym("help-multipouet"));
}

void multipouet_reset(t_multipouet *x){

	x->counter = x->start;

}
void multipouet_set(t_multipouet *x, t_floatarg f){

	x->counter = f;

}

void multipouet_setStep(t_multipouet *x, t_floatarg f){

	x->step = f;
}

void multipouet_bound(t_multipouet *x, t_floatarg min, t_floatarg max){

	x->start = min;
	x->end = max;

}







