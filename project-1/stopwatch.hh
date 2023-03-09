#ifndef STOPWATCH_HH
#define STOPWATCH_HH

#include <chrono>

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
};

#endif // STOPWATCH_HH
