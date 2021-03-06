#ifndef _MYSYNTH1_
# define _MYSYNTH1_

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
		PART FOR MYSYNTH1 OBJECT
*/

static t_class *mysynth1_tilde_class;
typedef struct _mysynth1_tilde {
  t_object x_obj;

  t_sample f;
  t_outlet *x_out; // signal output
  t_inlet *in2; // input of the second signal


  ringBuffer buff1;
  ringBuffer buff2;
  int bufferSize;
  float threshold;
  float bypass;


} t_mysynth1_tilde;

void *mysynth1_tilde_new(t_symbol *s, int argc, t_atom *argv);
void mysynth1_tilde_free(t_mysynth1_tilde *x);
void mysynth1_tilde_setup(void);

t_int *mysynth1_tilde_perform(t_int *w);
void mysynth1_tilde_dsp(t_mysynth1_tilde *x, t_signal **sp);

void mysynth1_set_threshold(t_mysynth1_tilde *x, t_floatarg f);
void mysynth1_set_bypass(t_mysynth1_tilde *x, t_floatarg f);



#endif