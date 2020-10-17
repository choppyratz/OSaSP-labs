#include "ThreadPool2.h"
#include <iostream>
#include "Task.h"
#include <vector>
#include <string>

ThreadPool::ThreadPool(int threadCount)
{
   ThreadPool::threadCount = threadCount;
   ThreadPool::logger = new Logger();
   for (int i = 0; i < threadCount; i++) {
       ThreadPool::threads.push_back(new Task(ThreadPool::logger));
   }
   logger->info("[Info] Init thread pool");
   logger->info("[Info] Thread count: " + std::to_string(threadCount));

}

void ThreadPool::callThread(void (*proc)()) {
	bool isStartedTask = false;
    for (int i = 0; i < ThreadPool::threadCount; i++) {
        if (!ThreadPool::threads[i]->isActive) {
            ThreadPool::threads[i]->call(proc);
			isStartedTask = true;
            break;
        }
    }

	if (!isStartedTask) {
		logger->info("[Warning] Cannot start task. Thread poll has no free threads");
	}
}

void ThreadPool::WaitForThreads()
{
	bool result;
	do
	{
		result = true;
		for (int i = 0; i < ThreadPool::threadCount; i++) {
			result &= !ThreadPool::threads[i]->isActive;
		}
	} while (!result);
}


ThreadPool::~ThreadPool()
{
	for (int i = 0; i < ThreadPool::threadCount; i++) {
		delete threads[i];
	}
	logger->info("[Info] Stop thread pool");
	delete logger;
}
