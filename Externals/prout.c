#include <math.h>
#include <stdio.h>
#include "fft4g_h.c"


#define PI 3.14159265358979323846

int main(){

  int n = 64;

  int T = n;
  int t;
  double fft_array[64];

  for(t=0; t<T; t++){

    fft_array[t] = 0.42 - 0.5*cos(2*PI*t/T) + 0.08*cos(4*PI*t/T);
    fft_array[t] *= sin(t);

  }

  rdft(T, 1, fft_array);

  int i;
  for(i=0; i<n; i++)
  {
    //out[i]=fft_array[i];
    printf("%f ",fft_array[i]);
  }

}
