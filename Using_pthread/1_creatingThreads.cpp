// https://randu.org/tutorials/threads/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3
void printPt(pthread_t pt);

// Create data-structure to pass information to thread threadFunction()
// Only a void pointer can be passed to thread function.
typedef struct {
	int tid;
	double stuff;
} thread_data_t;

// Thread/worker function.
// pthread accepts the thread function with following signature-
// void* (*threadFunc)(void*)
void * threadFunction(void *arg) {
	thread_data_t *data = (thread_data_t *)arg;

	printf("Hello from threadFunction, thread id: %d\n", data->tid);

	// pthread_self() return handle to current thread.
	pthread_t self = pthread_self();
	printPt(self);

	// Use pthread_exit() to exit a thread otherwise main and all threads will be terminated.
	pthread_exit(NULL);
}

int main(int argc, char **argv) {
	pthread_t thr[NUM_THREADS];
	// Create thread_data_t argument array
	thread_data_t thr_data[NUM_THREADS];

	// Create threads
	for (int i = 0; i < NUM_THREADS; ++i) {
		thr_data[i].tid = i;
		int rc = pthread_create(&thr[i], NULL, threadFunction, &thr_data[i]);

		if (rc) {
			fprintf(stderr, "Error: pthread_create failed, rc: %d\n", rc);
			return EXIT_FAILURE;
		}
	}

	// Block until all threads complete (pthread_join)
	for (int i = 0; i < NUM_THREADS; ++i) {
		pthread_join(thr[i], NULL);
	}

	// Use pthread_exit() to exit main thread without killing other threads.
	return EXIT_SUCCESS;
}

void printPt(pthread_t pt) {
	unsigned char *ptc = (unsigned char*)(void*)(&pt);
	printf("Thread id = 0x");
	for (size_t i=0; i<sizeof(pt); i++) {
		printf("%02x", (unsigned)(ptc[i]));
	}
	printf("\n");
}


