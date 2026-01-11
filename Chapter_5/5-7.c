#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <stdio.h>
#include "tlpi_hdr.h"

#define IOV_MAX 8
#define IOVCNT 2

struct iovec iov[IOVCNT];

int fd;
int accessFlags;

ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
ssize_t writev(int fd, const struct iovec *iov, int iovcnt);

ssize_t readv(int fd, const struct iovec *iov, int iovcnt){

	ssize_t totalBytesRead = 0;

	int fdFlags = fcntl(fd, F_GETFL);

	if(fdFlags == -1){

		errExit("Getting open file status flags.\n");

	}

	accessFlags = fdFlags & O_ACCMODE;

	if(accessFlags != O_RDWR && accessFlags != O_WRONLY){

		errExit("File not writable.\n");

	}

	if(iovcnt > IOV_MAX){

		errExit("iovcnt exceeds max.\n");

	}

	for(int i=0;i<iovcnt;i++){

		ssize_t remainingBytes = iov[i].iov_len;
		char *ptr = iov[i].iov_base;

		while(remainingBytes > 0){

			ssize_t bytesRead = read(fd, ptr, remainingBytes);

			if(bytesRead == -1){

				if(errno == EINTR) continue;

				errExit("read()\n");

			}

			if(bytesRead == 0) return totalBytesRead;

			ptr += bytesRead;

			remainingBytes -= bytesRead;

			totalBytesRead += bytesRead;

		}

	}

	return totalBytesRead;

}

ssize_t writev(int fd, const struct iovec *iov, int iovcnt){

	ssize_t totalBytesWritten = 0;

	int fdFlags = fcntl(fd, F_GETFL);

	if(fdFlags == -1){

		errExit("Getting open file status flags.\n");

	}

	accessFlags = fdFlags & O_ACCMODE;

	if(accessFlags != O_RDWR && accessFlags != O_WRONLY){

		errExit("File not writable.\n");

	}

	if(iovcnt > IOV_MAX){

		errExit("iovcnt exceeds max.\n");

	}

	if(fd > 2 && (lseek(fd, 0, SEEK_SET)) == -1) errExit("seek()\n");

	for(int i=0;i<iovcnt;i++){

		ssize_t remainingBytes = iov[i].iov_len;
		char *ptr = iov[i].iov_base;

		while(remainingBytes > 0){

			ssize_t bytesWritten = write(fd, ptr, remainingBytes);

			if(bytesWritten == -1){

				if(errno == EINTR) continue;

				errExit("write()\n");

			}

			ptr += bytesWritten;

			remainingBytes -= bytesWritten;

			totalBytesWritten += bytesWritten;

		}

	}

	return totalBytesWritten;

}

int main(int argc, char * argv[]){

	if(argc != 2 || strcmp(argv[1], "--help") == 0){

		usageErr("%s filename\n", argv[0]);

	}

	fd = open(argv[1], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	if(fd == -1){

		errExit("Creating dile descriptor.\n");

	}

	struct iovec iov[IOVCNT];

	char *str0 = (char *) calloc(IOV_MAX, sizeof(char));
	char *str1 = (char *) calloc(IOV_MAX, sizeof(char));

	iov[0].iov_base = str0;
	iov[0].iov_len = IOV_MAX;

	iov[1].iov_base = str1;
	iov[1].iov_len = IOV_MAX;

	ssize_t bytes = readv(fd, iov, IOVCNT);

	printf("%lu bytes read.\n", bytes);

	bytes = writev(1, iov, IOVCNT);

	printf("%lu bytes written.\n", bytes);

	exit(EXIT_SUCCESS);

}
