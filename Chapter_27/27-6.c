#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

/*

	From what I can see, assuming I did this correct, is that the SIGCHLD is
	delivered on the unblock. Not sure if I was supposed to do something else here
	or not.

*/

void handler(int signal){

	if(signal == SIGCHLD) write(1, "CHILD\n", 7);

}

int main(int argc, char *argv[]){

	sigset_t sigset;

	struct sigaction action;

	if(sigemptyset(&sigset) == -1){

		perror("sigemptyset()");

		return -1;

	}


	if(sigaddset(&sigset, SIGCHLD) == -1){

		perror("sigaddset()");

		return -1;

	}

	if(sigprocmask(SIG_BLOCK, &sigset, NULL) == -1){

		perror("sigprocmask()");

		return -1;

	}

	action.sa_handler = handler;
	action.sa_flags = 0;

	printf("SIGCHLD blocked.\n");

	if(sigaction(SIGCHLD, &action, NULL) == -1){

		perror("sigaction()");

		return -1;

	}

	printf("SIGCHLD handler set.\n");

	// After blocking and setting handler for SIGCHLD, fork() and kill.

	int status;

	switch(fork()){

		case -1:
			perror("fork()");
			return -1;
			break;
		case 0:
			exit(EXIT_SUCCESS);
		default:
			while((status = wait(NULL)) == -1) continue;

			if(sigprocmask(SIG_UNBLOCK, &sigset, NULL) == -1){

				perror("sigprocmask() unblock");

				return -1;

			}

	}

	printf("Finished.\n");

	return 0;

}
