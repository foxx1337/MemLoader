#include "MemoryFunctions.h"

#include <algorithm>
#include <cstring>

using std::distance;
using std::min;
using std::memcpy;

namespace MemLoader
{
    dwords::const_iterator clamp_to_second_end(
        const dwords &first,
        dwords::const_iterator second_begin,
        dwords::const_iterator second_end)
    {
        const size_t second_capacity = second_end > second_begin ? second_end - second_begin : 0;

        return first.begin() + std::min(first.size(), second_capacity);
    }

    void copy_method::pad(
        const dwords &source,
        dwords::iterator destination_begin,
        dwords::iterator destination_end, const copy_method &copier)
    {
        auto destination = destination_begin;

        while (distance(destination ,destination_end) >= source.size())
        {
            copier.copy(source, destination);
            destination += source.size();
        }

        copier.copy_at_end(source, destination, destination_end);
    }

    std_copy &std_copy::instance()
    {
        static std_copy instance;

        return instance;
    }

    void std_copy::copy(const dwords &source, dwords::iterator destination_begin) const
    {
        std::copy(source.begin(), source.end(), destination_begin);
    }

    void std_copy::copy_at_end(const dwords &source, dwords::iterator destination_begin, dwords::iterator destination_end) const
    {
        const auto source_end =
            clamp_to_second_end(source, destination_begin, destination_end);

        std::copy(source.begin(), source_end, destination_begin);
    }

    manual_copy &manual_copy::instance()
    {
        static manual_copy instance;

        return instance;
    }

    void manual_copy::copy(const dwords &source, dwords::iterator destination_begin) const
    {
        auto destination = destination_begin;
        for (auto i : source)
        {
            *destination++ = i;
        }
    }

    void manual_copy::copy_at_end(const dwords &source, dwords::iterator destination_begin, dwords::iterator destination_end) const
    {
        auto destination = destination_begin;
        for (auto i = source.begin(); i != source.end() && destination < destination_end; ++i)
        {
            *destination++ = *i;
        }
    }

    mech_copy &mech_copy::instance()
    {
        static mech_copy instance;

        return instance;
    }

    void mech_copy::copy(const dwords &source, dwords::iterator destination_begin) const
    {
        memcpy(
            &(*destination_begin),
            source.data(),
            source.size() * sizeof(dword));
    }

    void mech_copy::copy_at_end(const dwords &source, dwords::iterator destination_begin, dwords::iterator destination_end) const
    {
        const size_t destination_remaining = destination_end > destination_begin
            ? destination_end - destination_begin
            : 0;
        memcpy(
            &(*destination_begin),
            source.data(),
            min(source.size(), destination_remaining) * sizeof(dword));
    }

    bool match_method::accu(
        const dwords &pattern,
        dwords::const_iterator tileset_begin,
        dwords::const_iterator tileset_end,
        const match_method &matcher)
    {
        auto tileset = tileset_begin;

        while (distance(tileset, tileset_end) >= pattern.size())
        {
            if (!matcher.matches(pattern, tileset))
            {
                return false;
            }

            tileset += pattern.size();
        }

        return matcher.matches_at_end(pattern, tileset, tileset_end);
    }


    std_match &std_match::instance()
    {
        static std_match instance;

        return instance;
    }

    bool std_match::matches(const dwords &pattern, dwords::const_iterator tileset_begin) const
    {
        return std::equal(pattern.begin(), pattern.end(), tileset_begin);
    }

    bool std_match::matches_at_end(const dwords &pattern, dwords::const_iterator tileset_begin, dwords::const_iterator tileset_end) const
    {
        const auto pattern_end =
            clamp_to_second_end(pattern, tileset_begin, tileset_end);

        return std::equal(pattern.begin(), pattern_end, tileset_begin);
    }

    manual_match &manual_match::instance()
    {
        static manual_match instance;

        return instance;
    }

    bool manual_match::matches(const dwords &pattern, dwords::const_iterator tileset_begin) const
    {
        auto element = tileset_begin;
        for (auto i : pattern)
        {
            if (i != * element++)
            {
                return false;
            }
        }

        return true;
    }

    bool manual_match::matches_at_end(const dwords &pattern, dwords::const_iterator tileset_begin, dwords::const_iterator tileset_end) const
    {
        auto element = tileset_begin;
        for (auto i = pattern.begin(); i != pattern.end() && element < tileset_end; i++)
        {
            if (*i != *element++)
            {
                return false;
            }
        }

        return true;
    }

    mech_match &mech_match::instance()
    {
        static mech_match instance;

        return instance;
    }

    bool mech_match::matches(const dwords &pattern, dwords::const_iterator tileset_begin) const
    {
        return 0 == memcmp(
            pattern.data(),
            &(*tileset_begin),
            pattern.size() * sizeof(dword));
    }

    bool mech_match::matches_at_end(const dwords &pattern, dwords::const_iterator tileset_begin, dwords::const_iterator tileset_end) const
    {
        const size_t tileset_remaining = tileset_end > tileset_begin
            ? tileset_end - tileset_begin
            : 0;

        return 0 == memcmp(
            pattern.data(),
            &(*tileset_begin),
            min(pattern.size(), tileset_remaining) * sizeof(dword));
    }
}
