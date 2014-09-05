#ifndef PHOTOVIEWER_H
#define PHOTOVIEWER_H

#include <QMainWindow>
#include <QDir>

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

    void on_pictureDoubleClick();

    void on_actionSet_1_star_hovered();

    void on_actionSet_2_stars_hovered();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::PhotoViewer *ui;

    QDir *_currentDir;
    int _currentFile;


    void showCurrentPicture (bool animation = false);
    void hideWidgetList(QList<QWidget *>);
    void toggleFullScreen();

};

#endif // PHOTOVIEWER_H
