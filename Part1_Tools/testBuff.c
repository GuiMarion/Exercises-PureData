#include "ringBuffer/ringBuffer.h"

#include <stdio.h>

int main(){

	ringBuffer r;
	init_ringBuffer(&r, 16);

	int i;
	for (i = 0; i < 100; i++){

		double temp[4];

		int k;
		for (k = 0; k < 4; k++){
			temp[k] = (double)i;
		}

		write_ringBuffer(&r, temp, 4);
	}

	print_ringBuffer(&r);

	printf("kikou \n");
	return 1;

}
