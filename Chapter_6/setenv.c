#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "tlpi_hdr.h"

extern char ** environ;

int new_setenv(const char * name, const char * value, int overwrite);
int new_unset_env(const char * name);

int new_setenv(const char * name, const char * value, int overwrite){

	int appendFlag = 1;

	if(*name == '\0'){

		errno = EINVAL;

		return -1;

	}

	const char * tmpName = name;

	while(*tmpName != '\0'){

		if(*tmpName == '='){

			errno = EINVAL;

			return -1;

		}

		tmpName++;

	}

	int environLength = 0;

	char ** tmp;

	for(tmp = environ; *tmp != NULL; tmp++){

		environLength++;

	}

	for(int i=0;i<environLength;i++){

		if(strncmp(environ[i], name, strlen(name)) == 0 && environ[i][strlen(name)] == '='){

			if(overwrite){

				free(environ[i]);

				char * buffer = (char *) malloc((strlen(name) + strlen(value) + 2) * sizeof(char));

				snprintf(buffer, strlen(buffer), "%s=%s", name, value);

				environ[i] = buffer;

				appendFlag--;
			}

			else{

				return 0;

			}

		}


	}

	char ** tmpEnviron = malloc((environLength + appendFlag ? 1 : 0) * sizeof(char*));

	if(tmpEnviron == NULL){

		return -1;

	}

	int i;

	for(i=0;i<environLength;i++){

		if(overwrite){

			char * n = (char *) malloc((strlen(name) + 2) * sizeof(char));

			snprintf(n, strlen(name) + 2,  "%s%s", name, "=");

		}

		char * tmp = strdup(environ[i]);

		if(tmp == NULL){

			return -1;

		}

		tmpEnviron[i] = tmp;

	}

	if(appendFlag){

		int length = strlen(name) + strlen(value) + 2;

		char * buffer = (char *) malloc(length * sizeof(char));

		snprintf(buffer, length, "%s=%s", name, value);

		tmpEnviron[i] = buffer;

		tmpEnviron[i + 1] = NULL;

	}

	environ = tmpEnviron;

	return 0;

}


int main(int argc, char * argv[]){

	char * name = "TEST";
	char * value = "TEST";

	char * name1 = "";
	char * value1 = "TEST";

	printf("%d\n", 	new_setenv(name, value, 0));
	printf("%d\n", 	new_setenv(name, value, 0));
	printf("%d\n", 	new_setenv(name1, value, 0));

	exit(EXIT_SUCCESS);

}
