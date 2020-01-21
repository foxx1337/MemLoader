#include "memory_helpers.h"

#include <memory>
#include <vector>


using std::make_unique;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;

const string suffixes[] = { "B", "KB", "MB", "GB", "TB", "PB" };
const size_t sizes[] = { 1, 1 << 10, 1 << 20, 1 << 30, 1ull << 40, 1ull << 50 };

size_t apply_pressure(size_t max_size)
{
    size_t size = 0;
    const size_t chunk_size = 1024 * 1024 * 16;
    const size_t num_chunks = max_size / chunk_size;

    vector<unique_ptr<char []>> allocations;

    for(size_t step = 0; step < num_chunks; ++step)
    {
        char *buf = new char[chunk_size];

        for (size_t i = 0; i < chunk_size; ++i)
        {
            buf[i] = i % 256;
        }

        if (buf != nullptr)
        {
            allocations.emplace_back(buf);
            size += chunk_size;
        }
        else
        {
            break;
        }
    }

    return size;
}

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
