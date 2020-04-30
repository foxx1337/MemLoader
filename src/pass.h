#ifndef PASS_H_
#define PASS_H_

#include <optional>
#include <functional>
#include <memory>

#include "pattern.h"
#include "hot_spot.h"

namespace MemLoader
{
    typedef std::function<bool()> continuator_callback;
    typedef std::function<void(const hot_spot &)> progress_callback;

    struct pass
    {
        std::unique_ptr<BitPatterns::pattern> pattern;
        size_t repetitions;

        std::optional<hot_spot> execute(dwords::iterator memory_begin, dwords::iterator memory_end,
            size_t thread_id, progress_callback update_progress,
            continuator_callback can_continue) const;

    private:
        std::optional<hot_spot> execute_one(dwords::iterator memory_begin, dwords::iterator memory_end,
            size_t iteration, size_t thread_id, progress_callback update_progress,
            continuator_callback can_continue) const;
    };
}

#endif // PASS_H_
