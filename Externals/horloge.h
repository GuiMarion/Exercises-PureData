#ifndef _HORLOGE_
# define _HORLOGE_

#include "m_pd.h"



static t_class *horloge_class;
typedef struct _horloge {
  t_object x_obj;
  t_inlet *h_in;
  t_outlet *h_out;
  t_int counter;
} t_horloge;

void horloge_bang(t_horloge *x);

void *horloge_new(void);

void horloge_setup(void);

#endif