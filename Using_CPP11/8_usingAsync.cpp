// http://www.cplusplus.com/reference/future/async/
//
// std::async can be seen as a high level interface to std::threads. 
// std::async takes as argument a callable object and returns a std::future, that will store the result returned by your function or an error message.
//
// Argument launch policy determine the asynchronous behaviour of std::async. There are 3 different launch policies possible-
// 	std::launch::async - Asynchronous behaviour. The passed function will be executed in seperate thread either immediatly or when resources are available.
// 	std::launch::deferred - Synchronous behaviour. Function will be called when other thread will call get() on future to access the shared state.
// 	std::launch::async | std::launch::deferred - Default behaviour. Thread will run asynchronously or not depending on the load on system. User have no control over it.
//

#include <iostream>       // std::cout
#include <future>         // std::async, std::future

// A non-optimized way of checking for prime numbers:
bool isPrime (int x) {
	std::cout << "Thread B: Calculating, please wait.\n";
	for (int i=2; i<x; ++i) {
		if (x%i == 0)
			return false;
	}
	return true;
}


int main(int argc, char **argv) {
	// call isPrime() asynchronously.
	// std::future<bool> fut = std::async (isPrime, 313222313); // With default launch policy.
	std::future<bool> fut = std::async (std::launch::async, isPrime, 313222313);

	std::cout << "Thread A: Checking whether 313222313 is prime.\n";

	bool ret = fut.get();      // waits for isPrime to return

	if (ret)
		std::cout << "Thread A: It is prime!\n";
	else
		std::cout << "Thread A: It is not prime.\n";

	return 0;
}

