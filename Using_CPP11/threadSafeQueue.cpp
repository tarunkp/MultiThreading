#include <iostream>
#include <functional>
#include "threadSafeQueue.h"

MsgQueue::MsgQueue(int capacity) : mCap(capacity), mCount(0), mDebug(false) {}

MsgQueue::~MsgQueue() {}

void MsgQueue::enableDebug() {
	mDebug = true;
}

// Must be called within mutex lock mMutexCount
void MsgQueue::addMsg(std::string msg) {
	mQue.push_back(msg);
	mCount++;
}

// Must be called within mutex lock mMutexCount
std::string MsgQueue::getMsg() {
	std::string msg = mQue.front();
	mQue.pop_front();
	mCount--;
	return msg;
}

bool MsgQueue::hasSpaceForEnque() {
	return mCount < mCap;
}

bool MsgQueue::hasDataToDeque() {
	return mCount > 0;
}

void MsgQueue::enque(std::string msg) {
	std::unique_lock<std::mutex> lock(mMutexCount);
	mCondVarDeque.wait(lock, std::bind(&MsgQueue::hasSpaceForEnque, this));
	addMsg(msg);
	if(mDebug) {
		std::cout<<"Enqueued : "<<msg<<", count="<<mCount<<std::endl;
	}
	lock.unlock(); // It works without unlock as well but lock is not needed after this point.
	//mCondVarEnque.notify_all();
	mCondVarEnque.notify_one();
}

std::string MsgQueue::deque() {
	std::unique_lock<std::mutex> lock(mMutexCount);
	mCondVarEnque.wait(lock, std::bind(&MsgQueue::hasDataToDeque, this));
	std::string msg = getMsg();
	if(mDebug) {
		std::cout<<"Dequeued : "<<msg<<", count="<<mCount<<std::endl;
	}
	lock.unlock(); // It works without unlock as well but lock is not needed after this point.
	//mCondVarDeque.notify_all();
	mCondVarDeque.notify_one();
	return msg;
}

