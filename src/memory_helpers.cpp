#include "memory_helpers.h"

using std::string;
using std::to_string;

const string suffixes[] = { "B", "KB", "MB", "GB", "TB", "PB" };
const size_t sizes[] = { 1, 1 << 10, 1 << 20, 1 << 30, 1ull << 40, 1ull << 50 };

size_t truncate(size_t size, memory_size resolution)
{
    const size_t factor = sizes[resolution];
    return (size / factor) * factor;
}

std::string humanize_size(size_t size)
{
    if (size == 0)
    {
        return "0B";
    }

    string ret;

    memory_size unit = pb;
    while (size > 0)
    {
        const size_t n = size / sizes[unit];
        if (n != 0)
        {
            ret += to_string(n) + " " + suffixes[unit];
            size -= n * sizes[unit];
            if (size > 0)
            {
                ret += ", ";
            }
        }
        unit = static_cast<memory_size>(unit - 1);
    }

    return ret;
}
