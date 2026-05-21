#include <termios.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

/*

	Reading tcgetattr() description, I saw that if fd does not refer to terminal, it fails,
	and errno is set to ENOTTY. Knowing that, I just call it in my clone function
	and check if it fails with that errno. Otherwise I return 1.

	Can see that 0,1,2 all return 1, since they are terminal I/Os.
	The text file returns 0 since it is not a terminal.

*/

int isatty_clone(int fd);

int isatty_clone(int fd){

	struct termios t;

	if(tcgetattr(fd, &t) == -1){

		// Don't need to set errno, it is set by tcgetattr().

		return 0;

	}

	return 1;

}

int main(int argc, char *argv[]){

	// Check function on stdin, stdout, stderr, and random file.

	int fd = open("test.txt", O_CREAT | S_IRWXU | S_IRWXG);

	if(fd == -1){

		perror("open()");

	}

	printf("%d: %d\n", fd, isatty_clone(fd));

	for(int i=0;i<3;i++){

		printf("%d: %d\n", i, isatty_clone(i));

	}

	close(fd);

	return 0;

}
