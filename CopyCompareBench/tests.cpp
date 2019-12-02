#include <vector>
#include <gtest/gtest.h>

#include "MemoryFunctions.h"

namespace {
    using namespace std;
    using namespace testing;
    using namespace MemLoader;

    typedef void (*CopyFunction)(const vector<unsigned int>&, vector<unsigned int>&, size_t);

    class CopyFunctionsTest : public TestWithParam<CopyFunction>
    {
    };

    TEST_P(CopyFunctionsTest, SimpleMultiples)
    {
        const vector<unsigned int> source = { 1, 2, 3 };
        vector<unsigned int> destination(9);

        (*GetParam())(source, destination, 3);

        ASSERT_EQ(destination, vector<unsigned int>({ 0, 0, 0, 1, 2, 3, 0, 0, 0 }));
    }

    TEST_P(CopyFunctionsTest, WithOverflow)
    {
        const vector<unsigned int> source = { 1, 2, 3 };
        vector<unsigned int> destination(9);

        (*GetParam())(source, destination, 7);

        ASSERT_EQ(destination, vector<unsigned int>({ 0, 0, 0, 0, 0, 0, 0, 1, 2 }));
    }

    INSTANTIATE_TEST_SUITE_P(CopiesOnly, CopyFunctionsTest, Values(
        std_copy,
        manual_copy,
        mech_copy));
}
