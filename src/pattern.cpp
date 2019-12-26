#include <algorithm>

#include "pattern.h"

namespace BitPatterns
{
    using MemLoader::byte;
    using MemLoader::dword;
    using MemLoader::dwords;
    using std::fill;

    void count_pattern::calculate_frame()
    {
        byte b = 0;
        for (auto &element : pattern_)
        {
            dword value = 0;
            for (int i = 0; i < 4; i++)
            {
                value = (value << 8) | (b + i);
            }

            element = value;

            if (b != 0xFC)
            {
                b += 4;
            }
            else
            {
                b = 0;
            }
        }
    }

    bool count_pattern::is_valid_frame(const MemLoader::dwords &pattern)
    {
        byte b = 0;
        for (auto element : pattern)
        {
            dword value = 0;
            for (int i = 0; i < 4; i++)
            {
                value = (value << 8) | (b + i);
            }

            if (element != value)
            {
                return false;
            }

            if (b != 0xFC)
            {
                b += 4;
            }
            else
            {
                b = 0;
            }
        }

        return true;
    }

    void advancing_ones::calculate_frame()
    {
        const dword element = 1 << current_frame_;
        fill(pattern_.begin(), pattern_.end(), element);
    }

    bool advancing_ones::is_valid_frame(const MemLoader::dwords &pattern)
    {
        const dword hay = 1 << current_frame_;
        for (auto element : pattern)
        {
            if (element != hay)
            {
                return false;
            }
        }

        return true;
    }

    void advancing_zeroes::calculate_frame()
    {
        const dword element = ~(1 << current_frame_);
        fill(pattern_.begin(), pattern_.end(), element);
    }

    bool advancing_zeroes::is_valid_frame(const MemLoader::dwords &pattern)
    {
        const dword hay = ~(1 << current_frame_);
        for (auto element : pattern_)
        {
            if (element != hay)
            {
                return false;
            }
        }

        return true;
    }

    void inversions::calculate_frame()
    {
        const dword element = current_frame_ % 2 == 0 ? 0 : ~0;
        fill(pattern_.begin(), pattern_.end(), element);
    }

    bool inversions::is_valid_frame(const MemLoader::dwords &pattern)
    {
        const dword hay = current_frame_ % 2 == 0 ? 0 : ~0;
        for (auto element : pattern_)
        {
            if (element != hay)
            {
                return false;
            }
        }

        return true;
    }



}
