#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

/*

	From what I can tell, the grandchild becomes adopted immediately
	after the parent process terminates. Not adopted by init, but by
	systemd --user. I used procexec/orphan.c as a guide.

*/

int main(int argc, char *argv[]){

	pid_t original = getpid();

	setbuf(stdout, NULL);

	switch(fork()){

		case -1:
			return -1;
		case 0:
			pid_t parent;

			switch(fork()){

				case -1:
					return -1;
				case 0:
					pid_t child = getppid();

					while((parent = getppid()) == child){
						printf("PPID: %ld\n", (long) parent);
						sleep(1);
					}
					printf("PPID: %ld\n", (long) parent);


				default:
					sleep(3);
					exit(EXIT_SUCCESS);

			}
		default:
			sleep(3);
			exit(EXIT_SUCCESS);
	}

	return 0;

}
