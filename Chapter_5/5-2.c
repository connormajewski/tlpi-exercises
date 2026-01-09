#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

/* Changes made to file offset are overridden by O_APPEND flag. */

int fd;
char * data;
ssize_t bytesWritten;

int main(int argc, char *argv[]){

	data = "Hello World!\n";

	if(argc != 2 || strcmp(argv[1], "--help") == 0){

		usageErr("%s filename\n", argv[0]);

	}

	fd = open(argv[1], O_WRONLY | O_APPEND);

	if(fd == -1){

		errExit("open");

	}

	if(lseek(fd, 0, SEEK_SET) == -1){

		errExit("seek");

	}

	bytesWritten = write(fd, data, strlen(data));

	if(bytesWritten == -1){

		errExit("write");

	}

	close(fd);

	exit(EXIT_SUCCESS);

}
