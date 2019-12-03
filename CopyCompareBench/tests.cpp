#include <vector>
#include <gtest/gtest.h>

#include "MemoryFunctions.h"

namespace {
    using namespace std;
    using namespace testing;
    using namespace MemLoader;

    typedef void (*CopyFunction)(const dwords&, dwords&, size_t);

    class CopyFunctionsTest : public TestWithParam<CopyFunction>
    {
    };

    TEST_P(CopyFunctionsTest, SimpleMultiples)
    {
        const dwords source = { 1, 2, 3 };
        dwords destination(9);

        (*GetParam())(source, destination, 3);

        ASSERT_EQ(destination, dwords({ 0, 0, 0, 1, 2, 3, 0, 0, 0 }));
    }

    TEST_P(CopyFunctionsTest, BiggerSource)
    {
        const dwords source = { 1, 2, 3, 4, 5 };
        dwords destination(3);

        (*GetParam())(source, destination, 0);

        ASSERT_EQ(destination, dwords({ 1, 2, 3 }));
    }

    TEST_P(CopyFunctionsTest, WithOverflow)
    {
        const dwords source = { 1, 2, 3 };
        dwords destination(9);

        (*GetParam())(source, destination, 7);

        ASSERT_EQ(destination, dwords({ 0, 0, 0, 0, 0, 0, 0, 1, 2 }));
    }

    TEST_P(CopyFunctionsTest, AfterEnd)
    {
        const dwords source = { 1, 2, 3 };
        dwords destination(1);

        (*GetParam())(source, destination, 1);

        ASSERT_EQ(destination, dwords({ 0 }));

        (*GetParam())(source, destination, 10);

        ASSERT_EQ(destination, dwords({ 0 }));
    }

    TEST_P(CopyFunctionsTest, LargerSource)
    {
        const dwords source = { 1, 2, 3, 4, 5, 6 };
        dwords destination(5);

        (*GetParam())(source, destination, 0);

        ASSERT_EQ(destination, dwords({ 1, 2, 3, 4, 5 }));
    }

    INSTANTIATE_TEST_SUITE_P(CopiesOnly, CopyFunctionsTest, Values(
        std_copy,
        manual_copy,
        mech_copy));

    typedef bool (*EqualityFunction)(const dwords&, const dwords&, size_t);

    class EqualityFunctionsTest : public TestWithParam<EqualityFunction>
    {
    };

    TEST_P(EqualityFunctionsTest, SameLengthCheck)
    {
        const dwords pattern = { 1, 2, 3 };
        const dwords tileset = { 1, 2, 3 };

        ASSERT_TRUE((*GetParam())(pattern, tileset, 0));
    }

    TEST_P(EqualityFunctionsTest, OffsetCheck)
    {
        const dwords pattern = { 1, 2, 3 };
        const dwords tileset = { 0, 1, 2, 3, 4 };

        ASSERT_TRUE((*GetParam())(pattern, tileset, 1));
        ASSERT_FALSE((*GetParam())(pattern, tileset, 0));
    }

    TEST_P(EqualityFunctionsTest, OffsetOverflowCheck)
    {
        const dwords pattern = { 1, 2, 3 };
        const dwords tileset = { 0, 1, 2 };

        ASSERT_TRUE((*GetParam())(pattern, tileset, 1));
        ASSERT_FALSE((*GetParam())(pattern, tileset, 2));

        ASSERT_TRUE((*GetParam())(pattern, tileset, 3));
        ASSERT_TRUE((*GetParam())(pattern, tileset, 10));
    }

    INSTANTIATE_TEST_SUITE_P(EqualityOnly, EqualityFunctionsTest, Values(
        std_equals,
        manual_equals,
        mech_equals));
}
