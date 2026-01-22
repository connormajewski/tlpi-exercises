#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "tlpi_hdr.h"

#define BUFFER_SIZE 1024

int listProcessWithFilename(const char * filepath);

int listProcessWithFilename(const char * filepath){

	ssize_t readlinkCheck;

	struct dirent *dir;
	struct dirent *innerDir;

	char buffer[BUFFER_SIZE];
	char linkPath[BUFFER_SIZE * 2];
	char path[BUFFER_SIZE];

	char * parentDir = "/proc";

	int snprintfCheck;
	int closeDirCheck;

	DIR *ds = NULL;
	DIR *innerDs = NULL;

	ds = opendir(parentDir);

	if(ds == NULL){

		printf("Error opendir().\n");

		return -1;

	}

	errno = 0;

	// Converting filepath to number. Any file not consisting of only numbers fails.

	long filepathConversion;

	while((dir = readdir(ds)) != NULL){

		filepathConversion = strtol(dir->d_name, NULL, 10);

		if(filepathConversion > 0){

			snprintfCheck = snprintf(path, sizeof(path), "%s/%s/fd", parentDir, dir->d_name);

			if(snprintfCheck < 0 || snprintfCheck >= BUFFER_SIZE){

				printf("snprintf() error.\n");

				return -1;

			}

			innerDs = opendir(path);

			if(innerDs == NULL){

				printf("opendir() inner error. %s\n", path);

				return -1;

			}

			while((innerDir = readdir(innerDs)) != NULL){

				// We need to skip "." and ".." in directory before calling readlink().

				if(innerDir->d_name[0] != '.'){

					snprintf(linkPath, sizeof(linkPath), "%s/%s", path, innerDir->d_name);

					if(snprintfCheck < 0 || snprintfCheck >= BUFFER_SIZE){

						printf("snprintf() error.\n");

						return -1;

					}

					readlinkCheck = readlink(linkPath, buffer, BUFFER_SIZE);

					if(readlinkCheck < 0){

						printf("readlink() error. %s\n", linkPath);

						return -1;

					}

					buffer[readlinkCheck] = '\0';

					if(strcmp(buffer, filepath) == 0){

						printf("%lu: %s\n", filepathConversion, buffer);

					}

				}


			}

			closeDirCheck = closedir(innerDs);

			if(closeDirCheck == -1){

				printf("closedir() error.\n");

				return -1;

			}

		}

	}

	closeDirCheck = closedir(ds);

	if(closeDirCheck == -1){

		printf("closeDirCheck() error.\n");

		return -1;

	}

	return 0;

}

int main(int argc, char * argv[]){

	if(argc != 2){

		printf("Usage: ./12-3 filepath\n");

		return -1;

	}

	listProcessWithFilename(argv[1]);

	return 0;

}
