#include <pwd.h>
#include "tlpi_hdr.h"

struct passwd * new_getpwnam(const char *name);

struct passwd * new_getpwnam(const char *name){

	struct passwd * tmp;

	setpwent();

	while((tmp = getpwent()) != NULL){

		if(strcmp(tmp->pw_name, name) == 0){

			endpwent();

			return tmp;

		}

	}

	endpwent();

	return NULL;

}

int main(int argc, char * argv[]){

	const char * name = "root";

	errno = 0;

	struct passwd * tmp = new_getpwnam(name);

	if(tmp == NULL){

		printf("%d: No record found in /etc/passwd for %s\n", errno, name);

		return -1;

	}

	else{

		printf("%s\n%s\n%d\n%d\n%s\n%s\n%s\n\n", tmp->pw_name, tmp->pw_passwd, tmp->pw_uid, tmp->pw_gid, tmp->pw_gecos, tmp->pw_dir, tmp->pw_shell);

	}

	return 0;

}
