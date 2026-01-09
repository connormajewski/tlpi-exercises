#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "tlpi_hdr.h"

/*

	File made with -x flag are almost twice as large as file without.
	This is because O_APPEND is atomic. The copy using
	lseek() + write() is not atomic, so there are races happening.

	After running, file sizes were as follows:

	f1.txt - 1085284
	f2.txt - 2000000

*/

int appendFlag;
ssize_t bytesWritten;
int fd;

char * filename;
size_t numBytes;

int opt;

int main(int argc, char * argv[]){

	appendFlag = 1;

	if(argc > 4 || argc < 3 || (strcmp(argv[1], "--help") == 0)){

		usageErr("%s filename num-bytes [x]\n", argv[0]);

	}

	while((opt = getopt(argc, argv, "x")) != -1){

		switch(opt){

			case 'x': appendFlag--; break;
			case ':':
			case '?': exit(EXIT_FAILURE);

		}

	}

	filename = argv[optind];

	numBytes = getLong(argv[optind + 1], GN_ANY_BASE, "number-bytes");

	fd = open(argv[optind], O_CREAT | O_WRONLY | (appendFlag ? O_APPEND : O_TRUNC), S_IRUSR | S_IWUSR);

	if(fd == -1){

		errExit("open");

	}

	char c = 'a';

	for(size_t i = 0;i<numBytes;i++){

		if(!appendFlag) lseek(fd, 0, SEEK_END);

		bytesWritten = write(fd, &c, sizeof(c));

		if(bytesWritten == -1){

			errExit("write");

		}

	}

	close(fd);

	exit(EXIT_SUCCESS);

}
