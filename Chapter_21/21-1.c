#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/*

	Basic implementation of abort(). Not sure if I handled flushing correct,
	but this should work on a single-threaded process. Basic gist is:

		1 - Unblock and raise SIGABRT.
		2 - Reset its disposition to default.
		3 - Unblock again.
		4 - Flush streams.
		5 - Raise again.
		6 - Force exit if needed.

*/

int sig_helper(int sig);

void new_abort(void);

int sig_helper(int sig){

	sigset_t mask;

	if((sigemptyset(&mask) == -1) || (sigaddset(&mask, sig) == -1) || (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1)) return -1;

	return 0;

}

void new_abort(void){

	struct sigaction action = {0};

	if(sig_helper(SIGABRT) == -1) _exit(127);

	if(raise(SIGABRT) == -1){

		_exit(127);

	}

	action.sa_handler = SIG_DFL;

	if(sigemptyset(&action.sa_mask) == -1) _exit(127);

	action.sa_flags = 0;

	if(sigaction(SIGABRT, &action, NULL) == -1){

		_exit(127);

	}

	if(sig_helper(SIGABRT) == -1) _exit(127);

	fflush(NULL);

	if(raise(SIGABRT) == -1){

		_exit(127);

	}

	_exit(127);

}

int main(int argc, char *argv[]){

	new_abort();

	return 0;

}
