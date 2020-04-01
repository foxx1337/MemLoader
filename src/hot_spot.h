#ifndef HOT_SPOT_H_
#define HOT_SPOT_H_

#include <ostream>

namespace MemLoader
{
    struct pass;

    struct hot_spot
    {
        const struct pass &pass;
        size_t iteration;
        size_t frame;
        size_t thread_id;
        bool is_interrupted;

        friend std::ostream &operator<<(std::ostream &out, const hot_spot &spot);
    };
}

#endif // HOT_SPOT_H_
