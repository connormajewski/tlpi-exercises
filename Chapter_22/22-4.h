#include <signal.h>

//Had to define this here or else it will cause errors.

#ifndef SIG_HOLD
#define SIG_HOLD (sighandler_t) -1
#endif

typedef void (*sighandler_t)(int);

sighandler_t new_sigset(int sig, sighandler_t disp);

int new_sighold(int sig);

int new_sigrelse(int sig);

int new_sigignore(int sig);

int new_sigpause(int sig);
