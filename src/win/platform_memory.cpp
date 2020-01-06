#include "../memory_helpers.h"

#include <windows.h>

size_t get_free_memory()
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);

    BOOL ret = GlobalMemoryStatusEx(&status);

    if (!ret)
    {
        return 0;
    }

    return status.ullAvailPhys;
}
