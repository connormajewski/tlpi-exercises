#include <signal.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

/*

	Need two programs for this. One to send signals, and one to receive.
	I think basic gist is to block all signals, sleep for some time, and
	run second program to send a bunch of pending signals. Unblock, then
	just check handler output and see. Might use stdio functions for
	that even though they are not safe.

*/

void handler(int sig){

	// This shouldn't be used, but I am just to keep it simple.

	printf("RECEIVED %d.\n", sig);

}

int main(int argc, char * argv[]){

	sigset_t signalMask;

	for(int i=0;i<NSIG;i++) if(i != SIGINT && i != 15) signal(i, handler);

	sigfillset(&signalMask);

	sigprocmask(SIG_SETMASK, &signalMask, NULL);

	printf("Blocking all signals...\n");

	sleep(15);

	printf("Unblocking all signals.\n");

	sigprocmask(SIG_UNBLOCK, &signalMask, NULL);

	while(1) {}

}
