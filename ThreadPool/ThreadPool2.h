#pragma once
#include <windows.h>
#include "Task.h"
#include "Logger.h"
#include <vector>

class ThreadPool
{
    public:
        ThreadPool(int threadCount);
        virtual ~ThreadPool();
        void callThread(void (*proc)());
        void WaitForThreads();
    protected:

    private:
        int threadCount;
        std::vector<Task*> threads;
        Logger *logger;
};

