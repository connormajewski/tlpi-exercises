#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define MAX_LINE_LEN 4096

struct linePointer{

	off_t offset;
	size_t len;

};

int tail(char * file, int lines){

	char lineBuffer[MAX_LINE_LEN];

	int fd;

	int f;

	ssize_t bytesRead;
	off_t currentOffset = 0;
	off_t previousOffset = 0;

	int lineCount = 0;

	struct linePointer * lineTrack = malloc(lines * sizeof(struct linePointer));

	fd = open(file, O_RDONLY);

	if(fd == -1){

		printf("Error opening %s, or it does not exist.\n", file);

		return -1;

	}

	while((bytesRead = read(fd, lineBuffer, MAX_LINE_LEN)) != 0){

		if(bytesRead == -1){

			printf("read() error.\n");

			return -1;

		}

		for(size_t i=0;i<bytesRead;i++){

			if(lineBuffer[i] == '\n'){

				struct linePointer temp;
				temp.offset = previousOffset;
				temp.len = currentOffset + 1;

				previousOffset += currentOffset;
				currentOffset = 0;

				lineTrack[lineCount] = temp;

				lineCount = (lineCount + 1) % lines;

			}

			currentOffset++;

		}

	}

	off_t off = lseek(fd, lineTrack[lineCount % lines].offset + 1, SEEK_SET);

	if(off == (off_t) -1){

		printf("lseek() error.\n");

		return -1;

	}

	while((bytesRead = read(fd, lineBuffer, MAX_LINE_LEN)) != 0){

		if(bytesRead == -1){

			printf("read() error.\n");

			return -1;

		}

		size_t bytesWritten = write(1, lineBuffer, bytesRead);

		if(bytesWritten == -1){

			printf("write() error.\n");

			return -1;

		}

	}

	free(lineTrack);

	f = close(fd);

	if(f){

		if(errno = EOF) printf("EOF.\n");

		else printf("ERROR.\n");

		return -1;

	}

	return 0;

}

int main(int argc, char * argv[]){

	int opt;
	int lines = 10;

	char *ptr;
	char *line;

	while((opt = getopt(argc, argv, ":n:")) != -1){

		switch(opt){

			case 'n':
				line = optarg;

				lines = (int) strtol(line, &ptr, 10);

				if((*ptr != '\0') || (strcmp(ptr, line) == 0)){

					printf("Number could not be parsed.\n");

					return -1;

				}

				break;
			case ':':
				printf("Need value.\n");
				break;
			case '?':
				printf("Unknown option %d.\n", optopt);
				break;
		}

	}

	if(optind == argc){

		printf("File must be given.\n");

		return -1;

	}

	tail(argv[optind], lines);

	return 0;

}
