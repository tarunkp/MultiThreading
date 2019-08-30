// https://github.com/progschj/ThreadPool

#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool {
public:
    ThreadPool(size_t numThreads = std::thread::hardware_concurrency());
    ~ThreadPool();

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    // Container to keep track of threads, so we can join them when pool is stopped.
    std::vector< std::thread > mWorkers;
    // The task queue
    std::queue< std::function<void()> > mTasks;
    
    // Synchronization / control variables
    std::mutex mQueueMutex;
    std::condition_variable mCondVar;
    bool mStop;
};
 
// The constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t numThreads)
    :   mStop(false) {
    for(size_t i = 0; i < numThreads; ++i)
        mWorkers.emplace_back(
            [this] {
                while(1) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->mQueueMutex);
                        this->mCondVar.wait(lock,
                            [this]{ return this->mStop || !this->mTasks.empty(); });
                        if(this->mStop && this->mTasks.empty())
                            return;
                        task = std::move(this->mTasks.front());
                        this->mTasks.pop();
                    }

                    task();
                }
            }
        );
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();
    {
        std::lock_guard<std::mutex> lock(mQueueMutex);

        // don't allow enqueueing after stopping the pool
        if(mStop)
            throw std::runtime_error("Enqueue on stopped ThreadPool");

        mTasks.emplace([task](){ (*task)(); });
    }
    mCondVar.notify_one();
    return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(mQueueMutex);
        mStop = true;
    }
    mCondVar.notify_all();
    for(std::thread &worker: mWorkers)
        worker.join();
}

