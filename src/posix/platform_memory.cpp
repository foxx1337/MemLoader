#include "../memory_helpers.h"

#include <unistd.h>

size_t get_free_memory()
{
    long page_size = sysconf(_SC_PAGESIZE);
    long pages = sysconf(_SC_AVPHYS_PAGES);
    return static_cast<size_t>(pages * page_size);
}
