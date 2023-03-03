#ifndef CONFIG_HH
#define CONFIG_HH

#include <QtGlobal>

#include <cstdint>
#include <type_traits>
#include <limits>
#include <chrono>

namespace config
{
    // simulation world size
    typedef uint_fast32_t coord_t;
    const coord_t width =  200;
    const coord_t height = 200;
    // graphics scale factor
    const qreal scale = 2.5;

    // frequency of simulation and graphics updates
    using namespace std::chrono_literals;
    const auto world_tick = 200ms;
    const auto graphics_tick = 200ms;

    const auto board_size = width * height;
    static_assert( board_size < std::numeric_limits<uint_fast32_t>::max(),
                   "world too large" );
};
#endif // CONFIG_HH
