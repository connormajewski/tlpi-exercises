#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "tlpi_hdr.h"

int fd1;
int fd2;

/*

	offsetFlagCheck - Function for checking offset and open file flags
	of two file descriptor.

	INPUT

		int fd1 - First file descriptor.

		int fd2 - Second file desscriptor.

	RETURN

		0 - Offset and open file flags are equal.

		1 - Not equal

		-1 - Error.

*/

int offsetFlagCheck(int fd1, int fd2){

	int fd1Flags;
	int fd2Flags;

	off_t fd1Offset;
	off_t fd2Offset;

	fd1Flags = fcntl(fd1, F_GETFL);

	if(fd1Flags == -1){

		return -1;

	}

	fd2Flags = fcntl(fd1, F_GETFL);

	if(fd2Flags == -1){

		return -1;

	}

	fd1Offset = lseek(fd1, 0, SEEK_CUR);

	if(fd1Offset == -1){

		return -1;

	}

	fd2Offset = lseek(fd2, 0, SEEK_CUR);

	if(fd2Offset == -1){

		return -1;

	}

	if(fd1Flags != fd2Flags || fd1Offset != fd2Offset) return 1;

	return 0;

}

int main(int argc, char * argv[]){

	fd1 = open("fd1", O_CREAT | O_TRUNC | O_WRONLY , S_IRUSR | S_IWUSR);

	if(fd1 == -1){

		errExit("Creating fd1.");

	}

	fd2 = dup(fd1);

	if(fd2 == -1){

		errExit("Creating fd1.");

	}

	int check = offsetFlagCheck(fd1, fd2);

	printf("RESULT: %s\n", !check ? "EQUAL" : "NOT EQUAL");

	exit(EXIT_SUCCESS);

}
