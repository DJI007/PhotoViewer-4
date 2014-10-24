#-------------------------------------------------
#
# Project created by QtCreator 2014-08-23T08:44:35
#
#-------------------------------------------------

QT += core gui
QT += positioning location
QT += qml quick
QT += multimedia multimediawidgets
# QT += av
# QT += declarative
# QT += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoViewer
TEMPLATE = app

SOURCES += main.cpp\
        photoviewer.cpp \
    pictureview.cpp \
    exifmetadata.cpp \
    settingshelper.cpp \
    starsaction.cpp \
    starlabel.cpp \
#    animateditempicture.cpp \
    animationfade.cpp \
    animationslide.cpp \
    animationrotate.cpp \
    animationrotatemove.cpp \
    animationscale.cpp \
    mapview.cpp \
#    animateditemvideo.cpp \
    objectpixmapitem.cpp \
    videoitem.cpp \
    pictureviewitem.cpp \
    pictureviewitemcontainer.cpp \
    clickabletextitem.cpp \
    animatedtextitem.cpp \
    animationrotatefade.cpp \
    animationrotatescale.cpp \
    videocontrolpanel.cpp \
    xmpmetadata.cpp

HEADERS  += photoviewer.h \
    pictureview.h \
    exifmetadata.h \
    settingshelper.h \
    starsaction.h \
    starlabel.h \
#    animateditempicture.h \
    pictureanimation.h \
    animationfade.h \
    animationslide.h \
    animationrotate.h \
    animationrotatemove.h \
    animationscale.h \
    mapview.h \
#    animateditemvideo.h \
    animateditem.h \
    objectpixmapitem.h \
    videoitem.h \
    pictureviewitem.h \
    pictureviewitemcontainer.h \
    abstractmetadata.h \
    clickabletextitem.h \
    animatedtextitem.h \
    animationrotatefade.h \
    animationrotatescale.h \
    videocontrolpanel.h \
    xmpmetadata.h

FORMS    += photoviewer.ui

TRANSLATIONS = photoviewer_es.ts

RESOURCES += \
    PhotoViewer.qrc

win32|win32-msvc* {
    #exiv2
    INCLUDEPATH  += $$quote(C:/Users/dmagro/git/exiv2-0.24/msvc2012/include)
    LIBS         += $$quote(C:/Users/dmagro/git/exiv2-0.24/msvc2012/exiv2lib/Win32/Debug/exiv2sd.lib)
    LIBS         += $$quote(C:/Users/dmagro/git/exiv2-0.24/msvc2012/expat/Win32/Debug/libexpat.lib)
    LIBS         += $$quote(C:/Users/dmagro/git/exiv2-0.24/msvc2012/zlib/Win32/Debug/zlib1d.lib)
    LIBS         += $$quote(C:/Users/dmagro/git/exiv2-0.24/msvc2012/xmpsdk/Win32/Debug/xmpsdk.lib)

    #marble
    #INCLUDEPATH += $$quote(d:/programas/marble/include)
    #LIBS += $$quote(D:/Programas/marble/libmarblewidget.dll.a)
    #INCLUDEPATH += $$quote(C:/Program Files/Marble)
    #LIBS += $$quote(C:/Program Files/Marble/marblewidget.dll)
    #INCLUDEPATH += $$quote(C:/marble-tmp/)
    #LIBS += $$quote(C:/Program Files/Marble/marblewidget.dll)

}

unix {
    # enum
    QMAKE_CXXFLAGS += -std=c++11

    LIBS += -lexiv2

    INCLUDEPATH += -I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/libxml2
    LIBS += -pthread -lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lxml2 -lglib-2.0

    # marble
    #LIBS += -L/usr/local/lib -lmarblewidget

    # Qt 5.3.2
    # LIBS += -L/opt/Qt5.3.2/5.3/gcc_64/lib/
    # INCLUDEPATH  += /opt/Qt5.3.2/5.3/gcc_64/include

    # lib vlc-qt
    # LIBS += -lvlc-qt -lvlc-qt-widgets
}

OTHER_FILES += \
    MapViewer.qml \
    photoviewerUML.class.violet.html

