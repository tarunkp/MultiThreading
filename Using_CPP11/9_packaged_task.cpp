// https://thispointer.com/c11-multithreading-part-10-packaged_task-example-and-tutorial/
//
// Consider scenario where a function is used synchronously, and it is needed to be used asynchronously.
// If we use promise and future, then we will need to change the interface of the function to pass promise.
// std::packaged_task let us use this function for asynchronous processing without modifying the function interface.
//
// std::packaged_task<> is a class template and represents an asynchronous task.
// It encapsulates a shared state that stores the value returned or thrown exception by associated callback.
// When std::packaged_task<> is called in a separate thread, it calls the associated function and stores the return value/exception in its internal shared state. This value can be accessed in other thread through associated std::future object.


#include <iostream>
#include <thread>
#include <future>
#include <string>
 
// Fetch some data from DB
std::string getDataFromDB( std::string token) {
	// Do some stuff to fetch the data
	std::cout << "Thread B: Fetching data from DB!!!\n";
	std::string data = token + "_val";
	return data;
}
 
int main(int argc, char **argv) {
 
	// Create a packaged_task<> that encapsulated the function
	std::packaged_task<std::string (std::string)> task(getDataFromDB);
 
	// Fetch the associated future<> from packaged_task<>
	std::future<std::string> result = task.get_future();
 
	// Pass the packaged_task to thread to run asynchronously
	std::thread th(std::move(task), "key");
 
	// Join the thread. Its blocking and returns when thread is finished.
	th.join();
 
	// Fetch the result of packaged_task<> i.e. value returned by getDataFromDB()
	std::string data =  result.get();
 
	std::cout << "Thread A: Data fetched from DB: "<<  data << std::endl;
 
	return 0;
}





