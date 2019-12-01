// CopyCompareBench.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <chrono>

#include "MemoryFunctions.h"
#include "TimedExecutor.h"
#include <thread>

using namespace std;
using namespace MemLoader;

void dump_vector(const vector<unsigned int> &vector)
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

    vector<unsigned int> memory(1024ull * 1024ull * 1024ull * 1ull);
    vector<unsigned int> pattern(1024 * 1024 * 1);

    unsigned char b = 0;
    for (auto &element : pattern)
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

    const auto std_duration = timed_executor::run_timed<chrono::microseconds>([&pattern, &memory]()
        {
            pad(pattern, memory, std_copy);
        });

    const auto manual_duration = timed_executor::run_timed<chrono::microseconds>([&pattern, &memory]()
        {
            pad(pattern, memory, manual_copy);
        });

    const auto mech_duration = timed_executor::run_timed<chrono::microseconds>([&pattern, &memory]()
        {
            pad(pattern, memory, mech_copy);
        });

    cout << "standard: " << std_duration.count() << " microseconds." << endl;
    cout << "manual: " << manual_duration.count() << " microseconds." << endl;
    cout << "mech: " << mech_duration.count() << " microseconds." << endl;
}
