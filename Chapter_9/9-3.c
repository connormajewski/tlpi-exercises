#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include "tlpi_hdr.h"

#ifndef NGROUPS_MAX
#define NGROUPS_MAX (2 << 16)
#endif

int new_initgroups(const char *user, gid_t group);
int printgroups(size_t groupSize, gid_t * group);

int new_initgroups(const char *user, gid_t group){

	// Before anything, check that calling process has elevated privileges.

	uid_t uid = geteuid();

	if(uid){

		errno = EPERM;

		return -1;

	}

	gid_t groups[NGROUPS_MAX];
	size_t groupsCount = 0;

	groups[groupsCount++] = group;

	struct group * tmp;

	setgrent();

	while((tmp = getgrent()) != NULL){

		for(char ** gr = tmp->gr_mem; gr != NULL && *gr != NULL; gr++){

			if(strcmp(*gr, user) == 0){

				groups[groupsCount] = tmp->gr_gid;

				groupsCount++;

				if(groupsCount >= NGROUPS_MAX){

					errno = ENOMEM;

					return -1;

				}

			}


		}

	}

	endgrent();

	int check = setgroups(groupsCount, groups);

	if(check == -1){

		return -1;

	}

	return 0;

}

int printgroups(size_t groupSize, gid_t * group){

	size_t check = getgroups(groupSize, group);

	if(check == -1){

		return -1;

	}

	for(size_t i=0;i<check;i++){

		printf("%u ", group[i]);

	}

	printf("\n");

	return 0;

}

int main(int argc, char * argv[]){

	gid_t groups[NGROUPS_MAX];

	int check = new_initgroups("cmajews2", 0);

	if(check == -1){

		printf("Error new_initgroups().\n");

		return -1;

	}

	printgroups(NGROUPS_MAX, groups);

	check = initgroups("cmajews2", 0);

	if(check == -1){

		printf("Error new_initgroups().\n");

		return -1;

	}

	printgroups(NGROUPS_MAX, groups);

	exit(EXIT_SUCCESS);

}
