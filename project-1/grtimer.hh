#ifndef GRTIMER_HH 
#define GRTIMER_HH

#include "tuni.hh"
TUNI_WARN_OFF()
#include <QObject>
#include <QGraphicsPixmapItem>
TUNI_WARN_ON()

// helper class with two purposes:
// 1. generate periodic Qt Timer tick to redraw graphics
// 2. get notification of the application closing
//
class GrTimer : public QObject
{
    Q_OBJECT;
public:
    explicit GrTimer( QGraphicsPixmapItem* );

public slots:
    void closing();

protected:
    void timerEvent(QTimerEvent*) override;

private:
    QGraphicsPixmapItem* target_;
    int timerID_;
};

#endif 
