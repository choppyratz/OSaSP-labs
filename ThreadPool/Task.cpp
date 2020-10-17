#include "Task.h"
#include <string>

Task::Task(Logger *_logger)
{
    Task::logger = _logger;
    Task::handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Task::waitFn, this, CREATE_SUSPENDED, NULL);
    Task::isActive = false;
}

void Task::call(std::function<void()> threadProc) {
    Task::func = threadProc;
    Task::logger->info("[Info] Start new Task with thread id: " + std::to_string(GetThreadId(Task::handle)));
    try {
        ResumeThread(Task::handle);
        Task::isActive = true;
    }
    catch (...) {
        Task::logger->info("[Error] error occurred while processing task");
    }
}

Task::~Task()
{
    TerminateThread(Task::handle, 0);
    CloseHandle(Task::handle);
}
