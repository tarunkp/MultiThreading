#!/usr/bin/env bash

#
g++ -Wall -pthread 1_creatingThreads.cpp -o creatingThreads
./creatingThreads

g++ -Wall -pthread 2_usingMutex.cpp -o usingMutex
./usingMutex

g++ -Wall -pthread 3_usingConditionVar.cpp -o usingConditionVar
timeout 21s ./usingConditionVar

g++ -Wall -pthread threadSafeQueue.cpp threadSafeQueue_tests.cpp -o queueTests
./queueTests

