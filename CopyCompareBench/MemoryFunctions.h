#ifndef MEMLOADER_MEMORYFUNCTIONS_H_
#define MEMLOADER_MEMORYFUNCTIONS_H_

#include <vector>
#include <functional>

namespace MemLoader
{
    typedef unsigned int dword;
    typedef std::vector<dword> dwords;

    void std_copy(const dwords& source, std::vector<unsigned int>& destination, size_t offset);
    void manual_copy(const dwords& source, dwords& destination, size_t offset);
    void mech_copy(const dwords& source, dwords& destination, size_t offset);

    bool std_equals(const dwords& pattern, const dwords& tileset, size_t offset);
    bool manual_equals(const dwords& pattern, const dwords& tileset, size_t offset);
    bool mech_equals(const dwords& pattern, const dwords& tileset, size_t offset);

    void pad(
        const dwords& source,
        dwords& destination,
        const std::function<void(const dwords&, dwords&, size_t)>& copy_function);

    bool accumulate(
        const dwords& pattern,
        const dwords& tileset,
        const std::function<bool(const dwords&, const dwords&, size_t)>& equals_function);
}

#endif // MEMLOADER_MEMORYFUNCTIONS_H_
