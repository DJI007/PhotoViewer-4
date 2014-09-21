#-------------------------------------------------
#
# Project created by QtCreator 2014-08-23T08:44:35
#
#-------------------------------------------------

QT += core gui
QT += positioning location
QT += qml quick

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
    animateditemtext.cpp \
    animateditempicture.cpp \
    animationfade.cpp \
    animationslide.cpp \
    animationrotate.cpp \
    animationrotatemove.cpp \
    animationscale.cpp

HEADERS  += photoviewer.h \
    pictureview.h \
    exifmetadata.h \
    settingshelper.h \
    starsaction.h \
    starlabel.h \
    animateditemtext.h \
    animateditempicture.h \
    pictureanimation.h \
    animationfade.h \
    animationslide.h \
    animationrotate.h \
    animationrotatemove.h \
    animationscale.h

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

    # exiv2
    INCLUDEPATH  += /usr/local/include
    LIBS += -lexiv2

    # marble
    #LIBS += -L/usr/local/lib -lmarblewidget
}

OTHER_FILES += \
    MapViewer.qml

