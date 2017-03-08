#-------------------------------------------------
#
# Project created by QtCreator 2015-07-08T10:16:49
#
#-------------------------------------------------

QT       += core gui multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fm
TEMPLATE = app


SOURCES += main.cpp\
        fm.cpp \
    song.cpp \
    mouseevent.cpp \
    custom.cpp \
    doubanfm.cpp \
    channel.cpp

HEADERS  += fm.h \
    song.h \
    mouseevent.h \
    custom.h \
    doubanfm.h \
    douban_types.h \
    channel.h

FORMS    += fm.ui \
    custom.ui \
    channel.ui

CONFIG += c++11

RESOURCES += \
    fm.qrc
