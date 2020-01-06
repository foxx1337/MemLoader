#include <thread>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <mutex>

#include "pattern.h"
#include "hot_spot.h"
#include "pass.h"
#include "memory_helpers.h"

#include "MemoryFunctions.h"
#include "TimedExecutor.h"

using namespace std;
using namespace MemLoader;
using namespace BitPatterns;

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

    optional<hot_spot> execute(dwords::iterator memory_begin, dwords::iterator memory_end, size_t thread_id,
        progress_callback callback, continuator_callback can_continue) const
    {
        for (const auto &the_pass : passes_)
        {
            auto result = the_pass.execute(memory_begin, memory_end,
                thread_id, callback, can_continue);

            if (result.has_value())
            {
                return result;
            }
        }

        return optional<hot_spot>();
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
                auto result = plan.execute(start, stop, i, callback, 
                    [&m, &crashed]()
                    {
                        unique_lock<mutex> lock(m);
                        return !crashed;
                    });

                if (result.has_value())
                {
                    unique_lock<mutex> lock(m);
                    crashed = true;

                    if (!result->is_interrupted)
                    {
                        cerr << "Crashed in thread " << result.value() << endl;
                    }
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
    cout << spot << endl;
}

void no_log(const hot_spot &spot)
{
    
}

int main()
{
    cout << "Available memory: " << humanize_size(truncate(get_free_memory(), kb)) << endl;
    dwords memory(1024ull * 1024ull * 256ull);
    cout << "Available memory: " << humanize_size(truncate(get_free_memory(), kb)) << endl;

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
