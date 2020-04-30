#include "pass.h"

using std::optional;

namespace MemLoader
{
    optional<hot_spot> pass::execute_one(dwords::iterator memory_begin, dwords::iterator memory_end,
        size_t iteration, size_t thread_id, progress_callback update_progress,
        continuator_callback can_continue) const
    {
        pattern->reset_frame();
        while (pattern->current_frame() < pattern->total_frames())
        {
            pattern->calculate_frame();
            dwords frame = pattern->get();

            const hot_spot here{ *this, iteration, pattern->current_frame(), thread_id, !can_continue() };

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

            update_progress(here);

            pattern->advance_current_frame();
        }
        return optional<hot_spot>();
    }

    optional<hot_spot> pass::execute(dwords::iterator memory_begin, dwords::iterator memory_end,
        size_t thread_id, progress_callback update_progress,
        continuator_callback can_continue) const
    {
        for (size_t repetition = 0; repetition < repetitions; ++repetition)
        {
            auto result = execute_one(memory_begin, memory_end,
                repetition, thread_id, update_progress, can_continue);

            if (result.has_value())
            {
                return result;
            }
        }

        return optional<hot_spot>();
    }
}
