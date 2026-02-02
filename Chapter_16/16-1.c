#include <sys/xattr.h>
#include <stdio.h>
#include <string.h>

// Not sure if this is what exercise is supposed to look like, but it works.

int main(int argc, char * argv[]){

	int setCheck;

	if(argc != 4 || (strcmp(argv[1], "--help") == 0)){

		printf("Usage: ./16-1 name value file.\n");

		return -1;

	}

	// Check that only user EAs are being created.

	char * userCheck = strstr(argv[1], "user.");

	if(userCheck == NULL){

		printf("EA must be user.\n");

		return -1;

	}

	setCheck = setxattr(argv[3], argv[1], argv[2], strlen(argv[2]), 0);

	if(setCheck == -1){

		perror("setxattr() ");

		return -1;

	}

	printf("SUCCESS.\n");

	return 0;

}
