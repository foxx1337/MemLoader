#include <vector>
#include <gtest/gtest.h>

#include "MemoryFunctions.h"

namespace {
    using namespace std;
    using namespace testing;
    using namespace MemLoader;

    class CopyFunctionsTest : public TestWithParam<copy_method *>
    {
    };

    TEST_P(CopyFunctionsTest, SimpleMultiples)
    {
        const dwords source = { 1, 2, 3 };
        dwords destination(9);

        GetParam()->copy(source, destination.begin() + 3);

        ASSERT_EQ(destination, dwords({ 0, 0, 0, 1, 2, 3, 0, 0, 0 }));
    }

    TEST_P(CopyFunctionsTest, BiggerSource)
    {
        const dwords source = { 1, 2, 3, 4, 5 };
        dwords destination(3);

        GetParam()->copy_at_end(source, destination.begin(), destination.end());

        ASSERT_EQ(destination, dwords({ 1, 2, 3 }));
    }

    TEST_P(CopyFunctionsTest, WithOverflow)
    {
        const dwords source = { 1, 2, 3 };
        dwords destination(9);

        GetParam()->copy_at_end(source, destination.begin() + 7, destination.end());

        ASSERT_EQ(destination, dwords({ 0, 0, 0, 0, 0, 0, 0, 1, 2 }));
    }

    TEST_P(CopyFunctionsTest, AtEnd)
    {
        const dwords source = { 1, 2, 3 };
        dwords destination(1);

        GetParam()->copy_at_end(source, destination.begin(), destination.end());

        ASSERT_EQ(destination, dwords({ 1 }));
    }

    TEST_P(CopyFunctionsTest, LargerSource)
    {
        const dwords source = { 1, 2, 3, 4, 5, 6 };
        dwords destination(5);

        GetParam()->copy_at_end(source, destination.begin(), destination.end());

        ASSERT_EQ(destination, dwords({ 1, 2, 3, 4, 5 }));
    }

    INSTANTIATE_TEST_SUITE_P(CopiesOnly, CopyFunctionsTest, Values(
        &std_copy::instance(),
        &manual_copy::instance(),
        &mech_copy::instance()));

    class EqualityFunctionsTest : public TestWithParam<match_method *>
    {
    };

    TEST_P(EqualityFunctionsTest, SameLengthCheck)
    {
        const dwords pattern = { 1, 2, 3 };
        const dwords tileset = { 1, 2, 3 };

        ASSERT_TRUE(GetParam()->matches(pattern, tileset.begin()));
        ASSERT_TRUE(GetParam()->matches_at_end(pattern, tileset.begin(), tileset.end()));
    }

    TEST_P(EqualityFunctionsTest, OffsetCheck)
    {
        const dwords pattern = { 1, 2, 3 };
        const dwords tileset = { 0, 1, 2, 3, 4 };

        ASSERT_TRUE(GetParam()->matches(pattern, tileset.begin() + 1));
        ASSERT_FALSE(GetParam()->matches(pattern, tileset.begin()));
    }

    TEST_P(EqualityFunctionsTest, OffsetOverflowCheck)
    {
        const dwords pattern = { 1, 2, 3 };
        const dwords tileset = { 0, 1, 2 };

        ASSERT_TRUE(GetParam()->matches_at_end(pattern, tileset.begin() + 1, tileset.end()));
        ASSERT_FALSE(GetParam()->matches_at_end(pattern, tileset.begin() + 2, tileset.end()));
    }

    INSTANTIATE_TEST_SUITE_P(EqualityOnly, EqualityFunctionsTest, Values(
        &std_match::instance(),
        &manual_match::instance(),
        &mech_match::instance()));
}
