#include "hot_spot.h"

#include "pass.h"

using std::ostream;

namespace MemLoader
{
    ostream &operator<<(ostream &out, const hot_spot &spot)
    {
        out << spot.thread_id << ": " << spot.pass.pattern->name() << " ||| iter "
            << spot.iteration + 1 << '/' << spot.pass.repetitions
            << " frame " << spot.frame + 1 << '/'
            << spot.pass.pattern->total_frames();

        return out;
    }
}
