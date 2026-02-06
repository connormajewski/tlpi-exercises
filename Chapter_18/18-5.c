#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX_CWD_LEN 4096

/*

	Not sure if this is how book wanted it.

	Basically, I stat current directory and grab its i-node. I then loop through parent directory,
	until I find matching i-node. When found, I add directory name to array, and chdir into parent.
	I repeat this process until / is hit. I then move backwards through array, appending each subdirectory to char buf[],
	and return.

*/

char * new_getcwd(char buf[], size_t size);

char * new_getcwd(char buf[], size_t size){

	int flag = 1;

	int fd;

	DIR * directory;

	struct dirent * dir;

	struct stat st;

	unsigned long cwd_ino;
	unsigned long pwd_ino;

	char * dirNames[256];

	int depth = 0;

	// Store current directory to move back to later.

	if(fd = open(".", O_RDONLY) < 0){

		perror("open()");

		return NULL;

	}

	while(flag){

		if(stat(".", &st) == -1){

			perror("stat()");

			fchdir(fd);
			close(fd);

			return NULL;

		}

		cwd_ino = st.st_ino;

		directory = opendir("..");

		if(directory == NULL){

			printf("opendir() error.\n");

			fchdir(fd);
			close(fd);

			return NULL;

		}

		while(((dir = readdir(directory)) != NULL) && !errno){

			pwd_ino = dir->d_ino;

			if(pwd_ino == cwd_ino){

				if(strcmp(dir->d_name, "..") == 0){

					flag--;

				}

				else{

					dirNames[depth++] = strdup(dir->d_name);

					chdir("..");

				}

				break;

			}

		}

		closedir(directory);

	}

	int j = 0;

	buf[j++] = '/';

	for(int i=depth-1;i>=0;i--){

		if(j > 1 && buf[j - 1] != '/') buf[j++] = '/';

		memcpy(buf+j, dirNames[i], strlen(dirNames[i]));

		j += strlen(dirNames[i]);

		// strdup() allocates memory, so we have to make sure to free to prevent small memory leaks.

		free(dirNames[i]);

	}

	buf[j] = '\0';

	// We do this regardless of whether function succeeds or not, to make sure we always end up where we started.

	fchdir(fd);
	close(fd);

	return buf;

}

int main(int argc, char *argv[]){

	char buffer[MAX_CWD_LEN];

	if(new_getcwd(buffer, MAX_CWD_LEN) == NULL){

		perror("new_getcwd()");

		return -1;

	}

	printf("%s\n", buffer);

	return 0;

}
