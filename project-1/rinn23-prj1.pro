QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++2a

SOURCES += \
    graphics.cpp \
    main.cpp \
    grtimer.cpp \
    stopwatch.cpp \
    world.cpp

HEADERS += \
    config.hh \
    graphics.hh \
    grtimer.hh \
    stopwatch.hh \
    tuni.hh \
    world.hh

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
