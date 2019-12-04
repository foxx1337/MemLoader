// CopyCompareBench.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "MemoryFunctions.h"
#include "TimedExecutor.h"

using namespace std;
using namespace MemLoader;

void generate_pattern(dwords& pattern)
{
    unsigned char b = 0;
    for (auto& element : pattern)
    {
        unsigned int value = 0;
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

void test_pads(const dwords& pattern, dwords& memory)
{
    cout << "1/3" << flush;
    const auto std_duration = timed_executor::run_timed<chrono::microseconds>([&pattern, &memory]()
        {
            pad(pattern, memory, std_copy);
        });

    cout << "\r2/3" << flush;
    const auto manual_duration = timed_executor::run_timed<chrono::microseconds>([&pattern, &memory]()
        {
            pad(pattern, memory, manual_copy);
        });

    cout << "\r3/3" << flush;
    const auto mech_duration = timed_executor::run_timed<chrono::microseconds>([&pattern, &memory]()
        {
            pad(pattern, memory, mech_copy);
        });

    cout << "\rMemory padding stats:" << endl;
    cout << "standard: " << std_duration.count() << " microseconds." << endl;
    cout << "manual: " << manual_duration.count() << " microseconds." << endl;
    cout << "mech: " << mech_duration.count() << " microseconds." << endl;
}

void test_accu(const dwords& pattern, const dwords& memory)
{
    cout << "1/3" << flush;
    const auto std_duration = timed_executor::run_timed<chrono::microseconds>([&pattern, &memory]()
        {
            const bool result = accu(pattern, memory, std_equals);

            if (!result)
            {
                cerr << "std_equals() -> false" << endl;
            }
        });

    cout << "\r2/3" << flush;
    const auto manual_duration = timed_executor::run_timed<chrono::microseconds>([&pattern, &memory]()
        {
            const bool result = accu(pattern, memory, manual_equals);

            if (!result)
            {
                cerr << "manual_equals() -> false" << endl;
            }
        });

    cout << "\r3/3" << flush;
    const auto mech_duration = timed_executor::run_timed<chrono::microseconds>([&pattern, &memory]()
        {
            const bool result = accu(pattern, memory, mech_equals);

            if (!result)
            {
                cerr << "mech_equals() -> false" << endl;
            }
        });

    cout << "\rMemory accumulated equality check  stats:" << endl;
    cout << "standard: " << std_duration.count() << " microseconds." << endl;
    cout << "manual: " << manual_duration.count() << " microseconds." << endl;
    cout << "mech: " << mech_duration.count() << " microseconds." << endl;
}

void dump_dwords(const dwords& vector)
{
    for (auto i : vector)
    {
        cout << i << ' ';
    }
    cout << endl;
}

int main()
{
    cout << thread::hardware_concurrency() << " threads supported." << endl;

    dwords memory(1024ull * 1024ull * 1024ull * 6ull);
    dwords pattern(1024 * 1024 * 1);

    generate_pattern(pattern);

    test_pads(pattern, memory);
    cout << endl;
    test_accu(pattern, memory);
}
