#include <iostream>
#include <windows.h>
#include "ThreadPool2.h"


using namespace std;


void test() {
    cout << "Hello world3" << endl;
}

int main()
{
    ThreadPool* thd = new ThreadPool(3);
    thd->callThread(&test);
    thd->callThread(&test);
    thd->callThread(&test);
    thd->callThread(&test);
    thd->WaitForThreads();
    delete thd;
    cin.get();
}

