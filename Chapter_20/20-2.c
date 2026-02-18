#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

/*

	This is a simple program to check that the disposition change to SIG_IGN works.
	I sleep for 5 seconds, during which I send some signals. I then print set of
	pending signals, and change SIGINT to be ignored. Once signals are unblocked,
	handler() will be called automatically for each unblocked signal, except for
	SIGINT.

*/

static volatile sig_atomic_t sigInt = 0;

void printSigSet(const char * prefix, const sigset_t * sigset){

	int sig;
	int cnt;

	cnt = 0;

	struct sigaction action;

	for(sig=1;sig<NSIG;sig++){

		if(sigismember(sigset, sig)){

			cnt++;

			printf("%s%d: (%s)\n", prefix, sig, strsignal(sig));

			// Right here, I am setting disposition to be ignored.

			if(sig == SIGINT){

				action.sa_handler = SIG_IGN;

				if(sigaction(sig, &action, NULL) == -1){

					perror("sigaction()");

				}

			}

		}

	}

	if(cnt == 0) printf("Empty signal set.\n");

}

static void handler(int sig){

	printf("%d\n", sig);

}

int main(int argc, char *argv[]){

	sigset_t signalSet;
	sigset_t pendingSet;
	sigset_t emptySet;

	for(int i=0;i<NSIG;i++) (void) signal(i, handler);

	sigfillset(&signalSet);

	if(sigprocmask(SIG_SETMASK, &signalSet, NULL) == -1){

		perror("sigprocmask()");

		return -1;

	}

	printf("Sleeping for 5 seconds.\n");

	sleep(5);

	printf("Finished sleeping.\n");

	if(sigpending(&pendingSet) == -1){

		perror("sigpending()");

	}

	printf("PENDING SIGNALS:\n");
	printSigSet("", &pendingSet);

	printf("\nRECEIVED SIGNALS:\n");

	sigemptyset(&emptySet);

	if(sigprocmask(SIG_SETMASK, &emptySet, NULL) == -1){

		perror("sigprocmask()");

		return -1;

	}

	return 0;

}
