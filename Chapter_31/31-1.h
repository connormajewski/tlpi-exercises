#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

struct pthread_control{

	bool hasInitBeenCalled;

	pthread_mutex_t mutex;

};

// Static control struct to track if init has been called.

struct pthread_control control;

// Simple function to verify that init in one_time_init is called only once.

void test();

// My implementation of pthread_once().

int one_time_init(struct pthread_control *control, typeof(void (void)) *init);

// Wrapper function to called one_time_init when pthread_create() is called.

void* thread_function(void* arg);


