#ifndef _MULTIPOUET_
# define _MULTIPOUET_

#include "m_pd.h"



static t_class *multipouet_class;
typedef struct _multipouet {
  t_object x_obj;
  t_inlet *h_in;

  t_outlet *f_out; // outlet for list
  t_outlet *b_out; // outlet for bang

  t_int counter; // counter
  t_float step; // step for counter
  t_int start, end; // range for the counter

} t_multipouet;

void multipouet_bang(t_multipouet *x);
void *multipouet_new(int argc, t_atom *argv);
void multipouet_setup(void);


void multipouet_reset(t_multipouet *x);
void multipouet_set(t_multipouet *x, t_floatarg f);
void multipouet_setStep(t_multipouet *x, t_floatarg f);
void multipouet_bound(t_multipouet *x, t_floatarg min, t_floatarg max);


#endif