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
        if (TryEnterCriticalSection(criticalSection)) {
            std::cout << "TestProcess2, id = " << GetProcessId(GetCurrentProcess()) << std::endl;
            for (int i = 0; i < 10000; i++)
                std::cout << 1;
            std::cout << std::endl;
            LeaveCriticalSection(criticalSection);
            break;
        }
    }

    //UnmapViewOfFile(critsect);
    CloseHandle(fMap);
    return 0;
}
