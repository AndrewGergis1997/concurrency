#ifndef GRAPHICS_HH
#define GRAPHICS_HH

#include "tuni.hh"
TUNI_WARN_OFF()
#include <QImage>
#include <QWidget>
#include <QRgb>
#include <QLabel>
TUNI_WARN_ON()

namespace graphics {

    const auto bgColor = Qt::white;
    const auto fgColor =  Qt::black;

    // currently displayed pixmap:
    extern QPixmap* current_pixmap;
    // next update uses this pixmap:
    extern QPixmap* next_pixmap;


    void init( void );
    void draw_board(); // update world data to graphics scene
}

#endif // GRAPHICS_HH
