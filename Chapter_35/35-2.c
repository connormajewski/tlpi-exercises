#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/capability.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

	int policy;
	int priority;

	if(argc < 4){

		printf("Usage: %s policy priority command arg...\n", argv[0]);

		return -1;

	}

	// Check for 'r' or 'f' in policy argument.

	if(strlen(argv[1]) != 1){

		printf("Invalid polcy type %s.\n", argv[1]);

		return -1;

	}

	if(argv[1][0] == 'r') policy = SCHED_RR;

	else if(argv[1][0] == 'f') policy = SCHED_FIFO;

	else{

		printf("Unknown policy flag %s\n", argv[1]);

		return -1;

	}

	// Check if given priority is valid.

	priority = atoi(argv[2]);

	if(priority < sched_get_priority_min(policy) || priority > sched_get_priority_max(policy)){

		printf("Invalid priority value for range.\n");

		return -1;

	}

	// Set scheduling.

	struct sched_param scheduler;

	scheduler.sched_priority = priority;

	if(sched_setscheduler(0, policy, &scheduler) == -1){

		perror("setscheduler() error.\n");

		return -1;

	}

	// Drop privileges.

	seteuid(getuid());

	// Exec with command and arguments.

	execvp(argv[3], &argv[3]);

}
