#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

struct rusage usage;
pid_t wstatus;

/*

	We can see that when getrusage() is called the first time, its ru_utime values will
	both be 0. When re call wait() and try again, the values are updated, proving that the flag only works on children for which wait() was called.

*/

int main(int argc, char *argv[]){

	switch(fork()){

		case -1:
			printf("fork() error.\n");
			break;
		case 0:
			for(int i=0;i<1000000000;i++){

				int j = i + i;

			}
			break;
		default:
			getrusage(RUSAGE_CHILDREN, &usage);

			printf("TIME: %ld.%ld\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);

			wait(&wstatus);

			getrusage(RUSAGE_CHILDREN, &usage);

			printf("TIME: %ld.%ld\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);

			break;
	}


	return 0;

}
