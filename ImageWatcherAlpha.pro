#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T20:08:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageWatcherAlpha
TEMPLATE = app


SOURCES += main.cpp\
        imagewatcher.cpp

HEADERS  += imagewatcher.h

FORMS    += imagewatcher.ui

QMAKE_CXXFLAGS += -std=c++11
