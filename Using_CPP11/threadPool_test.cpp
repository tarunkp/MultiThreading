//

#include <iostream>
#include "threadPool.h"

int doubleFun(int x) {
	std::cout << "Worker function: Computing double of " << x << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "Worker function: Finished computing double of " << x << std::endl;
	return 2*x;
}

int main(int argc, char **argv) {
	// create thread pool with 2 worker threads
	ThreadPool pool(3);

	std::vector<std::future<int>> futures;
	// enqueue and store future
	for(size_t i = 0; i < 5; ++i) {
		futures.push_back(pool.enqueue(doubleFun, i+1));
	}

	// get result from future
	for(size_t i = 0; i < 5; ++i) {
		auto ans = futures[i].get();
		std::cout << "Double of "<< i+1 << " = " << ans << std::endl;
	}
}

