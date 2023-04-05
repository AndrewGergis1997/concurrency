#ifndef STOPWATCH_HH
#define STOPWATCH_HH

#include <chrono>
#include <mutex>

// used to time execution times
class stopwatch
{
public:
    // start the stopwatch (record object creation time)
    explicit stopwatch();

    // difference between now() and start time
    long long elapsed();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> starttime_;

    // mutex used to synchronize access to starttime_
    std::mutex mutex_;
};

#endif // STOPWATCH_HH
