#include "world.hh"
#include "graphics.hh"
#include "stopwatch.hh"

#include "tuni.hh"
TUNI_WARN_OFF()
#include <QPixmap>
#include <QPainter>
TUNI_WARN_ON()

#include <iostream>

namespace graphics {

QPixmap* current_pixmap = nullptr;
QPixmap* next_pixmap = nullptr;


void init(void) {
    current_pixmap = new QPixmap( config::width, config::height );
    current_pixmap->fill( bgColor );
    next_pixmap = new QPixmap( config::width, config::height );
    next_pixmap->fill( bgColor );
}

void draw_board()
{
    QPainter painter(next_pixmap);
    stopwatch clock;

    for(config::coord_t x=0; x<config::width; x++)
    {
        for(config::coord_t y=0; y<config::height; y++ ) {
            bool isEmpty = (*world::current)[ world::xy2array(x,y) ] == world::Block::empty;
            if( isEmpty ) {
                painter.setPen(bgColor);
                painter.drawPoint(x,y);
            } else {
                painter.setPen(fgColor);
                painter.drawPoint(x,y);

            }
        }
    }
    std::cerr << "graphics pixmap created in: " << clock.elapsed() << std::endl;

    std::swap( next_pixmap, current_pixmap );
}


} // graphics
