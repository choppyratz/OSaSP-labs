#include <iostream>
#include <windows.h>
#include <string>


using namespace std;

CRITICAL_SECTION critsect;
STARTUPINFO Pinf1;
STARTUPINFO Pinf2;
PROCESS_INFORMATION proc1;
PROCESS_INFORMATION proc2;
LPCTSTR pBuf;
HANDLE fMap;
TCHAR szName[] = TEXT("MyFileMappingObject");

int main()
{
    InitializeCriticalSection(&critsect);

    fMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(critsect), szName);
    pBuf = (LPTSTR) MapViewOfFile(fMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(critsect));
    CopyMemory((PVOID)pBuf, &critsect, sizeof(critsect));


    ZeroMemory(&Pinf1, sizeof(Pinf1));
    Pinf1.cb = sizeof(Pinf1);

    ZeroMemory(&Pinf2, sizeof(Pinf2));
    Pinf2.cb = sizeof(Pinf2);

    CreateProcess(NULL, "C:\\Users\\German\\Desktop\\лабы\\осисп\\lab 1\\CriticalSection\\proc1\\bin\\Debug\\proc1.exe", NULL, NULL, FALSE, 0, NULL, NULL, &Pinf1, &proc1);
    CreateProcess(NULL, "C:\\Users\\German\\Desktop\\лабы\\осисп\\lab 1\\CriticalSection\\proc2\\bin\\Debug\\proc2.exe", NULL, NULL, FALSE, 0, NULL, NULL, &Pinf2, &proc2);

    WaitForSingleObject(proc1.hProcess, INFINITE);
    WaitForSingleObject(proc2.hProcess, INFINITE);

    CloseHandle(proc1.hThread);
    CloseHandle(proc1.hProcess);
    CloseHandle(proc2.hThread);
    CloseHandle(proc2.hProcess);

    DeleteCriticalSection(&critsect);
    return 0;
}
