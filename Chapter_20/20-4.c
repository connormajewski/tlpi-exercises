#include <signal.h>
#include <stdio.h>

/*

	Followed what Section 21.5 says on siginterrupt(). Grab current disposition of
	signal using sigaction(), modify it based on flag, then pass disposition back
	to sigaction as act.

*/

int new_siginterrupt(int sig, int flag);

int new_siginterrupt(int sig, int flag){

	struct sigaction oldAction;

	if(sigaction(sig, NULL, &oldAction) == -1){

		perror("grabbing current sigaction()");

		return -1;

	}

	if(flag == 1){

		oldAction.sa_flags &= ~SA_RESTART;

	}

	else {

		oldAction.sa_flags |= SA_RESTART;

	}

	if(sigaction(sig, &oldAction, NULL) == -1){

		perror("resetting flags sigaction()");

		return -1;

	}

	return 0;

}

int main(int argc, char *argv[]){

	// Here I just do quick call to make sure it runs.

	if(new_siginterrupt(SIGINT, 0) == -1){

		printf("new_siginterrupt() failed.\n");

	}

	printf("new_siginterrupt() succeeded.\n");

	return 0;

}
