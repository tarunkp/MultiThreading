// Create few independent threads and wait for them to finish in main().
// CPP11 thread accept a callable as a function to run as thread.
// These callable could be a function, a function object, member function or a lambda.

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


int main(int argc, char **argv) {

	// Create thread using function
	std::thread thread1(threadFunction, 100);
	thread1.join();

	// Create thread using function object
	std::thread thread2(threadCallableClass(), 200, 300);
	thread2.join();

	// Create thread using member function
	ThreadCallableClassMemb threadMembObj;
	std::thread thread3(&ThreadCallableClassMemb::memberFn,std::ref(threadMembObj),400);
	thread3.join();

	// Create thread using lambda function
	auto fn = [](int a1) {
		std::cout<<"Inside Thread : ID  = "<<std::this_thread::get_id()<<std::endl;
		std::cout<<"lambdaFunction - Argument: a1 = "<<a1<<"\n\n";
	};
	std::thread thread4(fn, 500);
	thread4.join();

	// How to make thread sleep (C++14)
	using namespace std::chrono_literals;
	std::cout << "Start waiting\n" << std::flush;
	auto start = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(2s);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end-start;
	std::cout << "Waited " << elapsed.count() << " ms\n";

	return 0;
}

