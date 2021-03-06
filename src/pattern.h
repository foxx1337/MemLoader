#ifndef MEMLOADER_PATTERN_H_
#define MEMLOADER_PATTERN_H_

#include <string>
#include <utility>

#include "MemoryFunctions.h"

namespace BitPatterns
{
    class pattern
    {
    public:
        static const size_t default_size = 1024 * 1024;

        pattern(size_t frame_count, std::string name, size_t size = default_size)  :
            size_(size),
            total_frames_(frame_count),
            current_frame_(0),
            name_(std::move(name))
        {
            pattern_.resize(size_);
        }

        pattern(const pattern &old) = default;
        pattern(pattern &&old) noexcept = default;

        pattern &operator=(const pattern &) = delete;
        pattern &operator=(pattern &&) = delete;

        virtual ~pattern() = default;

        size_t size() const
        {
            return size_;
        }

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
        }

        void reset_frame()
        {
            current_frame_ = 0;
        }

        const MemLoader::dwords &get() const
        {
            return pattern_;
        }

        std::string name() const
        {
            return name_;
        }

        virtual void calculate_frame() = 0;

        virtual bool is_valid_frame(const MemLoader::dwords &pattern) = 0;

    protected:
        const size_t size_;
        const size_t total_frames_;
        size_t current_frame_;
        MemLoader::dwords pattern_;
        const std::string name_;
    };

    class count_pattern : public pattern
    {
    public:
        count_pattern() : pattern(1, "byte counter, four per dword") { }
        void calculate_frame() override;
        bool is_valid_frame(const MemLoader::dwords &pattern) override;
    };

    class advancing_ones : public pattern
    {
    public:
        advancing_ones() : pattern(32, "advancing ones") { }
        void calculate_frame() override;
        bool is_valid_frame(const MemLoader::dwords &pattern) override;
    };

    class advancing_zeroes : public pattern
    {
    public:
        advancing_zeroes() : pattern(32, "advancing zeroes") { }
        void calculate_frame() override;
        bool is_valid_frame(const MemLoader::dwords &pattern) override;
    };

    class inversions : public pattern
    {
    public:
        inversions() : pattern(2, "inversions: 000 -> 111") { }
        void calculate_frame() override;
        bool is_valid_frame(const MemLoader::dwords &pattern) override;
    };

    class checkerboard : public pattern
    {
    public:
        checkerboard() : pattern(2, "checkerboard") { }
        void calculate_frame() override;
        bool is_valid_frame(const MemLoader::dwords &pattern) override;

    private:
        static MemLoader::dword make_pattern();
    };
}
#endif // MEMLOADER_PATTERN_H_
