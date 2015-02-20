#-------------------------------------------------
#
# Project created by QtCreator 2014-12-08T11:25:50
#
#-------------------------------------------------

QT       += core gui
unix: QT += x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTimeTrack
TEMPLATE = app


SOURCES += main.cpp\
        settings.cpp \
    x11helper.cpp

HEADERS  += settings.h \
    x11helper.h

FORMS    += settings.ui

RESOURCES += \
    resources.qrc

#unix:!macx: LIBS += -lxcb -lxcb-screensaver

#unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += xcb xcb-screensaver xcb-xkb xcb-cursor x11-xcb
