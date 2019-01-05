#ifndef _MYFFT_
# define _MYFFT_

#include "m_pd.h"


/*
		PART FOR RINGBUFFER DEFINITION, a proper class file is placed in ringBuffer/
*/


typedef struct _ringBuffer {
    uint32_t currentIndex;
    uint32_t sizeOfBuffer;
    t_sample *data;
    int addedObjects;
} ringBuffer;

void init_ringBuffer(ringBuffer* rb, uint32_t bufferSize);

void free_ringBuffer(ringBuffer* rb);

void write_ringBuffer(ringBuffer *rb, t_sample *myData, int numsamples);

void print_ringBuffer(ringBuffer *rb);

void print_array(double* data, int n);


/*
		PART FOR MYFFT OBJECT
*/

static t_class *myfft_tilde_class;
typedef struct _myfft_tilde {
  t_object x_obj;

  t_sample f;
  t_outlet *x_out; // signal output

  ringBuffer buff;
  int bufferSize;


} t_myfft_tilde;

void *myfft_tilde_new(t_symbol *s, int argc, t_atom *argv);
void myfft_tilde_free(t_myfft_tilde *x);
void myfft_tilde_setup(void);

t_int *myfft_tilde_perform(t_int *w);
void myfft_tilde_dsp(t_myfft_tilde *x, t_signal **sp);



#endif