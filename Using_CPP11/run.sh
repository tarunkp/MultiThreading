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

g++ -Wall -std=c++11 -pthread threadSafeQueue.cpp threadSafeQueue_tests.cpp -o queueTests
./queueTests


