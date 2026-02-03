#include <sys/acl.h>
#include <acl/libacl.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include "ugid_functions.h"

/*
	This is a really crappy implementation, but it works. There is 100% a better way to do this.
 	Main issues were fitting user and group into single function.
	Sloppy workaround of only going through ACL once for both mask and user/group.
	Basically storing mask permissions and user permissions, then printing after ACL has
	been moved through.
*/

/*

	getEntryPermissions() - Retrieves ACL permissions for the specified user or group of the given file.

	ARGS:
		const char * pathname - Filepath for ACL.
		const char * name - Name for permissions. Can be text or a numeric UID/GID.
		const char opt - Flag determining if name is name of a user or group.

	RETURN:
		0 - SUCCESS.
	       -1 - FAILURE.

*/

int getEntryPermissions(const char * pathname, const char * name, const char opt);

int getEntryPermissions(const char * pathname, const char * name, const char opt){

	// ACL variables, these store ACL and related values.

	acl_t acl;
	acl_entry_t entry;
	acl_tag_t tag;
	acl_type_t type;
	acl_permset_t permissions;
	acl_permset_t maskPermissions;

	int permissionValue;
	int entryId;

	int mask = 0;

	// These are used inside ACL_USER check, during looping of permission bits.

	int permission[3] = {ACL_READ, ACL_WRITE, ACL_EXECUTE};
	int check[3] = {0,0,0};
	int maskCheck[3] = {0,0,0};
	int uidFound = 0;

	// Store uid of name provided in command line, and uid of entry with matching name in ACL.

	uint32_t uid;
	uint32_t *aclUid;

	long tmp = strtol(name, NULL, 10);

	// Numeric input outside uid/gid limits.

	if(tmp > INT_MAX){

		printf("UID must fit in unsigned integer range.\n");

		return -1;

	}

	// Text input.

	else if(tmp == 0){

		uid = (opt == 'u') ? (uint32_t) userIdFromName(name) : (uint32_t) groupIdFromName(name);

		if(uid == -1){

			printf("Invalid name.\n");

			return -1;

		}

	}

	// Valid numeric input.

	else{

		uid = tmp;

		printf("OPT: %c\n", opt);

		name = (opt == 'u') ? userNameFromId(uid) : groupNameFromId(uid);

		if(name == NULL){

			printf("Invalid ID.\n");

			return -1;

		}

	}

	// At this point we now have valid UID/name pair, can get ACL and start doind stuff.

	acl = acl_get_file(pathname, ACL_TYPE_ACCESS);

	if(acl == (acl_t)NULL){

		perror("acl_get_file(): ");

		return -1;

	}

	// Once ACL found, move through until we find user tag.

	for(entryId = ACL_FIRST_ENTRY; ; entryId = ACL_NEXT_ENTRY){

		if(acl_get_entry(acl, entryId, &entry) == 0) break;

		if(acl_get_tag_type(entry, &tag) == -1){

			perror("acl_get_tag_type(): ");

			return -1;

		}

		// Store ACL_MASK permissions when found to be used outside of loop.

		if(tag == ACL_MASK){

			if(acl_get_permset(entry, &maskPermissions) == -1){

				perror("acl_get_permset(): ");

				return -1;

			}

			for(int i=0;i<3;i++){

				permissionValue = acl_get_perm(maskPermissions, permission[i]);

				if(permissionValue == -1){

					perror("acl_get_perm(): ");

					return -1;

				}

				if(permissionValue) maskCheck[i]++;

			}

			mask++;

		}

		// When a ACL_USER entry is found, grab its name and uid. If these match we move through permissions and store bits for printing.

		if((opt == 'u' && (tag == ACL_USER)) || ((opt == 'g') && (tag == ACL_GROUP))){

			aclUid = acl_get_qualifier(entry);

			if(aclUid == NULL){

				perror("acl_get_qualifier(): ");

				return -1;

			}

			char * n = (opt == 'u') ? userNameFromId(*aclUid) : groupNameFromId(*aclUid);

			if(n == NULL){

				perror("userNameFromId(): ");

				return -1;

			}

			if(uid == *aclUid){

				uidFound++;

				if(acl_get_permset(entry, &permissions) == -1){

					perror("acl_get_permset(): ");

					return -1;

				}

				for(int i=0;i<3;i++){

					permissionValue = acl_get_perm(permissions, permission[i]);

					if(permissionValue == -1){

						perror("acl_get_perm(): ");

						return -1;

					}

					if(permissionValue) check[i]++;

				}

				if(acl_free(aclUid) == -1) perror("acl_free(): ");

			}

		}

	}

	// Printing after ACL has been checked, since ACL_MASK comes after ACL_USERs, meaning ANDing permissions bits for printing won't work,

	if(uidFound){

		printf("%s %-12s ", opt == 'u' ? "user" : "group", name);

		// ACL_USER and ACL_GROUP permissions always get ANDed woth ACL_MASK if it exists.

		if(mask){

			for(int i=0;i<3;i++) check[i] = (check[i] && maskCheck[i]);

		}

		for(int i=0;i<3;i++){

			printf("%c", (check[i]) ? i == 0 ? 'r' : i == 1 ? 'w' : i == 2 ? 'x' : '\0' : '-');

		}

		printf("\n");

	}

	if(acl_free(acl) == -1) perror("acl_free(): ");

	return 0;

}

int main(int argc, char * argv[]){

	int check;
	char type;

	// Make sure arguments are valid.

	if(argc != 4 || (strcmp(argv[1], "--help") == 0)){

		printf("Usage: ./17-1 [ug] (user or group - can be numeric or string).\n");

		return -1;

	}

	type = argv[1][0];

	if(type != 'u' && type != 'g' || strlen(argv[1]) != 1){

		printf("Invalid option '%s'. Must be single characters u or g.\n", argv[1]);

		return -1;

	}

	check = getEntryPermissions(argv[3], argv[2], argv[1][0]);

	if(check == -1){

		printf("Failure finding permissions for %s in %s.\n", argv[2], argv[3]);

		return -1;

	}

	return 0;

}
