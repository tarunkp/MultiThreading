//https://thispointer.com//c11-multithreading-part-3-carefully-pass-arguments-to-threads/
// Be careful when passing references as thread's arguments.
// std::ref() should be used while creating threads for passing references.

#include <iostream>
#include <thread>

// Only const reference is allowed for function to be used as a thread
void threadFunc(int const & x) {
	int & y = const_cast<int &>(x);
	y++;
	std::cout<<"Inside Thread x = "<<x<<std::endl;
}

int main(int argc, char **argv) {
	// Initialize variable x
	int x = 9;

	// Passing direct reference to variable x during thread creation
	std::cout<<"In Main Thread : Before first Thread Start x = "<<x<<std::endl;
	// Arguments are passed by copying these to new threads stack.
	// A reference becomes reference to variable on the new thread's stack.
	std::thread thread1(threadFunc, x);
	thread1.join();
	std::cout<<"In Main Thread : After first Thread Joins x = "<<x<<std::endl;
	std::cout<<std::endl;

	// Passing reference through std::ref(x) during thread creation
	std::cout<<"In Main Thread : Before second Thread Start x = "<<x<<std::endl;
	// Argument passed with std::ref() really refer to original variable
	std::thread thread2(threadFunc, std::ref(x));
	thread2.join();
	std::cout<<"In Main Thread : After second Thread Joins x = "<<x<<std::endl;

	return 0;
}

