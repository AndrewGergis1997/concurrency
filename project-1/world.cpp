#include "world.hh"
#include "stopwatch.hh"

#include "tuni.hh"
TUNI_WARN_OFF()
#include <QtGlobal>
#include <QDebug>
TUNI_WARN_ON()

#include <random>
#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <barrier>

#define THREADS 4

namespace world {

  // actual variables (extern in header file):
  std::unique_ptr< world_t > current;
  std::unique_ptr< world_t > next;

  bool running = true;
  std::mutex running_mutex;  // Mutex to protect the 'running' variable

  namespace {
    // QtCreator might give non-pod warning here, explanation:
    // https://github.com/KDE/clazy/blob/master/docs/checks/README-non-pod-global-static.md
    auto rand_generator = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());

    // glider data is from: https://raw.githubusercontent.com/EsriCanada/gosper_glider_gun/master/gosper.txt
    std::string glider_gun[] = {
      "0000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 1 0 1 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 1 0 0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 1 1 0 1 0 0 1 0 0 1 1 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 1 0 0 0 1 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 1 0 1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 1 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 1 1 1 1 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
      "0000 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0",
    };
  }

  void init(void)
  {
    // creaate and fill empty world data
    current = std::make_unique<world_t>();
    current->fill(Block::empty);
    next = std::make_unique<world_t>();
    next->fill(Block::empty);

    // create a random starting world
    config::coord_t x = 0;
    config::coord_t y = 0;
    for( x = 0; x < config::width; ++x )
      {
        for( y = 0; y < config::height; ++y )
          {
            if( x < (config::width/2) and y < (config::height/2) )
              continue; // leave space for the glider gun

            bool b = rand_generator();
            if(b) {
                (*current)[xy2array(x,y)] = Block::empty;
              } else {
                (*current)[xy2array(x,y)] = Block::occupied;
              }

          }
      }


    // glider gun data to world data
    const auto glider_pos = 20;
    x = glider_pos;
    y = glider_pos;
    for(auto& line : glider_gun)
      {
        for(char c : line)
          {
            if(c == '1') {
                (*current)[xy2array(x,y)] = Block::occupied;
                ++x;
              } else if(c == '0') {
                (*current)[xy2array(x,y)] = Block::empty;
                ++x;
              } else {
                // all other chars ignored
              }
          }
        x = glider_pos;
        ++y;
      }
  }

  void next_generation()
  {
    stopwatch clock;

    // Get the number of rows and columns in the world
    config::coord_t rows = config::height;
    config::coord_t cols = config::width;

    // Create a barrier to synchronize the threads after each generation
    std::barrier barrier(THREADS);

    // Create an array of threads to process the world in parallel
    std::thread threads[THREADS];

    // Calculate the number of rows to process in each thread
    config::coord_t rows_per_thread = rows / THREADS;

    // Partition the data processing into multiple parts and run them in separate threads
    for (int i = 0; i < THREADS; ++i) {
        config::coord_t start_row = i * rows_per_thread;
        config::coord_t end_row = (i == THREADS - 1) ? rows : start_row + rows_per_thread;
        threads[i] = std::thread([&](config::coord_t start, config::coord_t end) {

            for (config::coord_t row = start; row < end; ++row) {
          for (config::coord_t col = 0; col < cols; ++col) {

              if( world::running == false)
                {
                  std::unique_lock<std::mutex> lock(world::running_mutex);
                  return;  // nothing done if simulation is not running
                  lock.unlock();
                }
              // Perform the necessary data processing for each cell in the world
              auto current_pos = xy2array(col, row);
              auto neighbours = num_neighbours(col, row);


              // https://en.wikipedia.org/wiki/Conway's_Game_of_Life#Rules
              if ((*current)[current_pos] == Block::occupied && (neighbours == 2 || neighbours == 3)) {

                  // 1. stay alive if 2 or 3 neighbours
                  (*next)[current_pos] = Block::occupied;
                } else if ((*current)[current_pos] == Block::empty && neighbours == 3) {
                  // 2. spawn new if exactly three neighbours
                  (*next)[current_pos] = Block::occupied;
                } else {
                  // 3. else cell will be empty
                  (*next)[current_pos] = Block::empty;
                }
            }
        }
        // Wait at the barrier to synchronize the threads
        barrier.arrive_and_wait();
      }, start_row, end_row);
  }

  // Join the threads to wait for them to finish
  for (int i = 0; i < THREADS; ++i) {
      threads[i].join();
    }

  std::cerr << "next world created in: " << clock.elapsed() << std::endl;

  std::swap(current, next);
}



} // world
