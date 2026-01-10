#include <sys/stat.h>
#include <fcntl.h>
#include <sys/resource.h>
#include "tlpi_hdr.h"

int dup(int oldfd);
int dup2(int oldfd, int newfd);

int dup(int oldfd){

	int newfd;

	newfd = fcntl(oldfd, F_DUPFD, 0);

	if(newfd == -1){

		errExit("fcntl newfd");

	}

	printf("New fd: %d\n", newfd);

	return newfd;

}

int dup2(int oldfd, int newfd){

	if(fcntl(oldfd, F_GETFD) == -1){

		printf("Invalid oldfd.\n");

		errno = EBADF;

		return -1;

	}

	if(oldfd == newfd){

		return newfd;

	}

	struct rlimit limit;

	int rlimit = getrlimit(RLIMIT_NOFILE, &limit);

	if(newfd < 0 || newfd >= RLIMIT_NOFILE){

		printf("Invalid newfd.\n");

		errno = EBADF;

		return -1;

	}

	close(newfd);

	newfd = fcntl(oldfd, F_DUPFD, newfd);

	if(newfd == -1){

		printf("Invalid newfd dupe.\n");

		return -1;

	}

	printf("New fd: %d\n", newfd);

	return newfd;

}

int main(int argcc, char * argv[]){

	int fd;

	fd = open("test.txt", O_CREAT | O_WRONLY |  O_APPEND, S_IRUSR |  S_IWUSR | S_IRGRP | S_IWGRP);

	if(fd == -1){

		errExit("open");

	}

	dup(fd);

	dup2(fd, 5);

	dup2(fd, 8);

	exit(EXIT_SUCCESS);

}
