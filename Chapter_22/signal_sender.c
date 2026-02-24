#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/*

	All I doing here is sending a signal and realtime signal in a loop.
	Not clean at all, I used atoi() here when I shouldn't.

*/

int main(int argc, char *argv[]){

	int signal;
	int realtimeSignal;
	int signalCount;

	pid_t pid;

	union sigval sv;

	if(argc != 5){

		printf("Usage: ./signal_sender PID SIG RTSIG num.\n");

		return -1;

	}

	signal = atoi(argv[2]);
	realtimeSignal = atoi(argv[3]);
	signalCount = atoi(argv[4]);
	pid = atoi(argv[1]);

	printf("Sending signal %d %d times.\n", signal, signalCount);
	printf("Sending realtime signal %d %d times.\n", realtimeSignal, signalCount);

	for(int i=0;i<signalCount;i++){

		kill(pid, signal);
		sigqueue(pid, realtimeSignal, sv);

	}

	return 0;

}
