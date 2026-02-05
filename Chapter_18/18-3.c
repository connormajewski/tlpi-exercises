#include <unistd.h>
#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>

#define MAX_CWD_LEN 4096
#define MAX_NAME_LEN 256
#define MAX_DIRECTORY_COUNT 256

/*

	First working version of realpath(). Handles absolute and relative symbolic links, as well aas '.' and '..'.
	A lot of work to be done. There is a lot of duplicated code I need to handle better. For the most part, I believe it works.

	ARGS:
		const char * pathname - pathname given in command line.
		char * resolved_path - string to be returned for later use.

	RETURN:
		char * resolved_path - string containing path after links and other things are handled.
		NULL - error either in new_realpath() or getDirs(). True path cannot be made.

*/

char * getDirs(const char * pathname, char * resolved_path);

char * new_realpath(const char *pathname, char * resolved_path);

char * new_realpath(const char *pathname, char * resolved_path){

	int relativePath = 0;

	char cwd[MAX_CWD_LEN];
	char path[MAX_CWD_LEN];

	if(pathname[0] != '/') relativePath++;

	if(!relativePath){

		resolved_path = getDirs(pathname, resolved_path);

	}

	else{

		if(getcwd(cwd, MAX_CWD_LEN) == NULL){

			perror("getcwd()");

			return NULL;

		}

		strlcpy(path, cwd, MAX_CWD_LEN);
		strlcat(path, "/", MAX_CWD_LEN);
		strlcat(path, pathname, MAX_CWD_LEN);

		resolved_path = getDirs(path, resolved_path);

	}

	return resolved_path;

}

char * getDirs(const char * pathname, char * resolved_path){

	const char * tmp = pathname;
	const char * start;

	char dirs[MAX_DIRECTORY_COUNT][MAX_NAME_LEN];

	char buffer[MAX_CWD_LEN] = "/";

	char linkBuffer[MAX_CWD_LEN];
	char parentBuffer[MAX_CWD_LEN];

	size_t len;
	int index = 0;

	int flag = 0;
	int symFlag = 0;

	while(*tmp){

		while(*tmp == '/') tmp++;

		if(!*tmp) break;

		start = tmp;

		while(*tmp && *tmp != '/') tmp++;

		len = tmp - start;

		if(len > MAX_NAME_LEN) len = MAX_NAME_LEN - 1;

		if(index < MAX_DIRECTORY_COUNT){

			memcpy(dirs[index], start, len);

			dirs[index][len] = '\0';

			index++;

		}

	}

	len = 1;

	for(int i=0;i<index;i++){

		char * dir = dirs[i];

		if(strcmp(dir, ".") == 0) continue;

		if(strcmp(dir, "..") == 0){

			if(symFlag){

				printf("Cannot have '..' after symlink.\n");

				return NULL;

			}

			if(len > 1 && buffer[len - 1] == '/') len--;

			while(len > 0 && buffer[len - 1] != '/') len--;

			buffer[len] = '\0';

			continue;

		}

		if(len > 1 && buffer[len - 1] != '/') buffer[len++] = '/';

		memcpy(buffer+len, dir, strlen(dir));

		len += strlen(dir);

		buffer[len] = '\0';

		ssize_t n = readlink(buffer, linkBuffer, sizeof(linkBuffer));

		if(n == -1){

			if(errno == ENOENT){

				if(flag){

					perror("readlink()");

					return NULL;

				}

				else flag++;

			}

		}

		else{

			if(linkBuffer[0] == '/'){

				memset(buffer, 0, sizeof(buffer));

				strcpy(buffer, linkBuffer);

			}

			else{

				if(len > 1 && buffer[len - 1] == '/') len--;

				while(len > 0 && buffer[len - 1] != '/') len--;

				buffer[len] = '\0';

				if(len > 1 && buffer[len - 1] != '/') buffer[len++] = '/';

				memcpy(buffer+len, linkBuffer, strlen(linkBuffer));

				len += strlen(linkBuffer);

				buffer[len] = '\0';

				symFlag++;

			}

		}

	}

	strcpy(resolved_path, buffer);

	return resolved_path;

}

int main(int argc, char *argv[]){

	if(argc != 2 || strcmp(argv[1], "--help") == 0){

		printf("Usage: ./18-3 path.");

		return -1;

	}

	char returnString[MAX_CWD_LEN];

	if(new_realpath(argv[1], returnString) == NULL){

		printf("Unable to find real path.\n");

		return -1;

	}

	printf("%s\n", returnString);

	return 0;

}
