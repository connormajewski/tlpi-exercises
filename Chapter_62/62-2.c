#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#define DEV_PATH "/dev"
#define DEV_PTS_PATH "/dev/pts"
#define MAX_DIR_LEN 256

/*

	Here is my first attempt at ttyname(). It works, but I can definitely improve it.
	I implemented it based on what I read in the book.

*/

char *ttyname_clone(int fd);

char *tty_helper(int fd, char *path);

char *ttyname_clone(int fd){

	static char buf[MAX_DIR_LEN];

	char *ret = tty_helper(fd, DEV_PTS_PATH);

	if(ret != NULL){

		strncpy(buf, ret, sizeof(buf));

		buf[sizeof(buf) - 1] = '\0';

		free(ret);

		return buf;

	}

	ret = tty_helper(fd, DEV_PATH);

	if(ret != NULL){

		strncpy(buf, ret, sizeof(buf));

		buf[sizeof(buf) - 1] = '\0';

		free(ret);

		return buf;

	}

	return NULL;

}

char *tty_helper(int fd, char *path){

	DIR *dir;

	struct dirent *d;

	struct stat s;

	dev_t deviceId;

	if(!isatty(fd)){

		errno = ENOTTY;

		return NULL;

	}

	char fullPath[MAX_DIR_LEN];

	dir = opendir(path);

	if(dir == NULL){

		closedir(dir);

		return NULL;

	}

	while((d = readdir(dir)) != NULL){

		if(d->d_name[0] == '.') continue;

		snprintf(fullPath, (strlen(path) + strlen(d->d_name) + 2), "%s/%s", path, d->d_name);

		if(stat(fullPath, &s) == -1){

			closedir(dir);

			return NULL;

		}

		if(!S_ISCHR(s.st_mode) || S_ISDIR(s.st_mode)){

			continue;

		}

		deviceId = s.st_rdev;

		if(fstat(fd, &s) == -1){

			errno = EBADF;

			closedir(dir);

			return NULL;

		}

		if(deviceId == s.st_rdev){

			closedir(dir);

			return strdup(fullPath);

		}

	}

	closedir(dir);

	return NULL;

}

int main(int argc, char *argv[]){

	char *ret;

	for(int i=0;i<100000;i++){

		if((ret = ttyname(i)) != NULL){

			printf("%d: %s: ", i, ret);

		}

		if((ret = ttyname_clone(i)) != NULL){

			printf("%s\n", ret);

		}

	}

	return 0;

}
