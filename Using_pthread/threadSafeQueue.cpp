#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "threadSafeQueue.h"

MsgQueue::MsgQueue(int capacity) {
	mCap = capacity;
	mQue = (char**)malloc(sizeof(char*) * capacity);
	mHead = mTail = 0;
	mDebug = false;
	mMutexCount = PTHREAD_MUTEX_INITIALIZER;
	mCondVarEnque  = PTHREAD_COND_INITIALIZER;
	mCondVarDeque  = PTHREAD_COND_INITIALIZER;
}

MsgQueue::~MsgQueue() {
	free(mQue);
}

void MsgQueue::enableDebug() {
	mDebug = true;
}

// Must be called within mutex lock mMutexCount
void MsgQueue::addMsg(const char* msg) {
	mQue[mHead] = (char*) msg;
	mHead++;
	if(mHead >= mCap)
		mHead = 0;
	mCount++;
}

const char* MsgQueue::getMsg() {
	char* msg = mQue[mTail];
	mTail++;
	if(mTail >= mCap)
		mTail = 0;
	mCount--;
	return msg;
}

void MsgQueue::enque(const char *msg) {
	pthread_mutex_lock( &mMutexCount );
	while(mCount == mCap) {
		pthread_cond_wait( &mCondVarDeque, &mMutexCount );
	}
	addMsg(msg);
	if(mDebug) {
		printf("Enqueued : %s, count=%d\n", msg, mCount);
	}
	pthread_mutex_unlock( &mMutexCount );

	//pthread_cond_broadcast( &mCondVarEnque );
	pthread_cond_signal( &mCondVarEnque );
}

const char * MsgQueue::deque() {
	pthread_mutex_lock( &mMutexCount );
	while(mCount == 0) {
		pthread_cond_wait( &mCondVarEnque, &mMutexCount );
	}
	const char* msg = getMsg();
	if(mDebug) {
		printf("Dequeued : %s, count=%d\n", msg, mCount);
	}
	pthread_mutex_unlock( &mMutexCount );

	//pthread_cond_broadcast( &mCondVarDeque );
	pthread_cond_signal( &mCondVarDeque );
	return msg;
}

