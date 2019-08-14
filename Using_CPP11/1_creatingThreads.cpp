// Create few independent threads and wait for them to finish in main().
// CPP11 thread accept a callable as a function to run as thread.
// These callable could be a function, a function object, member function or a lambda.
// https://thispointer.com//c-11-multithreading-part-1-three-different-ways-to-create-threads/

#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>

// Function for creating thread
void threadFunction(int a1) {
	std::cout<<"Inside Thread : ID  = "<<std::this_thread::get_id()<<std::endl;
	std::cout<<"threadFunction - Argument: a1 = "<<a1<<"\n\n";
}

// A callable class for creating thread
struct threadCallableClass {
	void operator()(int a1, int a2) {
		std::cout<<"Inside Thread : ID  = "<<std::this_thread::get_id()<<std::endl;
		std::cout<<"threadCallableClass - Argument: a1="<<a1<<", a2="<<a2<<"\n\n";
	}
};

// A class with member function for creating thread
struct ThreadCallableClassMemb {
	void memberFn(int a1) {
		std::cout<<"Inside Thread : ID  = "<<std::this_thread::get_id()<<std::endl;
		std::cout<<"ThreadCallableClassMemb - Argument: a1 = "<<a1<<"\n\n";
	}
};
ThreadCallableClassMemb threadMembObj;

// RAII (Resource Acquisition Is Initialization) wrapper 
class ThreadRAII {
	std::thread & mThread;
public:
	ThreadRAII(std::thread  & threadObj) : mThread(threadObj) {}
	~ThreadRAII() {
		if(mThread.joinable()) // if thread is joinable then detach it
			mThread.detach();
	}
};

int main(int argc, char **argv) {
	int data1=100, data2=200, data3=300, data4=400;

	// Create thread using function pointer
	std::thread thread1(threadFunction, 10);
	// Always call either join() or detach() exactly once (even in case of exception).
	// Calling join() or detach() more than once or not calling will terminate the program.
	// Check joinable() to check if join() or detach() can be called.
	if(thread1.joinable())
		thread1.join(); // Join thread

	// Create thread using function object
	// Arguments are passed by copying these to new threads stack.
	// A reference becomes reference to variable on the new thread's stack.
	// Don't pass pointer/ref to local variable unless they will live longer than the new thread
	std::thread thread2(threadCallableClass(), data1, data2);
	if(thread2.joinable())
		thread2.detach(); // Detach thread

	// Create thread using member function. First two arguments specifies the function to be called
	// Pass pointer to memberFn as thread-function and pointer to object as second argument.
	std::thread thread3(&ThreadCallableClassMemb::memberFn, &threadMembObj, data3);
	// Alternatively, std::ref(threadMembObj) can be used as second argument(as shown below)
	//std::thread thread3(&ThreadCallableClassMemb::memberFn, std::ref(threadMembObj), data3);
	if(thread3.joinable())
		thread3.join(); // Join thread

	// Create thread using lambda function
	auto fn = [](int a1) {
		std::cout<<"Inside Thread : ID  = "<<std::this_thread::get_id()<<std::endl;
		std::cout<<"lambdaFunction - Argument: a1 = "<<a1<<"\n\n";
	};
	std::thread thread4(fn, data4);
	// Use ThreadRAII, it will detach thread if thread creator missed to join/detach,
	// or an exception occured before join/detach.
	ThreadRAII raii4(thread4);

	// How to make thread sleep (C++14)
	using namespace std::chrono_literals;
	std::cout << "Start waiting\n" << std::flush;
	auto start = std::chrono::high_resolution_clock::now();
	//std::chrono::milliseconds dur(2000); // Pass dur to sleep_for(dur)
	std::this_thread::sleep_for(2s);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end-start;
	std::cout << "Waited " << elapsed.count() << " ms\n";

	return 0;
}


