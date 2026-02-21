#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include "22-4.h"

// My attempt at implementing some System-V functions.

sighandler_t new_sigset(int sig, sighandler_t disp){

	struct sigaction action;
	struct sigaction previousAction;

	sigset_t mask;

	sigprocmask(0, NULL, &mask);

	if(sigismember(&mask, sig) && disp != SIG_HOLD){

		return SIG_HOLD;

	}

	action.sa_handler = disp;

	sigemptyset(&action.sa_mask);

	if(disp != SIG_DFL && disp != SIG_IGN && disp != SIG_HOLD){

		sigaddset(&action.sa_mask, sig);

	}

	action.sa_flags = 0;

	if(sigaction(sig, &action, &previousAction) == -1){

		return (sighandler_t) -1;

	}

	return previousAction.sa_handler;

}

int new_sighold(int sig){

	sigset_t currentSet;

	sigemptyset(&currentSet);

	sigaddset(&currentSet, sig);

	if(sigprocmask(SIG_BLOCK, &currentSet, NULL) == -1){

		return -1;

	}

	return 0;

}

int new_sigrelse(int sig){

	sigset_t currentSet;

	sigemptyset(&currentSet);

	sigaddset(&currentSet, sig);

	if(sigprocmask(SIG_UNBLOCK, &currentSet, NULL) == -1){

		return -1;

	}

	return 0;

}

int new_sigignore(int sig){

	struct sigaction action;

	action.sa_handler = SIG_IGN;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	if(sigaction(sig, &action, NULL) == -1){

		return -1;

	}

	return 0;

}

int new_sigpause(int sig){

	sigset_t currentMask;
	sigset_t newMask;

	sigprocmask(0, NULL, &currentMask);

	sigemptyset(&newMask);
	sigaddset(&newMask, sig);

	sigsuspend(&newMask);

	errno = EINTR;

	return -1;

}

int main(int argc, char *argv[]){

	return 0;

}
