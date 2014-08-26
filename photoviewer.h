#ifndef PHOTOVIEWER_H
#define PHOTOVIEWER_H

#include <QMainWindow>
#include <QDir>
#include <QImage>
#include <QGraphicsScene>

namespace Ui {
class PhotoViewer;
}

class PhotoViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit PhotoViewer(QWidget *parent = 0);
    ~PhotoViewer();

private slots:
    void on_actionChange_folder_triggered();

    void on_actionNext_picture_triggered();

    void on_actionPrevious_picture_triggered();

private:
    Ui::PhotoViewer *ui;

    QDir *_currentDir;
    int _currentFile;

    QGraphicsScene *_pictureScene;

    void showCurrentPicture ();

};

#endif // PHOTOVIEWER_H
