#ifndef _MYFFT_
# define _MYFFT_

#include "m_pd.h"



static t_class *myfft_tilde_class;
typedef struct _myfft_tilde {
  t_object x_obj;

  t_sample f;
  t_outlet *x_out; // signal output


} t_myfft_tilde;

void *myfft_tilde_new(void);
void myfft_tilde_free(t_myfft_tilde *x);
void myfft_tilde_setup(void);

t_int *myfft_tilde_perform(t_int *w);
void myfft_tilde_dsp(t_myfft_tilde *x, t_signal **sp);


#endif