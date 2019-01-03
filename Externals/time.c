#include <time.h>
#include <stdio.h>

int main(void)
{
	int a = 100;
	while (a > 10){
	    time_t mytime = time(NULL);
		char* time_str = malloc(8);
		strncpy(time_str, ctime(&mytime) + 11, 8);

	    printf(time_str);
	    printf("\n");

	    free(time_str); //Libération des 20 octets précédemment alloués
    	time_str = NULL; // Invalidation du pointeur
	    a --;
	}

    return 0;
}
