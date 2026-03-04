#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

/*

	Program that just forks and kills child in a loop, waiting each time before continuing.
	I just wrote a simple Makefile that ran each loop with increasing values for n.

	Haven't tested it extensively yet, but vfork() seems to be ~4-5 times
	faster than fork(). for values up to 100000

*/

int main(int argc, char *argv[]){

	if(argc < 2){

		printf("Usage: ./28-1 n [flag for vfork].\n");

		return -1;

	}

	int loop = atoi(argv[1]);

	pid_t status;

	printf("Running %s %d times.\n", argc > 2 ? "vfork()" : "fork()", loop);

	for(int i=0;i<loop;i++){

		switch(argc > 2 ? vfork() : fork()){
			case -1:
				perror("forking function");
				return -1;
				break;
			case 0:
				exit(EXIT_SUCCESS);
			default:
				while((status = wait(NULL)) != -1) continue;

		}

	}

	return 0;

}
