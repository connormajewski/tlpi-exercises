#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

/*

	When this runs, it gives output like:

		129652509107904: 0	1
		129652517500608: 1	0

	Two threads, can see that they have distince pending signals.

*/

void * thread_function(void *arg){

	sigset_t mask;

	sleep(3);

	sigpending(&mask);

	// Shouldn't do this but just for testing.

	printf("%ld: %d\t%d\n", pthread_self(), sigismember(&mask, SIGUSR1),  sigismember(&mask, SIGUSR2));

}

int main(int argc, char *argv[]){

	pthread_t thread_1;
	pthread_t thread_2;

	sigset_t sigmask;

	sigemptyset(&sigmask);

	sigaddset(&sigmask, SIGUSR1);
	sigaddset(&sigmask, SIGUSR2);

	pthread_sigmask(SIG_BLOCK, &sigmask, NULL);

	pthread_create(&thread_1, NULL, thread_function, NULL);
	pthread_create(&thread_2, NULL, thread_function, NULL);

	pthread_kill(thread_1, SIGUSR1);
	pthread_kill(thread_2, SIGUSR2);

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);

	return 0;


}
