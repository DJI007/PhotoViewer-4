#include "photoviewer.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QDebug>
#include <QProcessEnvironment>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator qtTranslator;
    QTranslator photoViewerTranslator;

    // Print environment
/*
    QStringList variables;

    variables = QProcessEnvironment::systemEnvironment ().toStringList();
    QTextStream(stdout) << "Environment: " << endl;
    foreach(QString x, variables) {
        QTextStream(stdout) << x << endl;
    }
    QTextStream(stdout) << "<<<<<<<<<<<<<<<<<<<: " << endl;
*/

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
