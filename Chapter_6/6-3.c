#include <unistd.h>
#include <stdlib.h>
#include "tlpi_hdr.h"

extern char ** environ;

int new_setenv(const char * name, const char * value, int overwrite);
int new_unsetenv(const char * name);
int validateName(const char * name);


int validateName(const char * name){

	if(*name == '\0'){

		return -1;

	}

	for(const char * tmp = name; *tmp != '\0'; tmp++){

		if(*tmp == '='){

			return -1;

		}

	}

	return 0;

}

int new_setenv(const char * name, const char * value, int overwrite){

	char * pointer;

	if(validateName(name) == -1){

		errno = EINVAL;

		return -1;

	}

	pointer = getenv(name);

	if((pointer != NULL && overwrite) || pointer == NULL){

		int length = strlen(name) + strlen(value) + 2;

		char * buffer = (char *) malloc(length * sizeof(char));

		int s = snprintf(buffer, length, "%s=%s", name, value);

		if(s == -1){

			return -1;

		}

		int ret = putenv(buffer);

		if(ret == -1){

			errno = ENOMEM;

			return -1;

		}

	}

	return 0;

}

int new_unsetenv(const char * name){

	char * pointer;

	if(validateName(name) == -1){

		errno = EINVAL;

		return -1;

	}

	int length = strlen(name);

	int i, j;

	for(i=0;environ[i] != NULL; ){

		if((strncmp(environ[i], name, length) == 0) && (environ[i][length] == '=')){

			int j = i;

			while(environ[j] != NULL){

				environ[j] = environ[j + 1];

				j++;

			}


		}

		else{

			i++;

		}

	}

	return 0;

}

int main(int argc, char * argv[]){

	new_setenv("A", "B", 0);
	new_setenv("B", "C", 0);
	new_setenv("C", "D", 0);

	printf("%s\n", getenv("A"));

	new_setenv("A", "C", 1);

	printf("%s\n", getenv("A"));

	new_setenv("A", "D", 1);

	printf("%s\n", getenv("A"));

	int a = new_unsetenv("A");

	if(a == -1){

		errExit("new_unsetenv()");

	}

	printf("%p\n", getenv("A"));

	exit(EXIT_SUCCESS);

}
