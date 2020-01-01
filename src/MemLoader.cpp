#include <memory>
#include <vector>
#include <thread>
#include <iostream>

#include "MemoryFunctions.h"
#include "pattern.h"

using namespace std;
using namespace MemLoader;
using namespace BitPatterns;

struct run
{
    unique_ptr<pattern> pattern;
    size_t repetitions;

    static const run none;

    bool execute(dwords::iterator memory_begin, dwords::iterator memory_end) const
    {
        pattern->reset_frame();
        while (pattern->current_frame() < pattern->total_frames())
        {
            pattern->calculate_frame();
            dwords frame = pattern->get();

            copy_method::pad(frame, memory_begin, memory_end, std_copy::instance());

            const bool has_crashed =
                !pattern->is_valid_frame(frame)
                || !match_method::accu(frame, memory_begin, memory_end, std_match::instance());

            if (has_crashed)
            {
                return false;
            }
            
            pattern->advance_current_frame();
        }

        return true;
    }
};

const run run::none = { unique_ptr<BitPatterns::pattern>(), 0 };

struct hot_spot
{
    const run &the_run;
    size_t the_iteration;

    bool is_success() const
    {
        return &the_run == &run::none;
    }
};

class execution_plan
{
public:
    execution_plan()
    {
        runs_.push_back({ unique_ptr<pattern>(new count_pattern), 1 });
        runs_.push_back({ unique_ptr<pattern>(new advancing_ones), 2 });
        runs_.push_back({ unique_ptr<pattern>(new advancing_zeroes), 2 });
        runs_.push_back({ unique_ptr<pattern>(new inversions), 4 });
        runs_.push_back({ unique_ptr<pattern>(new checkerboard), 4 });
    }

    const vector<run> &runs() const
    {
        return runs_;
    }

    typedef void (*log_intermediate_stage)(const hot_spot &spot);

    hot_spot execute(dwords::iterator memory_begin, dwords::iterator memory_end, log_intermediate_stage callback) const
    {
        for (const auto &the_run : runs_)
        {
            for (size_t r = 0; r < the_run.repetitions; ++r)
            {
                const bool has_crashed = !the_run.execute(memory_begin, memory_end);

                if (has_crashed)
                {
                    return { the_run, r };
                }

                if (callback != nullptr)
                {
                    callback({ the_run, r });
                }
            }
        }
        return { run::none, 0 };
    }

private:
    vector<run> runs_;
};



int main()
{
    const execution_plan plan;
    dwords memory(1024ull * 1024ull * 1024ull);

    hot_spot result = plan.execute(memory.begin(), memory.end(),
        [](const hot_spot &spot)
        {
            cout << spot.the_run.pattern->name() << " ||| "
                << spot.the_iteration + 1 << '/' << spot.the_run.repetitions << endl;
        });

    if (!result.is_success())
    {
        cerr << "Failure in " << result.the_run.pattern->name() << " ||| "
            << result.the_iteration + 1 << '/' << result.the_run.repetitions << endl;
    }
    
    return 0;
}
