#ifndef _DUCK_
# define _DUCK_

#include "m_pd.h"



static t_class *duck_tilde_class;
typedef struct _duck_tilde {
  t_object x_obj;

  t_sample f;
  t_inlet *x_in2; // input and the second signal
  t_outlet *x_out; // signal output


} t_duck_tilde;

void *duck_tilde_new(void);
void duck_tilde_free(t_duck_tilde *x);
void duck_tilde_setup(void);

t_int *duck_tilde_perform(t_int *w);
void duck_tilde_dsp(t_duck_tilde *x, t_signal **sp);


#endif