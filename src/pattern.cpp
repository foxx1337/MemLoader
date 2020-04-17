#include "pattern.h"

#include <algorithm>

namespace BitPatterns
{
    using MemLoader::byte;
    using MemLoader::dword;
    using MemLoader::dwords;
    using std::fill;

    bool all_equal(const dwords &memory, dword sample)
    {
        for (auto element : memory)
        {
            if (element != sample)
            {
                return false;
            }
        }
        return true;
    }

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
        const dword sample = 1 << current_frame_;
        fill(pattern_.begin(), pattern_.end(), sample);
    }

    bool advancing_ones::is_valid_frame(const MemLoader::dwords &pattern)
    {
        const dword sample = 1 << current_frame_;
        return all_equal(pattern, sample);
    }

    void advancing_zeroes::calculate_frame()
    {
        const dword sample = ~(1 << current_frame_);
        fill(pattern_.begin(), pattern_.end(), sample);
    }

    bool advancing_zeroes::is_valid_frame(const MemLoader::dwords &pattern)
    {
        const dword sample = ~(1 << current_frame_);
        return all_equal(pattern, sample);
    }

    void inversions::calculate_frame()
    {
        const dword sample = current_frame_ % 2 == 0 ? 0 : ~0;
        fill(pattern_.begin(), pattern_.end(), sample);
    }

    bool inversions::is_valid_frame(const MemLoader::dwords &pattern)
    {
        const dword sample = current_frame_ % 2 == 0 ? 0 : ~0;
        return all_equal(pattern, sample);
    }

    void checkerboard::calculate_frame()
    {
        const dword sample = make_pattern() << (current_frame_ % 2);
        fill(pattern_.begin(), pattern_.end(), sample);
    }

    bool checkerboard::is_valid_frame(const MemLoader::dwords &pattern)
    {
        const dword sample = make_pattern() << (current_frame_ % 2);
        return all_equal(pattern, sample);
    }

    dword checkerboard::make_pattern()
    {
        MemLoader::dword sample = 0;
        for (size_t i = 0; i < 32; i += 2)
        {
            sample = (sample << 2) | 1;
        }

        return sample;
    }
}
