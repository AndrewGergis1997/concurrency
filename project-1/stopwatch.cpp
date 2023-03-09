#include "stopwatch.hh"

stopwatch::stopwatch()
{
    // save object creation time
    starttime_ = std::chrono::high_resolution_clock::now();
}

long long stopwatch::elapsed()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - starttime_).count();
}
