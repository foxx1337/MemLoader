#ifndef MEMLOADER_PATTERN_H_
#define MEMLOADER_PATTERN_H_

#include <string>

#include "MemoryFunctions.h"

namespace BitPatterns
{
    template <size_t frame_count> class pattern
    {
    public:
        pattern(size_t size = 256) :
            size_(size),
            total_frames_(frame_count),
            current_frame_(0)
        {
            pattern_.resize(size_);
        }

        pattern(const pattern &) = delete;
        pattern(pattern &&) = delete;
        pattern &operator=(const pattern &) = delete;
        pattern &operator=(pattern &&) = delete;

        virtual ~pattern() = 0;

        size_t total_frames() const
        {
            return total_frames_;
        }

        size_t current_frame() const
        {
            return current_frame_;
        }

        void advance_current_frame()
        {
            ++current_frame_;

            if (current_frame_ == total_frames_)
            {
                current_frame_ = 0;
            }
        }

        virtual std::string name() = 0;

        virtual void calculate_frame() = 0;

        virtual bool is_valid_frame(const MemLoader::dwords &pattern) = 0;

    protected:
        const size_t size_;
        const size_t total_frames_;
        size_t current_frame_;
        MemLoader::dwords pattern_;
    };

    class count_pattern : public pattern<1>
    {
        std::string name() override { return "byte counter, four per dword"; }
        void calculate_frame() override;
        bool is_valid_frame(const MemLoader::dwords &pattern) override;
    };

    class advancing_ones : public pattern<32>
    {
        std::string name() override { return "advancing ones"; }
        void calculate_frame() override;
        bool is_valid_frame(const MemLoader::dwords &pattern) override;
    };

    class advancing_zeroes : public pattern<32>
    {
        std::string name() override { return "advancing zeroes"; }
        void calculate_frame() override;
        bool is_valid_frame(const MemLoader::dwords &pattern) override;
    };

    class inversions : public pattern<2>
    {
        std::string name() override { return "inversions: 000 -> 111"; };
        void calculate_frame() override;
        bool is_valid_frame(const MemLoader::dwords &pattern) override;
    };
}
#endif // MEMLOADER_PATTERN_H_
