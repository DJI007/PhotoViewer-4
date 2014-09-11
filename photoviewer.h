#ifndef PHOTOVIEWER_H
#define PHOTOVIEWER_H

#include <QMainWindow>
#include <QDir>
#include <QTimer>

#include "pictureview.h"

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

    void on_actionPlay_triggered();

    void on_playerTimerTimeout();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::PhotoViewer *ui;

    QDir *_currentDir;
    int _currentFile;
    bool _lastStatusMaximized;

    QTimer *_playerTimer;

    void showCurrentPicture (PictureView::PictureAnimationType anim = PictureView::PictureAnimationType::None);
    void hideWidgetList(QList<QWidget *>);
    void toggleFullScreen();

};

#endif // PHOTOVIEWER_H
