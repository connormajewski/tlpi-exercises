#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[]){

	if(argc != 3 || strcmp(argv[1], "help") == 0){

		usageErr("%s [-a] file\n", argv[0]);

	}

	exit(EXIT_SUCCESS);

}
