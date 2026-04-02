#include "31-1.h"

/*

	My attempt at pthread_once(). It seems to work, with test() only being called once when N threads are made.
	Not sure if this is what book meant, but it seems close.

*/

void test(){

	write(1, "TEST.\n", 7);

}

int one_time_init(struct pthread_control *control, typeof(void (void)) *init){

	pthread_mutex_lock(&(control->mutex));

	if(!(control->hasInitBeenCalled)){

		init();

		(control->hasInitBeenCalled) = true;

	}

	pthread_mutex_unlock(&(control->mutex));

	return 0;

}

void* thread_function(void* arg){

	one_time_init(&control, &test);

}

int main(int argc, char* argv[]){

	int threadNum = 5;

	pthread_t * threads = malloc(threadNum * sizeof(pthread_t));

	control.hasInitBeenCalled = false;

	if(pthread_mutex_init(&(control.mutex), NULL)){

		printf("Error initializing control mutex.\n");

		return -1;

	}

	for(int i=0;i<threadNum;i++){

		printf("creating thread.\n");

		pthread_create(&threads[i], NULL, thread_function, NULL);

	}

	for(int i=0;i<threadNum;i++){

		printf("joining thread.\n");

		pthread_join(threads[i], NULL);

	}

	pthread_mutex_destroy(&(control.mutex));

	free(threads);

	return 0;

}
