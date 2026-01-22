#include <time.h>
#include "tlpi_hdr.h"

int main(int argc, char * argv[]){

	time_t tt;

	time_t t = time(NULL);

	if(t == ((time_t) -1)){

		printf("time error.\n");

		return -1;

	}

	printf("%ld\n", t);

	t = time(&tt);

	if(t == ((time_t) -1)){

		printf("time error.\n");

		return -1;

	}

	printf("%ld\n%ld\n", t, tt);

	char * b = ctime(&tt);

	if(b == NULL){

		printf("Error ctime().\n");

	}

	printf("%s", b);

	printf("%s\n", asctime(gmtime(&tt)));

	return 0;
}
