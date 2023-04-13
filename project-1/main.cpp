//
// Conway's Game of Life
// https://en.wikipedia.org/wiki/Conway's_Game_of_Life
//
// Course template, COMP.CS.320 Concurrency, spring 2023
// license: Free to use for any purpose in tuni.fi teaching,
// other uses: https://creativecommons.org/licenses/by-nc-sa/4.0/
//
#include "config.hh"
#include "world.hh"
#include "graphics.hh"
#include "grtimer.hh"

#include "tuni.hh"
TUNI_WARN_OFF()
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
TUNI_WARN_ON()

#include <thread>
#include <chrono>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  // data model of the world
  world::init();

  // Qt pixmap of the world
  graphics::init();

  // Qt graphics scene to show the pixmap
  QGraphicsScene scene;
  QGraphicsPixmapItem grarea(*(graphics::current_pixmap));
  scene.addItem( &grarea );

  // Qt graphics view of the pixmap world (scaled)
  QGraphicsView grview(&scene);
  grview.setCacheMode( QGraphicsView::CacheModeFlag::CacheNone );
  grview.setTransformationAnchor(QGraphicsView::AnchorViewCenter);
  grview.scale(config::scale, config::scale);


  // timer to periodically update the graphics
  GrTimer myTimer( &grarea );
  QObject::connect(&app,SIGNAL(aboutToQuit()),&myTimer,SLOT(closing()));


  //    // new thread to periodically update the
  //    // simulation world
      std::thread update( [&]() {
          while(world::running) {
            std::this_thread::sleep_for( config::world_tick );
            world::next_generation();
          };
      });


  grview.show();
  app.exec(); // main thread handles the Qt event loop

  return EXIT_SUCCESS;
}
