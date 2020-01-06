#ifndef PLATFORM_MEMORY_H_
#define PLATFORM_MEMORY_H_

#include <string>

enum memory_size
{
    b, kb, mb, gb, tb, pb
};

size_t get_free_memory();
size_t truncate(size_t size, memory_size precision);
std::string humanize_size(size_t size);

#endif // PLATFORM_MEMORY_H_
