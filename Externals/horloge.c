#include "horloge.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

void horloge_bang(t_horloge *x){

    time_t mytime = time(NULL);
	char* time_str = malloc(8);
	strncpy(time_str, ctime(&mytime) + 11, 8);

	time_str[8] = '\0';

	outlet_symbol(x->h_out, gensym(time_str));

    free(time_str); // free the memory
	time_str = NULL; // invalidate the pointer

	//outlet_float(x->h_out, x->counter);
}

void *horloge_new(void){

	t_horloge *h;

	h = (t_horloge *)pd_new(horloge_class);
	h->h_out = outlet_new(&h->x_obj, &s_symbol);
	h->counter = 0;

	return (void*)h;

}

void horloge_setup(void){
	horloge_class = class_new(gensym("horloge"),
	    (t_newmethod)horloge_new,
	    0, sizeof(t_horloge),
	    CLASS_DEFAULT, 0);
  class_addbang(horloge_class, horloge_bang);
}
