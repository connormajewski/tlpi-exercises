#define _DEFAULT_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <shadow.h>
#include <errno.h>
#include <crypt.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/wait.h>

/*

	My attempt at a douser like program. At its core im just forking+execing in child, with the added password checks, group and user id swaps, and privleges.

	For this program to work, I do the following after compiling.

	sudo chmod u+s 38-2
	sudo chown root:root 38-2

	Need these so that privileged functions can work.

*/

int main(int argc, char *argv[]){

	int maxAttempts = 2;

	int passwordAttempts = 0;
	int authOk = 0;

	struct passwd *passwd;
	struct spwd *spasswd;

	long user_len_max = sysconf(_SC_LOGIN_NAME_MAX);

	if(user_len_max == -1) user_len_max = 256;

	char * username = malloc(user_len_max);

	if(username == NULL){

		perror("malloc() ");

		return -1;

	}

	int opt;

	strcpy(username, "root");

	if(argc < 2){

		printf("Usage: %s [-u user] program-file arg1 arg2 ...\n", argv[0]);

		return -1;

	}

	while((opt = getopt(argc, argv, ":+u:")) != -1){

		if(opt == '?'){

			printf("Unrecognized option \n");

			return -1;

		}

		if(opt == 'u'){

			strcpy(username, optarg);

			if(username == NULL){

				perror("strcpy(): ");

				return -1;

			}

		}

	}

	passwd = getpwnam(username);

	if(passwd == NULL){

		perror("getpwnam() ");

		return -1;

	}

	// At this point in the program, Should be set-user-id-root, need perm to read shadow file.

	uid_t ruid = getuid();
	uid_t euid = geteuid();

	if(seteuid(0) != 0){

		perror("seteuid() ");

		return -1;

	}

	spasswd = getspnam(username);

	if(spasswd == NULL && errno == EACCES){

		perror("getspnam() ");

		return -1;

	}

	if(spasswd != NULL) passwd->pw_passwd = spasswd->sp_pwdp;

	while(!authOk){

		printf("[%s] password for %s: ", argv[0], username);

		char * password = getpass("");

		char * encrypted = crypt(password, passwd->pw_passwd);

		if(encrypted == NULL){

			perror("crypt(): ");

			return -1;

		}

		for(char *p = password; *p != '\0';) *p++ = '\0';

		if(strcmp(encrypted, passwd->pw_passwd)){

			printf("Sorry, try again.\n");

			if(passwordAttempts++ >= maxAttempts){

				printf("%s: 3 incorrect password attempts.\n", argv[0]);

				return -1;

			}

		}

		else{

			authOk++;

		}

	}

	pid_t pid = fork();

	if(pid == -1){

		perror("fork() ");

		return -1;

	}

	if(pid == 0){

		if(initgroups(username, passwd->pw_gid)){

			perror("initgroups() ");

			return -1;

		}

		if(setresgid(passwd->pw_gid, passwd->pw_gid, passwd->pw_gid)){

			perror("setresgid() ");

			return -1;

		}


		if(setresuid(ruid, ruid, ruid) != 0){

			perror("seteuid() ");

			return -1;

		}

		execvp(argv[optind], &argv[optind]);

		perror("execvp() ");

		return -1;

	}

	else{

		wait(NULL);

	}

	return 0;

}
