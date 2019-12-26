#ifndef MEMLOADER_MEMORYFUNCTIONS_H_
#define MEMLOADER_MEMORYFUNCTIONS_H_

#include <vector>

namespace MemLoader
{
    typedef unsigned int dword;
    typedef std::vector<dword> dwords;

    class copy_method
    {
    public:
        virtual void copy(const dwords &source, dwords::iterator destination_begin) const = 0;
        virtual void copy_at_end(const dwords &source, dwords::iterator destination_begin, dwords::iterator destination_end) const = 0;

        static void pad(
            const dwords &source,
            dwords::iterator destination_begin,
            dwords::iterator destination_end,
            const copy_method &copier);
    };

    class std_copy : public copy_method
    {
    public:
        static std_copy &instance();

        void copy(const dwords &source, dwords::iterator destination_begin) const override;
        void copy_at_end(const dwords &source, dwords::iterator destination_begin, dwords::iterator destination_end) const override;

        std_copy(const std_copy &) = delete;
        std_copy &operator=(const std_copy &) = delete;

    private:
        std_copy() = default;
    };

    class manual_copy : public copy_method
    {
    public:
        static manual_copy &instance();

        void copy(const dwords &source, dwords::iterator destination_begin) const override;
        void copy_at_end(const dwords &source, dwords::iterator destination_begin, dwords::iterator destination_end) const override;

        manual_copy(const manual_copy &) = delete;
        manual_copy &operator=(const manual_copy &) = delete;

    private:
        manual_copy() = default;
    };

    class mech_copy : public copy_method
    {
    public:
        static mech_copy &instance();

        void copy(const dwords &source, dwords::iterator destination_begin) const override;
        void copy_at_end(const dwords &source, dwords::iterator destination_begin, dwords::iterator destination_end) const override;

        mech_copy(const mech_copy &) = delete;
        mech_copy &operator=(const mech_copy &) = delete;

    private:
        mech_copy() = default;
    };

    class match_method
    {
    public:
        virtual bool matches(const dwords &pattern, dwords::const_iterator tileset_begin) const = 0;
        virtual bool matches_at_end(const dwords &pattern, dwords::const_iterator tileset_begin, dwords::const_iterator tileset_end) const = 0;

        static bool accu(
            const dwords &pattern,
            dwords::const_iterator tileset_begin,
            dwords::const_iterator tileset_end,
            const match_method &matcher);
    };

    class std_match : public match_method
    {
    public:
        static std_match &instance();

        bool matches(const dwords &pattern, dwords::const_iterator tileset_begin) const override;
        bool matches_at_end(const dwords &pattern, dwords::const_iterator tileset_begin, dwords::const_iterator tileset_end) const override;

        std_match(const std_match &) = delete;
        std_match &operator=(const std_match &) = delete;

    private:
        std_match() = default;
    };

    class manual_match : public match_method
    {
    public:
        static manual_match &instance();

        bool matches(const dwords &pattern, dwords::const_iterator tileset_begin) const override;
        bool matches_at_end(const dwords &pattern, dwords::const_iterator tileset_begin, dwords::const_iterator tileset_end) const override;

        manual_match(const manual_match &) = delete;
        manual_match &operator=(const manual_match &) = delete;

    private:
        manual_match() = default;
    };

    class mech_match : public match_method
    {
    public:
        static mech_match &instance();

        bool matches(const dwords &pattern, dwords::const_iterator tileset_begin) const override;
        bool matches_at_end(const dwords &pattern, dwords::const_iterator tileset_begin, dwords::const_iterator tileset_end) const override;

        mech_match(const mech_match &) = delete;
        mech_match &operator=(const mech_match &) = delete;

    private:
        mech_match() = default;
    };
}

#endif // MEMLOADER_MEMORYFUNCTIONS_H_
