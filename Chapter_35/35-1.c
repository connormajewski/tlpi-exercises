#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/capability.h>
#include <sys/resource.h>
#include <sys/wait.h>

/*

	My atttempt at making a mock nice(). Currently runs like:

	[-v] [-h] [-n N] -- [command arg ...]

	Need the "--" to prevent optionals meant for command being processed as if for program.
	Not sure how to fix, not too confident with optargs.

	Basically I get priority number, check if it is valid and if program has privileges, then call an exec().

*/

int nice_capability_check();

int nice_capability_check(){

	cap_t capabilities = cap_get_proc();

	if(capabilities == NULL) return -1;

	cap_flag_value_t value;

	if(cap_get_flag(capabilities, CAP_SYS_NICE, CAP_EFFECTIVE, &value) == -1){

		cap_free(capabilities);

		return -1;

	}

	cap_free(capabilities);

	return (value == CAP_SET);

}

int main(int argc, char *argv[]){

	int opt;

	int priority = 0;

	while((opt = getopt(argc, argv, ":+n:vh")) != -1){

		switch(opt){

			case 'v':
				printf("Version: 0.0.1\n");
				break;
			case 'h':
				printf("Usage: %s [-v] [-h] [-n N] -- [command arg ...]\n", argv[0]);
				break;
			case 'n':
				priority = atoi(optarg);
				break;
		}

	}

	int currentNiceValue = getpriority(PRIO_PROCESS, 0);

	if(priority >= 0 || (priority < 0 && nice_capability_check())){

		pid_t pid = fork();

		if(pid == -1){

			printf("fork() error.\n");

			return -1;

		}

		else if(pid == 0){

			if(setpriority(PRIO_PROCESS, 0, currentNiceValue + priority) == -1){

				printf("setpriority() error.\n");
				return -1;

			}

			execvp(argv[optind], &argv[optind]);

			printf("execvp() error.\n");

			return -1;

		}

		else{

			wait(NULL);

		}

	}

	else{

		printf("Cannot add %d to nice value.\n", priority);

		return -1;

	}


	return 0;

}
