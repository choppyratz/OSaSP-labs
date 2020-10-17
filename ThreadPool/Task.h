#ifndef TASK_H
#define TASK_H

#include <functional>
#include <windows.h>
#include "Logger.h"


class Task
{
    public:
        Task(Logger *_logger);
        virtual ~Task();
        void call(std::function<void()> threadProc);
        bool isActive;
    protected:

    private:
        std::function<void()> func;
        HANDLE handle;
        Logger *logger;
        static void waitFn(Task* cP)
        {
            while (true) {
                cP->func();
                cP->isActive = false;
                SuspendThread(cP->handle);
            }
        }

};

#endif
