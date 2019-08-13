#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "threadSafeQueue.h"

#define CAPACITY 5
MsgQueue msgQueue(CAPACITY);

// Create data-structure to pass information to thread threadFunction()
// Only a void pointer can be passed to thread function.
typedef struct {
	int tid;
	const char *msg;
} sender_thread_data_t;
typedef struct {
	int tid;
	const char **pmsg;
} receiver_thread_data_t;

// Thread/worker function.
// pthread accepts the thread function with following signature-
// void* (*threadFunc)(void*)
void * senderFunction(void *arg) {
	sender_thread_data_t *data = (sender_thread_data_t *)arg;

	sleep(2);
	printf("Sending Begin - thread id: %d, Msg: %s\n", data->tid, data->msg);
	msgQueue.enque(data->msg);
	printf("Sending End   - id: %d, Msg: %s\n", data->tid, data->msg);

	// Use pthread_exit() to exit a thread otherwise main and all threads will be terminated.
	pthread_exit(NULL);
}

void * receiverFunction(void *arg) {
	receiver_thread_data_t *data = (receiver_thread_data_t *)arg;

	sleep(2);
	printf("Receiving Begin - thread id: %d\n", data->tid);
	*data->pmsg = msgQueue.deque();
	printf("Receiving End   - thread id: %d, Msg: %s\n", data->tid, *data->pmsg);

	// Use pthread_exit() to exit a thread otherwise main and all threads will be terminated.
	pthread_exit(NULL);
}

#define NUM_THREADS 26

int main(int argc, char **argv) {
	int i=0, j=0;
	const char *sendMsgs[26] = {
		"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
		"n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"
	};
	const char *recMsgs[26];
	pthread_t send_thr[NUM_THREADS];
	pthread_t rec_thr[NUM_THREADS];
	sender_thread_data_t send_data[NUM_THREADS];
	receiver_thread_data_t rec_data[NUM_THREADS];
	msgQueue.enableDebug();


	// Test 1 - send 10 msg, wait 1 second, receive 10 msg.
	for(; i < 10; ++i) {
		send_data[i].tid = i;
		send_data[i].msg = sendMsgs[i];
		int rc = pthread_create(&send_thr[i], NULL, senderFunction, &send_data[i]);
		if (rc) {
			fprintf(stderr, "Error: pthread_create failed, rc: %d\n", rc);
			return EXIT_FAILURE;
		}
	}
	sleep(1);
	for(; j < 10; ++j) {
		rec_data[j].tid = j;
		rec_data[j].pmsg = &recMsgs[j];
		int rc = pthread_create(&rec_thr[j], NULL, receiverFunction, &rec_data[j]);
		if (rc) {
			fprintf(stderr, "Error: pthread_create failed, rc: %d\n", rc);
			return EXIT_FAILURE;
		}
	}
	// Block until all threads complete (pthread_join)
	for (int i = 0; i < 10; ++i) {
		pthread_join(send_thr[i], NULL);
		pthread_join(rec_thr[i], NULL);
	}
	printf("Test1 Finished!!!\n\n");


	// Test 2 - schedule 10 receive threads, wait 1 second, schedule 10 send threads.
	i=0, j=0;
	for(; j < 10; ++j) {
		rec_data[j].tid = j;
		rec_data[j].pmsg = &recMsgs[j];
		int rc = pthread_create(&rec_thr[j], NULL, receiverFunction, &rec_data[j]);
		if (rc) {
			fprintf(stderr, "Error: pthread_create failed, rc: %d\n", rc);
			return EXIT_FAILURE;
		}
	}
	sleep(1);
	for(; i < 10; ++i) {
		send_data[i].tid = i;
		send_data[i].msg = sendMsgs[i];
		int rc = pthread_create(&send_thr[i], NULL, senderFunction, &send_data[i]);
		if (rc) {
			fprintf(stderr, "Error: pthread_create failed, rc: %d\n", rc);
			return EXIT_FAILURE;
		}
	}
	// Block until all threads complete (pthread_join)
	for (int i = 0; i < 10; ++i) {
		pthread_join(send_thr[i], NULL);
		pthread_join(rec_thr[i], NULL);
	}
	printf("Test2 Finished!!!\n\n");


	return 0;
}

