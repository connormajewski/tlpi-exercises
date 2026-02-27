#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

/*

	When close(1) is called in child process, the write() immediately after
	does not show on terminal. The write() in parent process, does, however.
	This shows that the change to the child's file descriptor did not affect
	the parent's.

*/

int main(int argc, char *argv[]){

	pid_t childStatus;

	childStatus = vfork();

	switch(childStatus){

		case 0:

			// These writes should show on terminal.

			if(close(1) == -1) write(1, "Error.\n", 8);;
			write(1, "Closed.\n", 9);
			_exit(EXIT_SUCCESS);
			break;

		case -1:
			perror("vfork()");
			break;

		default:
			sleep(1);

			// This one should, since this is parent stdout.

			printf("Testing stdout in parent.\n");
			exit(EXIT_SUCCESS);
			break;

	}

	return 0;

}
