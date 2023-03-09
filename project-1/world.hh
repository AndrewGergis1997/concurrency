#ifndef WORLD_HH
#define WORLD_HH

#include <memory>
#include <array>
#include "config.hh"

namespace world
{
    enum class Block { empty, occupied };

    typedef std::array< Block, config::width * config::height > world_t;

    // use two arrays:
    // 'current' world state
    // 'next' where changes are made
    // these are swapped for the next world iteration
    extern std::unique_ptr< world_t > current;
    extern std::unique_ptr< world_t > next;

    // is world simulation running or should it stop?
    extern bool running;

    void init(void);
    void next_generation(void);

    // --------------------------------------------------------------------------------------------
    // various helper routines to process (x,y) coordinates with one flat std::array datastructure:

    inline config::coord_t xy2array( config::coord_t x, config::coord_t y)
    {
        Q_ASSERT( ( x <= config::width ) && ( y <= config::height ) );
        config::coord_t pos = (y * config::width) + x;
        Q_ASSERT( pos <= config::board_size );
        return pos;
    }

    inline config::coord_t W(config::coord_t x, config::coord_t y) { return xy2array( ((x-1)%config::width), y); }
    inline config::coord_t E(config::coord_t x, config::coord_t y) { return xy2array( ((x+1)%config::width), y ); }
    inline config::coord_t N(config::coord_t x, config::coord_t y) { return xy2array( x, ((y-1)%config::height) ); }
    inline config::coord_t S(config::coord_t x, config::coord_t y) { return xy2array( x, ((y+1)%config::height) ); }

    inline config::coord_t NE(config::coord_t x, config::coord_t y) { return xy2array( ((x+1)%config::width), ((y-1)%config::height) ); }
    inline config::coord_t SE(config::coord_t x, config::coord_t y) { return xy2array( ((x+1)%config::width), ((y+1)%config::height) ); }
    inline config::coord_t SW(config::coord_t x, config::coord_t y) { return xy2array( ((x-1)%config::width), ((y+1)%config::height) ); }
    inline config::coord_t NW(config::coord_t x, config::coord_t y) { return xy2array( ((x-1)%config::width), ((y-1)%config::height) ); }

    inline std::array<config::coord_t,8> neighbourhood( config::coord_t x, config::coord_t y)
    {
        return { N(x,y), NE(x,y), E(x,y), SE(x,y), S(x,y), SW(x,y), W(x,y), NW(x,y) };
    }

    inline unsigned int num_neighbours( config::coord_t x, config::coord_t y)
    {
        unsigned int total_neighbours = 0;
        for( config::coord_t& position : neighbourhood(x,y) ) {
            if( (*world::current)[ position ] == Block::occupied )
                total_neighbours++;
        }
        return total_neighbours;
    }

};
#endif // WORLD_HH
