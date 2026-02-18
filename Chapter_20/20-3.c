#include <signal.h>
#include <stdio.h>
#include <unistd.h>

/*

	When SA_RESETHAND is set, SIGINT should run handler function once, then perform
	normally on all subsequent uses.

	When SA_NODEFER is set, handler should be able to interrupt itself,
	meaning multiple SIGINTs in a rowshould "stack", instead of running sequentially.

*/

void handler(int sig){

	printf("ENTER\n");

	sleep(2);

	printf("EXIT\n");

}

int main(int argc, char *argv[]){

	sigset_t signalSet;

	struct sigaction action;

	int flag;

	if(argc != 2){

		printf("Usage: ./20-3 [0-1] (1 - SA_RESETHAND, 0 - SA_NODEFER)\n");

		return -1;

	}

	flag = argv[1][0] - '0';

	for(int i=0;i<NSIG;i++) signal(i, handler);

	sigemptyset(&signalSet);

	if(sigprocmask(SIG_SETMASK, &signalSet, NULL) == -1){

		perror("sigprocmask()");

		return -1;

	}

	action.sa_handler = &handler;
	action.sa_flags = flag ? SA_RESETHAND : SA_NODEFER;

	printf("SET SIGINT FLAG %s.\n", flag ? "SA_RESETHAND" : "SA_NODEFER");

	if(sigaction(SIGINT, &action, NULL) == -1){

		perror("sigaction()");

		return -1;

	}

	while(1);

	return 0;

}
