#-------------------------------------------------
#
# Project created by QtCreator 2014-08-23T08:44:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoViewer
TEMPLATE = app


SOURCES += main.cpp\
        photoviewer.cpp

HEADERS  += photoviewer.h

FORMS    += photoviewer.ui

OTHER_FILES += \
    images/folder-open.png \
    images/media-playback-start.png

RESOURCES += \
    PhotoViewer.qrc
