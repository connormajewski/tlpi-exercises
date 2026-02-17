#define _XOPEN_SOURCE 500
#include <sys/inotify.h>
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <ftw.h>
#include <string.h>
#include <errno.h>

/*
	Memory limits on inotify mechanism.

	max_queued_events  - 16,384 - Number of events that can be queued on inotify instance.
	max_user_instances - 128    - Number of inotify instances created per real user id.
	max_user_watches   - 8192   - Number of watch items created per real user id.
*/

#define MAX_BUFFER_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))
#define INOTIFY_MASK (IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF | IN_MOVED_FROM | IN_MOVED_TO)
#define MAX_WATCHES 8192

// Need this to keep track of inotify paths for printing.

struct watch_descriptor{

	int wd;
	char path[PATH_MAX];

};

// Needs to be global since it cannot be passed to fixed nftw helper function.

int inotify_fd;
int inotify_wd;

// Stores watch descriptor info for printing.

struct watch_descriptor watch_descriptors[MAX_WATCHES];
int watch_descriptor_index = 0;


int delete_watch_descriptor(char * watch_path);

void print_watch_descriptor();

char *path_from_wd(int wd);

void log_inotify_event(struct inotify_event *event);

int nftw_helper(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf);

int create_directory_list(const char * directory_path, char * directory_list[]);



int delete_watch_descriptor(char * watch_path){

	char path[PATH_MAX] = "";
	int wd = -1;

	for(int i=0;i<watch_descriptor_index;i++){

		if(strcmp(watch_descriptors[i].path, watch_path) == 0){

			strncpy(path, watch_descriptors[i].path, PATH_MAX - 1);

			path[PATH_MAX - 1] = '\0';

			wd = watch_descriptors[i].wd;

		}

	}

	if((path[0] == '\0') || (wd < 0)){

		printf("WD could not be found for %s\n", watch_path);

		return -1;

	}


	for(int i=0;i<watch_descriptor_index;){

		if(strncmp(watch_descriptors[i].path, path, PATH_MAX) == 0){

			if((watch_descriptors[i].path[strlen(path)] == '/' || watch_descriptors[i].path[strlen(path)] == '\0')){

				if((inotify_rm_watch(inotify_fd, watch_descriptors[i].wd) == -1) && errno != EINVAL) {

					perror("inotify_rm_watch");

					return -1;

				}

				else {

					for(int j=i;j<watch_descriptor_index - 1;j++){

						watch_descriptors[j] = watch_descriptors[j+1];

					}

					watch_descriptor_index--;

				}

			}

		}

		else i++;

	}

	return 0;

}

void print_watch_descriptors(){

	for(int i=0;i<watch_descriptor_index;i++){

		printf("%d %s\n", watch_descriptors[i].wd, watch_descriptors[i].path);

	}

}

char *path_from_wd(int wd){

	for(int i=0;i<watch_descriptor_index;i++){

		if(watch_descriptors[i].wd == wd) return watch_descriptors[i].path;

	}

	return NULL;

}

void log_inotify_event(struct inotify_event *event){

	char full_path[PATH_MAX];
	char *wd_path;

	int mask = event->mask;
	int wd;

	wd_path = path_from_wd(event->wd);

	if(wd_path == NULL){

		printf("WD NULL.\n");

		return;

	}

	if(event->len > 0) snprintf(full_path, PATH_MAX, "%s/%s", path_from_wd(event->wd), event->name);
	else snprintf(full_path, PATH_MAX, "%s", path_from_wd(event->wd));

	if(mask & IN_ISDIR){

		if(mask & (IN_CREATE | IN_MOVED_TO)){

			if((wd = inotify_add_watch(inotify_fd, full_path, INOTIFY_MASK)) == -1){

				perror("inotify_add_watch()");

			}

			watch_descriptors[watch_descriptor_index].wd = wd;
			strncpy(watch_descriptors[watch_descriptor_index].path, full_path, PATH_MAX - 1);
			watch_descriptors[watch_descriptor_index].path[PATH_MAX - 1] = '\0';
			watch_descriptor_index++;

			printf("Created directory at %s\n", full_path);

		}

		else if(mask & (IN_DELETE_SELF | IN_MOVE_SELF | IN_DELETE)){

			if(delete_watch_descriptor(full_path) == -1){

				printf("Error deleting inotify watch descriptor.\n");

			}

			printf("Deleted directory %s.\n", full_path);

		}

	}

	else{

		if(mask & (IN_CREATE)){

			printf("Created file at %s\n", full_path);

		}

		if(mask & (IN_DELETE)){

			printf("Deleted file at %s\n", full_path);

		}

		// These two should appear as duo when using mv, which is why I wrote it like this.

		if(mask & (IN_MOVED_FROM)){

			printf("Renaming file %s to ", full_path);

		}

		if(mask & (IN_MOVED_TO)){

			printf("%s\n", full_path);

		}

	}

}

int nftw_helper(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){

	if(typeflag == FTW_D){

		inotify_wd = inotify_add_watch(inotify_fd, fpath, INOTIFY_MASK);

		if(inotify_wd == -1){

			perror("inotify_add_watch()");

			return -1;

		}

		watch_descriptors[watch_descriptor_index].wd = inotify_wd;
		strncpy(watch_descriptors[watch_descriptor_index].path, fpath, PATH_MAX - 1);
		watch_descriptors[watch_descriptor_index].path[PATH_MAX - 1] = '\0';
		watch_descriptor_index++;

	}

	return 0;

}

int create_directory_list(const char * directory_path, char * directory_list[]){

	const struct stat *sb;
	struct FTW *ftwbuf;
	int typeflag;

	nftw(directory_path, nftw_helper, 20, FTW_PHYS);

	return 0;

}

int main(int argc, char *argv[]){

	ssize_t bytesRead;

	char eventBuffer[MAX_BUFFER_LEN];

	char *p;

	struct inotify_event * event;

	if(argc != 2){

		printf("Usage: ./19-1 directory.\n");

		return -1;

	}

	inotify_fd = inotify_init();

	if(inotify_fd == -1){

		perror("inotify_init()");

		return -1;

	}

	// Need to add all currently existing dir/subdirs to watch_descriptor array and inotify instance.

	if(nftw(argv[1], nftw_helper, 20, FTW_PHYS) == -1){

		perror("nftw()");

		return -1;

	}

	for(;;){

		bytesRead = read(inotify_fd, eventBuffer, MAX_BUFFER_LEN);

		if(!bytesRead){

			perror("inotify read() read 0 bytes");

			return -1;

		}

		if(bytesRead == -1){

			perror("inotify read() failed");

			return -1;

		}

		for(p = eventBuffer; p < eventBuffer + bytesRead;){

			event = (struct inotify_event *) p;

			log_inotify_event(event);

			p += sizeof(struct inotify_event) + event->len;

		}

	}

	return 0;

}
