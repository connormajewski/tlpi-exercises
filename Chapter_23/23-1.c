#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

static volatile sig_atomic_t alarmFlag = 0;

void handler(int sig);

unsigned int new_alarm(unsigned int seconds);


void handler(int sig){

	alarmFlag = 1;

}

unsigned int new_alarm(unsigned int seconds){

	struct itimerval timerval = {0};

	struct itimerval oldTimer;

	if(seconds > 0){

		struct timeval value;

		value.tv_sec = seconds;
		value.tv_usec = 0;

		timerval.it_value = value;

	}

	if(setitimer(ITIMER_REAL, &timerval, &oldTimer) == -1){

		perror("setitimer()");

		return 0;

	}

	return oldTimer.it_value.tv_sec;

}

int main(int argc, char *argv[]){

	char *ptr;

	int time;

	if(argc != 2){

		printf("Usage: ./23-1 [timer interval - sec]\n");

		return -1;

	}

	signal(SIGALRM, handler);

	time = strtol(argv[1], &ptr, 10);

	printf("Setting %d second alarm.\n", time);

	new_alarm(time);

	while(1) {

		if(alarmFlag) {

			printf("ALARM.\n");

		}

	}

	return 0;

}
