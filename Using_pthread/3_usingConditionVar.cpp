// https://randu.org/tutorials/threads/
// http://www.cs.kent.edu/~ruttan/sysprog/lectures/multi-thread/multi-thread.html
// https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html

// Two threads working on a shared_cout. 
// One thread increments it. The other thread resets shared_count when it reaches to MAX_COUNT.

// Sometimes a thread needs to wait for an event to happen like a condition to become true or a task to be completed by another thread before it can start its operation. One way to achieve this with polling but it has disadvange of busy waiting.

// Condition Variables are used for signaling between threads. One thread can wait for it to get signaled, while other thread can signal this on occurance of an event. When a thread is waiting for Condition Variable, it will be in sleep state without consuming any CPU cycles. When other thread identifies that required event/condition has occured for which the first thread(s) is waiting, it can send wakeup signal to waiting thread(s). A mutex is required along with condition variable.

// How things work with condition variable
// =======================================
// Thread 1 acquire mutex associated with CondVar to get exclusive access to check for event
// Thread 1 checks for event/condition (in a loop actually to avoid spurious wakeup). 
// Possobility 1 - Condition is not met, then Thread 1 calls the wait on condition variable. Wait function  puts this thread in sleep and releases the associated mutex atomically and adds it to the list of threads waiting on the current condition variable.
// This may wake up some other thread who was waiting for this mutex.
// Some other thread should send signal on this condition variable for this thread to wake up when appropriate event has occured.
// When this thread wakes up, the wait function has already acquired mutex for this thread and it can check for condition again. 
// Possibility 2 - Now if condition is met, it can proceed to operation it need to perform. (release the mutex if it is not needed).


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


#define MAX_COUNT 5

// Shared variable between threads
int shared_count;
// Mutex for shared variable
pthread_mutex_t count_lock;
// Conditional variable
pthread_cond_t count_cond = PTHREAD_COND_INITIALIZER;

// The function to increment shared counter.
void * incrementorFunc(void *arg) {
	while(1) {
		sleep(1);

		// Lock mutex count_lock so shared_count can be read without entering a potential race condition. 
		pthread_mutex_lock(&count_lock);

		// Increment the shared_count variable
		shared_count++;
		printf("From incrementorFunc:\tshared_count = %d\n", shared_count);

		if (shared_count == MAX_COUNT) {
			// count_lock should be released before signal or broadcast.
			pthread_mutex_unlock(&count_lock);
			pthread_cond_signal(&count_cond);
		} else {
			pthread_mutex_unlock(&count_lock);
		}
	}

	pthread_exit(NULL);
}

void * resetorFunc(void *arg) {
	while(1) {
		sleep(1);

		// Thread code blocks here until MAX_COUNT is reached
		// Lock mutex count_lock so we can read the value of shared_count without entering a potential race condition. 
		pthread_mutex_lock(&count_lock);

		// Check condition in a loop to avoid spurious wakeup problems
		while (shared_count < MAX_COUNT) {
			// pthread_cond_wait() must be called after acquiring mutex count_lock.
			pthread_cond_wait(&count_cond, &count_lock);
		}

		// Modify the shared_count
		shared_count = 0;
		printf("From resetorFunc:\tshared_count = %d\n\n", shared_count);
		// Release the mutex count_lock
		pthread_mutex_unlock(&count_lock);
	}

	pthread_exit(NULL);
}

int main(int argc, char **argv) {
	shared_count = 0;

	// Initialize pthread mutex protecting "shared_count" and "count_cond\n" with pthread_mutex_init()
	// or use 'count_lock = PTHREAD_MUTEX_INITIALIZER'. (used where default mutex attributes are appropriate).
	pthread_mutex_init(&count_lock, NULL);

	// Initialize conditional variable with pthread_cond_init()
	// or use 'count_cond = PTHREAD_COND_INITIALIZER'. (used where default cond-var attributes are appropriate).
	pthread_cond_init(&count_cond, NULL);

	printf("From main:\tshared_count = %d\n", shared_count);

	// Create threads
	pthread_t t_inc, t_rst;
	int rc1 = pthread_create(&t_inc, NULL, incrementorFunc, NULL);
	if (rc1) {
		fprintf(stderr, "Error: pthread_create failed for incrementorFunc.\n");
		return EXIT_FAILURE;
	}
	int rc2 = pthread_create(&t_rst, NULL, resetorFunc, NULL);
	if (rc2) {
		fprintf(stderr, "Error: pthread_create failed for resetorFunc.\n");
		return EXIT_FAILURE;
	}

	// Block until all threads complete (pthread_join)
	pthread_join(t_inc, NULL);
	pthread_join(t_rst, NULL);

	// Use pthread_exit() to exit main thread without killing other threads.
	return EXIT_SUCCESS;
}


