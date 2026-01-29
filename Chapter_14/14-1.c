#include <fcntl.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>

#define FILENAME_LIMIT 1000000

int filenameNumericSort(const struct dirent ** filename1, const struct dirent ** filename2);
int fileCheck(const struct dirent *file);

int filenameNumericSort(const struct dirent ** filename1, const struct dirent ** filename2){

	long a = strtol(((*filename1)->d_name) + 1, NULL, 10);
	long b = strtol(((*filename2)->d_name) + 1, NULL, 10);

	if(a < b) return -1;
	if(a > b) return 1;

	return 0;

}

int fileCheck(const struct dirent * file){

	if(file->d_name[0] != 'x') return 0;

	return 1;

}

int main(int argc, char * argv[]){

	// Make sure process has superuser privileges for mount.

	uid_t processId = geteuid();

	if(processId != 0){

		printf("Process must have superuser privileges to run.\n");

		return -1;

	}

	// Make sure arguments fit.

	if(argc != 3 || (strcmp(argv[1], "--help") == 0)){

		printf("Usage: ./14-1 numfiles (0 - 999999) dirpath.\n");

		return -1;

	}

	long numFiles = strtol(argv[1], NULL, 10);

	if(numFiles == LONG_MIN || numFiles == LONG_MAX || numFiles > FILENAME_LIMIT){

		if(errno == ERANGE){

			printf("strtol() error.\n");

		}

		else{

			printf("numFiles must be between 0-999999.\n");

		}

		return -1;

	}

	// Attempt to make directory for mount.

	int dirCheck = mkdir(argv[2], S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);

	if(dirCheck == -1){

		printf("ERROR: ");

		switch(errno){

			case EACCES:
				printf("Write access not allowed in directory.\n");
				break;
			case EEXIST:
				printf("%s already exists.\n", argv[2]);
				break;
			case EDQUOT:
				printf("Disk block quota exhausted.\n");
				break;

		}

		return -1;

	}

	// If directory is made, attempt to mount to it.

	int mountCheck = mount("tmpfs", argv[2], "tmpfs", 0, "size=10M");

	if(mountCheck == -1){

		printf("ERROR.\n");

		perror("Mount: ");

		return -1;

	}

	struct timeval tv;

	int t = gettimeofday(&tv, NULL);

	if(t){

		printf("gettimeofday() error.\n");

		return -1;

	}

	srandom(tv.tv_sec ^ tv.tv_usec);

	long a;
	char filePath[PATH_MAX];
	int fd;

	for(int i=0;i<numFiles;i++){

		a = random() % FILENAME_LIMIT;

		snprintf(filePath, sizeof(filePath), "%s/x%lu", argv[2], a);

		fd = open(filePath, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP);

		if(fd == -1){

			printf("Error creating file %s.\n", filePath);

			perror("");

			return -1;

		}

		close(fd);

	}

	printf("%ld Files created.\n", numFiles);

	// At this point we need to open directory, scan it, sort files, and delete them.

	struct dirent ** direntArray;

	int dirScan = scandir(argv[2], &direntArray, fileCheck, filenameNumericSort);

	if(dirScan == -1){

		printf("dirscan() error.\n");

		return -1;

	}

	// Now have sorted array of files. Now need to loop through and delete each one.

	for(int i=0;i<dirScan;i++){

		snprintf(filePath, sizeof(filePath), "%s/%s", argv[2], direntArray[i]->d_name);

		remove(filePath);
		free(direntArray[i]);

	}

	free(direntArray);

	printf("Attempting umount.\n");

	mountCheck = umount(argv[2]);

	if(mountCheck == -1){

		printf("Error unmounting.\n");

		perror("ERROR: ");

		return -1;

	}

	printf("Umount successful. Removing Directory %s.\n", argv[2]);

	dirCheck  = rmdir(argv[2]);

	if(dirCheck == -1){

		printf("Error removing directory.\n");

	}

	printf("%s removed successfully.\n", argv[2]);

	printf("Sucess.\n");

	return 0;

}

