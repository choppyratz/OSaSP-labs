#include <iostream>
#include <windows.h>

using namespace std;

LPCRITICAL_SECTION critsect;
TCHAR szName[] = TEXT("MyFileMappingObject");

int main()
{
    HANDLE fMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szName);
    critsect = (LPCRITICAL_SECTION)MapViewOfFile(fMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(critsect));

    while (true) {
        if (TryEnterCriticalSection(critsect)) {
            std::cout << "Proc 1" << GetCurrentThreadId() << std::endl;
            for (int i = 0; i < 100; i++)
                std::cout << "Proc1" << endl;
            LeaveCriticalSection(critsect);
            break;
        }
    }

    UnmapViewOfFile(critsect);
    CloseHandle(fMap);
    return 0;
}
