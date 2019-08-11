// https://randu.org/tutorials/threads/
// http://www.cs.kent.edu/~ruttan/sysprog/lectures/multi-thread/multi-thread.html
// https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html

// Create few threads which operates on shared data. Synchronization is achieved with mutex.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5 

// Create data-structure to pass information to thread threadFunction()
// Only a void pointer can be passed to thread function.
typedef struct {
	int tid;
	double stuff;
} thread_data_t;

// Shared variable between threads
double shared_x;
// Mutex for shared variable
pthread_mutex_t lock_x;

// Thread/worker function.
// pthread accepts the thread function with following signature-
// void* (*threadFunc)(void*)
void * threadFunction(void *arg) {
	thread_data_t *data = (thread_data_t *)arg;

	printf("Hello from threadFunction, thread id: %d\n", data->tid);

	// Get mutex before modifying and printing shared_x
	pthread_mutex_lock(&lock_x);

	// Perform operation on shared data atomically
	shared_x += data->stuff;
	printf("x = %f\n", shared_x);

	// Release mutex before returning
	pthread_mutex_unlock(&lock_x);

	// Use pthread_exit() to exit a thread otherwise main and all threads will be terminated.
	pthread_exit(NULL);
}

int main(int argc, char **argv) {
	pthread_t thr[NUM_THREADS];
	// Create a thread_data_t argument array
	thread_data_t thr_data[NUM_THREADS];

	// Initialize shared data
	shared_x = 0;

	// Initialize pthread mutex protecting "shared_x" with pthread_mutex_init()
	// or use 'lock_x = PTHREAD_MUTEX_INITIALIZER'. (used where default mutex attributes are appropriate).
	pthread_mutex_init(&lock_x, NULL);

	// Create threads
	for (int i = 0; i < NUM_THREADS; ++i) {
		thr_data[i].tid = i;
		thr_data[i].stuff = (i + 1);
		int rc = pthread_create(&thr[i], NULL, threadFunction, &thr_data[i]);

		if (rc) {
			fprintf(stderr, "Error: pthread_create failed, rc: %d\n", rc);
			return EXIT_FAILURE;
		}
	}

	// Block until all threads complete (pthread_join)
	for (int i = 0; i < NUM_THREADS; ++i) {
		pthread_join(thr[i], NULL);
		//pthread_detach(thr[i]); 
	}

	// Destroy mutex when no thread needs it.
	pthread_mutex_destroy(&lock_x);

	// Use pthread_exit() to exit main thread without killing other threads.
	return EXIT_SUCCESS;
}

