QT += core gui
greaterThan(QT_MAJOR_VERSION, 4) QT += widgets

TARGET = brainy
TEMPLATE = app

SOURCES += main.cc \
    interpreter.cc \
    mainwindow.cc \
    interpreterthread.cc

HEADERS += \
    interpreter.hh \
    mainwindow.hh \
    interpreterthread.hh

