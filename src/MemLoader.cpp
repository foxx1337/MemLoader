#include <memory>
#include <vector>
#include <thread>
#include <iostream>

#include "MemoryFunctions.h"
#include "pattern.h"

using namespace std;
using namespace MemLoader;
using namespace BitPatterns;

struct pass;

struct hot_spot
{
    const pass &pass;
    size_t iteration;
    size_t frame;

    bool is_success() const;
};

typedef void (*progress_callback)(const hot_spot &spot);

struct pass
{
    unique_ptr<pattern> pattern;
    size_t repetitions;

    static const pass none;

    hot_spot execute_one(dwords::iterator memory_begin, dwords::iterator memory_end, size_t iteration, progress_callback callback) const
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

            const hot_spot here{ *this, iteration ,pattern->current_frame() };

            if (has_crashed)
            {
                return here;
            }

            callback(here);

            pattern->advance_current_frame();
        }
        return { pass::none, 0, 0 };
    }

    hot_spot execute(dwords::iterator memory_begin, dwords::iterator memory_end, progress_callback callback) const
    {
        for (size_t repetition = 0; repetition < repetitions; ++repetition)
        {
            const hot_spot result = execute_one(memory_begin, memory_end, repetition, callback);

            if (!result.is_success())
            {
                return result;
            }
        }

        return { pass::none, 0, 0 };
    }
};

const pass pass::none = { unique_ptr<BitPatterns::pattern>(), 0 };

bool hot_spot::is_success() const
{
    return &(hot_spot::pass) == &pass::none;
}

class execution_plan
{
public:
    execution_plan()
    {
        passes_.push_back({ unique_ptr<pattern>(new count_pattern), 1 });
        passes_.push_back({ unique_ptr<pattern>(new advancing_ones), 2 });
        passes_.push_back({ unique_ptr<pattern>(new advancing_zeroes), 2 });
        passes_.push_back({ unique_ptr<pattern>(new inversions), 4 });
        passes_.push_back({ unique_ptr<pattern>(new checkerboard), 4 });
    }

    const vector<pass> &passes() const
    {
        return passes_;
    }

    hot_spot execute(dwords::iterator memory_begin, dwords::iterator memory_end, progress_callback callback) const
    {
        for (const auto &the_pass : passes_)
        {
            const hot_spot result = the_pass.execute(memory_begin, memory_end, callback);

            if (!result.is_success())
            {
                return result;
            }
        }

        return { pass::none, 0, 0 };
    }

private:
    vector<pass> passes_;
};



int main()
{
    const execution_plan plan;
    dwords memory(1024ull * 1024ull * 1024ull);

    hot_spot result = plan.execute(memory.begin(), memory.end(),
        [](const hot_spot &spot)
        {
            cout << spot.pass.pattern->name() << " ||| iter "
                << spot.iteration + 1 << '/' << spot.pass.repetitions
                << " frame " << spot.frame + 1 << '/'
                << spot.pass.pattern->total_frames() << endl;
        });

    if (!result.is_success())
    {
        cerr << "Failure in " << result.pass.pattern->name() << " ||| iter "
            << result.iteration + 1 << '/' << result.pass.repetitions
            << " frame " << result.frame + 1 << ' '
            << result.pass.pattern->total_frames() << endl;
    }
    
    return 0;
}
