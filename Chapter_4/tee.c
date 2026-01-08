#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include "tlpi_hdr.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

#define printable(ch) (isprint((unsigned char) ch) ? ch : '#')

// option flag specific variables.

int opt;
char *fileString;
int aFlag;
// tee specific variables.

ssize_t bytesRead, bytesWritten;
char buffer[BUFFER_SIZE];
int fileDescriptor;
char * filepath;
int isFileExisting;

static void usageError(char *progName, char *msg, int opt){

	if(msg != NULL && opt != 0){

		fprintf(stderr, "%s (-%c)\n", msg, printable(opt));

	}

	fprintf(stderr, "Usage: %s [-a] filename\n", progName);

	exit(EXIT_FAILURE);

}

int main(int argc, char *argv[]){

	isFileExisting = 0;

	aFlag = 0;

	fileString = NULL;

	if(argc == 2){

		fileString = argv[1];

	}

	while((opt = getopt(argc, argv, "a:")) != -1){

		switch(opt){

			case 'a': fileString = optarg; aFlag = 1; break;
			case '?': usageError(argv[0], "Missing argument", optopt);
			case ':': usageError(argv[0], "Unrecognized argument", optopt);
			default: fatal("Unexpected case in switch().");

		}
	}

	if(fileString != NULL){

		int flags = O_WRONLY | O_CREAT;

		flags |= (aFlag ? O_APPEND : O_TRUNC);

		umask(0);

		fileDescriptor = open(fileString, flags, S_IRUSR | S_IWUSR| S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

		if(fileDescriptor == -1){

			errExit("open");

		}

		bytesRead = read(STDIN_FILENO, buffer, BUFFER_SIZE);

		if(bytesRead == -1){

			errExit("read");

		}


		bytesWritten = write(fileDescriptor, buffer, strlen(buffer));

		if(bytesWritten == -1){

			errExit("write");

		}

		close(fileDescriptor);


	}

	exit(EXIT_SUCCESS);

}
