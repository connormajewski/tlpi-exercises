#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

/*

	In my case, the orphaned child's parent does not become init. Instead, it's
	parent becomes systemd --user. Checking ppid with ~ps -fp ppid confirms this.

*/

int main(int argc, char *argv[]){

	pid_t pid;

	pid = fork();

	switch(pid){

		case -1:
			perror("fork()");
			return -1;
			break;
		case 0:
			sleep(1);
			printf("PPID: %d\n", getppid());
			break;
		default:
			exit(EXIT_SUCCESS);
	}

	return 0;

}
