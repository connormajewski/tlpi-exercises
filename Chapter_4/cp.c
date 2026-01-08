#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

char buffer[BUFFER_SIZE];
int originalFileDescriptor;
int copiedFileDescriptor;

ssize_t bytesRead;
ssize_t bytesWritten;

int main(int argc, char *argv[]){

	if(argc != 3 || strcmp(argv[1], "--help") == 0){

		usageErr("%s file file\n", argv[0]);

	}

	originalFileDescriptor = open(argv[1], O_RDWR);

	if(originalFileDescriptor == -1){

		errExit("open");

	}

// ADD NULL HOLES TO FILE.

	lseek(originalFileDescriptor, 10000, SEEK_END);

	write(originalFileDescriptor, "hello", strlen("hello"));

	lseek(originalFileDescriptor, 0, SEEK_SET);

	printf("%s", argv[2]);

	copiedFileDescriptor = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR |  S_IWGRP | S_IRGRP | S_IROTH | S_IWOTH);

	if(copiedFileDescriptor == -1){

		errExit("open copied");

	}

	while((bytesRead = read(originalFileDescriptor, buffer, BUFFER_SIZE)) != 0){

		if(bytesRead == -1){

			errExit("read");

		}

		bytesWritten = write(copiedFileDescriptor, buffer, bytesRead);

		if(bytesWritten == -1 || bytesWritten != bytesRead){

			errExit("write");

		}

	}

	close(originalFileDescriptor);
	close(copiedFileDescriptor);

	exit(EXIT_SUCCESS);

}

