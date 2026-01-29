#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>

int new_access(const char *pathname, int mode);

int new_access(const char *pathname, int mode){

	struct stat s;
	int ngroups;

	uid_t effectiveUid = geteuid();

	mode_t permissions;

	int groupCheck = 0;

	if(stat(pathname, &s) == -1){

		printf("stat() error.\n");

		return -1;

	}

	if((ngroups = getgroups(0, NULL)) == -1){

		printf("getgroups() ngroups error.\n");

		return -1;

	}

	gid_t groups[ngroups];

	if((ngroups = getgroups(ngroups, groups)) == -1){

		printf("getgroups() error.\n");

		return -1;

	}

	for(int i=0;i<ngroups;i++){

		if(groups[i] == s.st_gid){

			groupCheck = 1;

			break;

		}

	}

	if(effectiveUid == 0){

		if((mode & X_OK) && !(s.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))){

			errno = EACCES;

			return -1;

		}

		return 0;

	}

	else if(effectiveUid == s.st_uid){

		permissions = s.st_mode >> 6;

	}

	else if(groupCheck){

		permissions = s.st_mode >> 3;

	}

	else{

		permissions = s.st_mode;

	}

	permissions = permissions & 7;

	if((mode & R_OK) && !(permissions & 4)){

		errno = EACCES;

		return -1;

	}

	if((mode & W_OK) && !(permissions & 2)){

		errno = EACCES;

		return -1;

	}

	if((mode & X_OK) && !(permissions & 1)){

		errno = EACCES;

		return -1;

	}

	return 0;

}

int main(int argc, char * argv[]){

	if(argc != 2){

		printf("Usage: ./15-4 filepath.\n");

		return -1;

	}

	int r = new_access(argv[1], R_OK);
    	int w = new_access(argv[1], W_OK);
    	int x = new_access(argv[1], X_OK);

    	printf("R: %s\nW: %s\nE: %s\n", r == 0 ? "YES" : "NO", w == 0 ? "YES" : "NO", x == 0 ? "YES" : "NO");

	return -1;

}
