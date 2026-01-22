#include <dirent.h>
#include <pwd.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

uid_t userIdFromName(const char *name);
int allProcessFromUser(const char *name);

uid_t userIdFromName(const char *name){

	struct passwd *pwd;
	uid_t u;
	char * endptr;

	if(name == NULL || *name == '\0'){

		return -1;

	}

	u = strtol(name, &endptr, 10);

	if(*endptr == '\0'){

		return u;

	}

	pwd = getpwnam(name);

	if(pwd == NULL){

		return -1;

	}

	return pwd->pw_uid;

}

int allProcessFromUser(const char *name){

	uid_t userId;

	struct dirent *dir;

	char path[1024];

	char * parentDir = "/proc";

	userId = userIdFromName(name);

	if(userId == -1){

		printf("Error userIdFromName().\n");

		return -1;

	}

	DIR *ds = NULL;

	ds = opendir(parentDir);

	if(ds == NULL){

		printf("Error opendir().\n");

		return -1;

	}

	errno = 0;

	while((dir = readdir(ds)) != NULL){

		long conversion = strtol(dir->d_name, NULL, 10);

		if(conversion > 0){

			snprintf(path, sizeof(path), "%s/%s/status", parentDir, dir->d_name);

			int fd = open(path, O_RDONLY);

			if(fd == -1){

				if(errno = ENOENT){

					continue;

				}

				else{

					printf("Error open().\n");

					return -1;

				}

			}

			char line[256];

			FILE *fp = fdopen(fd, "r");

			int isNameFound = 0;

			char statusName[256];

			while(fgets(line, sizeof(line), fp)){

				char * colon = strchr(line, ':');
				if(!colon) continue;

				*colon = '\0';

				char * key = line;
				char * value = colon + 1;

				if(strcmp(key, "Name") == 0){

					while(*value == ' ' || *value == '\t') {

						value++;

					}

					strcpy(statusName, value);
				}


				if(strcmp(key, "Uid") == 0){

					int a, b, c, d;

					sscanf(value, " %d %d %d %d ", &a, &b, &c, &d);

					if(b == userId){

						printf("%s: %s", dir->d_name, statusName);

					}

					break;

				}


			}

			fclose(fp);

		}


	}

	if(errno != 0){

		printf("readdir() error.\n");

		return -1;

	}

	int t = closedir(ds);

	if(t == -1){

		printf("Error closedir().\n");

		return -1;

	}

	return 0;

}

int main(int argc, char * argv[]){

	if(argc != 2){

		printf("Usage: ./12-2 name\n");

		return -1;

	}

	int a = allProcessFromUser(argv[1]);

	return 0;

}

