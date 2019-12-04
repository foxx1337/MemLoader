#include "MemoryFunctions.h"

#include <algorithm>
#include <functional>

using std::function;
using std::min;

namespace MemLoader {
    dwords::const_iterator clamp_first_to_end(const dwords& first, const dwords& second, size_t offset)
    {
        const size_t to_seconds_end = second.size() > offset ? second.size() - offset : 0;

        return first.begin() + min(first.size(), to_seconds_end);
    }

    dwords::iterator clamp_to_end(dwords& values, size_t offset)
    {
        return values.begin() + min(values.size(), offset);
    }

    dwords::const_iterator clamp_to_end(const dwords& values, size_t offset)
    {
        return values.begin() + min(values.size(), offset);
    }

    void std_copy(const dwords& source, dwords& destination, size_t offset)
    {
        const auto source_end =
            clamp_first_to_end(source, destination, offset);

        const auto destination_offset =
            clamp_to_end(destination, offset);

        copy(source.begin(), source_end, destination_offset);
    }

    void manual_copy(const dwords& source, dwords& destination, size_t offset)
    {
        size_t position = offset;
        for (auto i = source.begin(); i != source.end() && position < destination.size(); ++i)
        {
            destination[position++] = *i;
        }
    }

    void mech_copy(const dwords& source, dwords& destination, size_t offset)
    {
        const size_t remaining_destination = destination.size() > offset
            ? destination.size() - offset
            : 0;

        memcpy(
            destination.data() + offset,
            source.data(),
            min(source.size(),remaining_destination) * sizeof(unsigned int));
    }

    bool std_equals(const dwords& pattern, const dwords& tileset, size_t offset)
    {
        const auto pattern_end =
            clamp_first_to_end(pattern, tileset, offset);

        const auto tileset_offset =
            clamp_to_end(tileset, offset);

        return equal(pattern.begin(), pattern_end, tileset_offset);
    }

    bool manual_equals(const dwords& pattern, const dwords& tileset, size_t offset)
    {
        size_t position = offset;
        for (auto i = pattern.begin(); i != pattern.end() && position < tileset.size(); ++i)
        {
            if (tileset[position++] != *i)
            {
                return false;
            }
        }

        return true;
    }

    bool mech_equals(const dwords& pattern, const dwords& tileset, size_t offset)
    {
        const size_t remaining_tileset = tileset.size() > offset
            ? tileset.size() - offset
            : 0;

        return memcmp(
            pattern.data(),
            tileset.data() + offset,
            min(pattern.size(), remaining_tileset) * sizeof(unsigned int)) == 0;
    }

    void pad(
        const dwords& source,
        dwords& destination,
        const function<void(const dwords&, dwords&, size_t)>& copy_function)
    {
        for (size_t i = 0; i + source.size() <= destination.size(); i += source.size())
        {
            copy_function(source, destination, i);
        }
    }

    bool accu(
        const dwords& pattern,
        const dwords& tileset,
        const function<bool(const dwords&, const dwords&, size_t)>& equals_function)
    {
        for (size_t i = 0; i + pattern.size() <= tileset.size(); i += pattern.size())
        {
            if (!equals_function(pattern, tileset, i))
            {
                return false;
            }
        }

        return true;
    }

}
