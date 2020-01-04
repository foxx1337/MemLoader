#include <memory>
#include <vector>
#include <thread>
#include <functional>
#include <chrono>
#include <iostream>

#include "MemoryFunctions.h"
#include "TimedExecutor.h"
#include "pattern.h"
#include <algorithm>
#include <mutex>

using namespace std;
using namespace MemLoader;
using namespace BitPatterns;

struct pass;

struct hot_spot
{
    const pass &pass;
    size_t iteration;
    size_t frame;
    size_t thread_id;
    bool is_interrupted;

    bool is_success() const;
};

typedef function<bool()> continuator_callback;
typedef function<void(const hot_spot &)> progress_callback;

struct pass
{
    unique_ptr<pattern> pattern;
    size_t repetitions;

    static const pass none;

    hot_spot execute_one(dwords::iterator memory_begin, dwords::iterator memory_end,
        size_t iteration, size_t thread_id, progress_callback callback,
        continuator_callback can_continue) const
    {
        pattern->reset_frame();
        while (pattern->current_frame() < pattern->total_frames())
        {
            pattern->calculate_frame();
            dwords frame = pattern->get();

            const hot_spot here{ *this, iteration ,pattern->current_frame(), thread_id, !can_continue() };

            if (here.is_interrupted)
            {
                return here;
            }

            copy_method::pad(frame, memory_begin, memory_end, std_copy::instance());

            const bool has_crashed =
                !pattern->is_valid_frame(frame)
                || !match_method::accu(frame, memory_begin, memory_end, std_match::instance());

            if (has_crashed)
            {
                return here;
            }

            callback(here);

            pattern->advance_current_frame();
        }
        return { pass::none, 0, 0 };
    }

    hot_spot execute(dwords::iterator memory_begin, dwords::iterator memory_end,
        size_t thread_id, progress_callback callback,
        continuator_callback can_continue) const
    {
        for (size_t repetition = 0; repetition < repetitions; ++repetition)
        {
            const hot_spot result = execute_one(memory_begin, memory_end,
                repetition, thread_id, callback, can_continue);

            if (!result.is_success())
            {
                return result;
            }
        }

        return { pass::none, 0, 0, thread_id, false };
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

    hot_spot execute(dwords::iterator memory_begin, dwords::iterator memory_end, size_t thread_id,
        progress_callback callback, continuator_callback can_continue) const
    {
        for (const auto &the_pass : passes_)
        {
            const hot_spot result = the_pass.execute(memory_begin, memory_end,
                thread_id, callback, can_continue);

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

void memload_threaded(size_t block_size, size_t num_threads,
    dwords &memory, progress_callback callback)
{
    const size_t num_pieces = memory.size() / block_size
        + (memory.size() % block_size == 0 ? 0 : 1);

    const size_t num_pieces_per_thread = num_pieces / num_threads;
    const size_t cutoff = num_pieces - num_pieces_per_thread * num_threads;

    vector<thread> workers;
    dwords::iterator start = memory.begin();

    mutex m;
    bool crashed = false;
    for (size_t i = 0; i < num_threads; ++i)
    {
        const dwords::iterator stop = min(
            start + (num_pieces_per_thread + (i < cutoff ? 1 : 0)) * block_size,
            memory.end());
        workers.emplace_back(thread(
            [&m, &crashed, start, stop, i, callback]()
            {
                execution_plan plan;
                hot_spot here = plan.execute(start, stop, i, callback, 
                    [&m, &crashed]()
                    {
                        unique_lock<mutex> lock(m);
                        return !crashed;
                    });

                if (!here.is_success())
                {
                    unique_lock<mutex> lock(m);
                    crashed = true;
                }
            }));

        start = stop;
    }

    for (auto &worker : workers)
    {
        worker.join();
    }
}

void log_progress(const hot_spot &spot)
{
    
    cout << spot.thread_id << ": " << spot.pass.pattern->name() << " ||| iter "
        << spot.iteration + 1 << '/' << spot.pass.repetitions
        << " frame " << spot.frame + 1 << '/'
        << spot.pass.pattern->total_frames() << endl;
}

void no_log(const hot_spot &spot)
{
    
}

int main()
{
    dwords memory(1024ull * 1024ull * 1024ull);

    for (size_t i = 1; i <= 24; i++)
    {
        auto duration = timed_executor::run_timed<chrono::seconds>(
            [&memory, i]()
            {
                memload_threaded(pattern::default_size, i, memory, no_log);
            });

        cout << i << '\t' << duration.count() << " seconds" << endl;
    }

    return 0;
}
