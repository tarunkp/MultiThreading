// https://www.modernescpp.com/index.php/promise-and-future
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

void prodDiv(std::promise<int>&& prodPromise, std::promise<int>&& divPromise, int a, int b) {
	std::cout << "Worker thread: Computing Prod.\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "Worker thread: Finished computing Prod.\n";
	prodPromise.set_value(a*b);

	std::cout << "Worker thread: Computing Div.\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "Worker thread: Finished computing Div.\n";
	divPromise.set_value(a/b);
}

int main(int argc, char **argv) {
	int a=10, b=5;

	// Define the promises
	std::promise<int> prodPromise;
	std::promise<int> divPromise;

	// Get the futures
	std::future<int> prodFuture = prodPromise.get_future();
	std::future<int> divFuture = divPromise.get_future();

	// Calculate the result in a separat thread
	std::thread Thread(prodDiv, std::move(prodPromise), std::move(divPromise), a, b);

	// Get the result
	int prod = prodFuture.get();
	std::cout <<"Main Thread: "<<a<<" * "<<b<<" = "<< prod << std::endl;
	int div = divFuture.get();
	std::cout <<"Main Thread: "<<a<<" / "<<b<<" = "<< div << std::endl;

	Thread.join();
}

