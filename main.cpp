#include "photoviewer.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator qtTranslator;
    QTranslator photoViewerTranslator;

    qtTranslator.load("qt_" + QLocale::system().name(),
                               QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    photoViewerTranslator.load("photoviewer_" + QLocale::system().name(),
                              a.applicationDirPath(),
                              "_",
                              ".qm");
    a.installTranslator(&photoViewerTranslator);

    PhotoViewer w;
    w.show();

    return a.exec();
}
