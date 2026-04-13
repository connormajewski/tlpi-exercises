#include <stdio.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/*

	Basic program to test resource limits. From what I can tell, at least for open file descriptors,
	changing the resource limit does not affect current resource, only new ones.
	The first fd still works properly, but the second one will not.

*/

struct rlimit rlim;

int fd;

int main(int argc, char *argv[]){

	fd = open("test.txt", O_CREAT, S_IRWXU | S_IRWXG);

 	getrlimit(RLIMIT_NOFILE, &rlim);

	printf("FD LIMIT: %ld\n", rlim.rlim_max);

	rlim.rlim_cur = 0;
	rlim.rlim_max = 0;

	setrlimit(RLIMIT_NOFILE, &rlim);

 	getrlimit(RLIMIT_NOFILE, &rlim);

	printf("FD LIMIT: %ld\n", rlim.rlim_max);

	close(fd);

	fd = open("t.txt", O_CREAT, S_IRWXU | S_IRWXG);

	if(fd == -1 && errno == EMFILE) printf("fd process limit reached.\n");

	return 0;

}
