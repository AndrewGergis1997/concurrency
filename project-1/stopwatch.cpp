#include "stopwatch.hh"

stopwatch::stopwatch()
{
    // save object creation time
    starttime_ = std::chrono::high_resolution_clock::now();
}

long long stopwatch::elapsed()
{
    // lock_guard used to automatically lock and unlock mutex
   // std::lock_guard<std::mutex> lock(mutex_);
    return std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - starttime_).count();
}
