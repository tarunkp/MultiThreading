#

#
g++ -Wall -std=c++14 -pthread 1_creatingThreads.cpp -o creatingThreads
./creatingThreads

g++ -Wall -std=c++11 -pthread 2_passingRefToThreads.cpp -o passingRefToThreads
./passingRefToThreads

g++ -Wall -std=c++11 -pthread 3_usingMutex.cpp -o usingMutex
./usingMutex

g++ -Wall -std=c++11 -pthread 4_usingAtomic.cpp -o usingAtomic
./usingAtomic

g++ -Wall -std=c++11 -pthread 5_usingConditionVar.cpp -o usingCondVar
./usingCondVar

g++ -Wall -std=c++11 -pthread 6_promiseAndFuture1.cpp -o promiseAndFuture1
./promiseAndFuture1

g++ -Wall -std=c++11 -pthread 7_promiseAndFuture2.cpp -o promiseAndFuture2
./promiseAndFuture2

g++ -Wall -std=c++11 -pthread 8_usingAsync.cpp -o usingAsync
./usingAsync

g++ -Wall -std=c++11 -pthread 9_packaged_task.cpp -o packaged_task
./packaged_task

g++ -Wall -std=c++11 -pthread threadSafeQueue.cpp threadSafeQueue_tests.cpp -o queueTests
./queueTests

