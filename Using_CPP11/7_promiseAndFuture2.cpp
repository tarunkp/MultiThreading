// http://www.cplusplus.com/reference/future/promise/
// Promise and Future are two endpoints of event driven communication and let's us implement certain event driven synchronizations.
// These endpoints can be moved to separate threads. So the communication is taking place between threads.
// 
// A promiseObj sends value/signal to associated futureObj and this value/signal can be accessed with futureObj.get().
// 
// A std::promise permits-
// 	To set a value, a notification or an exception. That result can in addition delayed be provided by the promise.
// A std::future permits to-
// 	Pick up the result/value from the promise.
// 	Asks the promise, if the value is available.
// 	Wait/sleep for the notification from the promise if results are not available yet. => Replacement for Condition-Variables.
// 	Create a shared future (std::shared_future).
//
// First create a std::promise object. 
// Each std::promise object has an associated std::future object, which can be obtained with promiseObj.get_future().
// Pass promise to event creator and future to event receiver.
// Event creator uses promiseObj.set_value() to signal the value.
// Event receiver uses futureObj.get() to get the event. This function waits if value is not yet available.
// 


#include <iostream>
#include <thread>
#include <future>
#include <utility>
#include <functional>

void print_int (std::future<int>& fut) {
	std::cout << "Thread B: Waiting for value.\n";
	int x = fut.get();
	std::cout << "Thread B: value = " << x << '\n';
}

int main(int argc, char **argv) {
	std::promise<int> prom;                      // create promise

	std::future<int> fut = prom.get_future();    // engagement with future

	std::thread th1 (print_int, std::ref(fut));  // send future to new thread

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	std::cout << "Thread A: Sending value.\n";
	prom.set_value (10);                         // fulfill promise

	th1.join();
	return 0;
}


