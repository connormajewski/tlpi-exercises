#include <sys/resource.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t wstatus;
struct rusage usage;

/*

	For this, I am just execing command and arguments, waiting for child, then calling
	getrusage to get some resource data.

*/

void printUsage(struct rusage *usage){

	printf("USER TIME: %ld.%ld\nCPU TIME: %ld.%ld\n",
		usage->ru_utime.tv_sec,
		usage->ru_utime.tv_usec,
		usage->ru_stime.tv_sec,
		usage->ru_stime.tv_usec
	);

	printf("\nTEXT: %ld\nDATA: %ld\nSTACK: %ld\n",
		usage->ru_ixrss,
		usage->ru_idrss,
		usage->ru_isrss
	);

}

int main(int argc, char *argv[]){

	if(argc < 3){

		printf("Usage: %s command arg ...\n", argv[0]);

		return -1;

	}

	switch(fork()){

		case -1:
			printf("fork() error.\n");
			break;
		case 0:
			execvp(argv[1], &argv[2]);
			printf("FAIL.\n");
			break;
		default:
			wait(&wstatus);

			getrusage(RUSAGE_CHILDREN, &usage);

			printUsage(&usage);

			break;

	}

	return 0;

}
