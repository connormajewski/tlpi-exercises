#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <stdlib.h>

/*

	Not sure if this is correct, I followed what was said in book, but I am not sure if
	this is the right way to measure CPU time increments.

*/

void processFunction(pid_t pid){

	struct tms buffer;

	long ticks = sysconf(_SC_CLK_TCK);

	times(&buffer);

	long startingTime = buffer.tms_utime + buffer.tms_stime;

	long quarter = ticks / 4;
	long second = ticks;

	long duration = 3 * ticks;

	for(;;){

		times(&buffer);

		long elapsedTime = (buffer.tms_utime + buffer.tms_stime) - startingTime;

		if(elapsedTime >= duration) break;

		if(elapsedTime >= quarter){

			printf("%d: %ld\n", pid, buffer.tms_utime + buffer.tms_stime);

			quarter += ticks / 4;

		}

		if(elapsedTime >= second){

			sched_yield();

			second += ticks;

		}

	}

}

int main(int argc, char *argv[]){

	struct sched_param schedule;

	schedule.sched_priority = 99;

	if(sched_setscheduler(0, SCHED_FIFO, &schedule) == -1){

		perror("sched_setscheduler()");

		return -1;

	}

	pid_t pid = fork();

	if(pid == -1){

		perror("fork()");

		return -1;

	}

	else{

		processFunction(pid);

	}

	return 0;

}
