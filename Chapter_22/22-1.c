#include <signal.h>
#include <stdio.h>
#include <unistd.h>

/*

	This program shows that SIGCONT is only handled once it is unblocked.
	Stopping and resuming program sends a SIGCONT, and we can see that the
	handler only executes once sigprocmask() is called to unblock.

*/

void handler(int sig){

	printf("RECEIVED: %d\n", sig);

}

int main(int argc, char * argv[]){

	sigset_t signalSet;

	if(signal(SIGCONT, handler) == SIG_ERR) perror("signal()");

	if(sigemptyset(&signalSet) == -1) perror("sigemptyset()");

	if(sigaddset(&signalSet, SIGCONT) == -1) perror("sigaddset()");

	printf("Blocking SIGCONT.\n");

	if(sigprocmask(SIG_BLOCK, &signalSet, NULL) == -1) perror("sigprocmask()");

	printf("Process sleeping...\n");

	sleep(10);

	printf("Unblocking SIGCONT.\n");

	if(sigprocmask(SIG_UNBLOCK, &signalSet, NULL) == -1) perror("sigprocmask()");

	return 0;

}
