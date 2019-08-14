// https://thispointer.com//c11-multithreading-part-7-condition-variables-explained/
//
// A network based application which does following tasks using two threads-
// Thread 1 -   Perform handshaking with server.
//              Wait for data to be loaded by Thread 2.
//              Send loaded data.
// Thread 2 -   Load data from Hard Disk.
//              Notify another Thread when data is loaded.

// Sometimes a thread needs to wait for an event to happen like a condition to become true or a task to be completed by another thread before it can start its operation. One way to achieve this with polling but it has disadvange of busy waiting.

// Condition Variables are used for signaling between threads. One thread can wait for it to get signaled, while other thread can signal this on occurance of an event. When a thread is waiting for Condition Variable, it will be in sleep state without consuming any CPU cycles. When other thread identifies that required event/condition has occured for which the first thread(s) is waiting, it can send wakeup signal to waiting thread(s). A mutex is required along with condition variable.

// How things work with condition variable
// =======================================
// Thread A acquire mutex associated with CondVar to get exclusive access
// Thread A calls the wait on condition variable, which takes a mutex-variable and a function to check for the event. If event-checker function returns true, then wait() returns and Thread A continues (Mutex was already acquired in previous step and thread continue to have it).
// If event-checker function returns false, then wait function puts this thread in sleep and releases the associated mutex atomically and adds it to the list of threads waiting on the current condition variable.
// The release of mutex may wake up some other Thread B who was waiting for it.
// Some other Thread B or C may send signal on this condition variable on occurance of appropriate event.
// On receiving signal on condition variable, the wait function first acquired mutex and it checks event with event-checker. If event-checker function returns false, then wait function releases the mutex and sleeps. If event-checker function returns true, then wait function wakes up Thread A. When it wakes up, it already has the mutex (acquired by wait function). Now thread 1 can proceed to operation it need to perform. (release the mutex if it is not needed).
// 
// It is not mandatory to pass event-checker function to wait(). If event-checker function is not passed, then wait function always block(sleep) and wakeup when a signal is received. In this case, event checking need to be done in the thread itself. Event checking is done in a loop to avoid spurious wakeups.
// 


#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std::placeholders;

class Application {
	std::mutex mMutex;
	std::condition_variable mCondVar;
	bool mBDataLoaded;
public:
	Application() {
		mBDataLoaded = false;
	}
	void loadData() {
		// Make This Thread sleep for 2 Second to simulate data loading
		std::cout<<"Thread 2: Loading Data"<<std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		std::cout<<"Thread 2: Data Loaded"<<std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		// Lock The Data structure
		std::lock_guard<std::mutex> guard(mMutex);
		// Set the flag to true, means data is loaded
		mBDataLoaded = true;
		// Notify the condition variable
		mCondVar.notify_one();
	}
	bool isDataLoaded() {
		return mBDataLoaded;
	}
	void mainTask() {
		std::cout<<"Thread 1: Stablish Network Connections."<<std::endl;
		// Acquire the lock
		std::unique_lock<std::mutex> lock(mMutex);
		// Start waiting for the Condition Variable to get signaled
		std::cout<<"Thread 1: Waiting for data to be loaded by Thread 2."<<std::endl;
		mCondVar.wait(lock, std::bind(&Application::isDataLoaded, this));
		std::cout<<"Thread 1: Sending loaded Data"<<std::endl;
	}
};

int main() {
	Application app;
	std::thread thread_1(&Application::mainTask, &app);
	std::thread thread_2(&Application::loadData, &app);
	thread_2.join();
	thread_1.join();
	return 0;
}

