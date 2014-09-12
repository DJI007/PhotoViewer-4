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
    void on_actionPrevious_picture_triggered();
    void on_actionPlay_triggered();
    void on_actionNext_picture_triggered();
    void on_actionSet_0_stars_triggered();
    void on_actionSet_1_star_triggered();
    void on_actionSet_2_stars_triggered();
    void on_actionSet_3_stars_triggered();
    void on_actionSet_4_stars_triggered();
    void on_actionSet_5_stars_triggered();

    void on_pictureDoubleClick();
    void on_pictureMouseMove();
/*
    void on_actionSet_1_star_hovered();
    void on_actionSet_2_stars_hovered();
*/

    void on_playerTimerTimeout();
    void on_toolBarTimerTimeout();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::PhotoViewer *ui;

    const int PLAYER_TIMER_MILLISECONDS = 5000;

    QDir *_currentDir;
    int _currentFile;
    bool _lastStatusMaximized;
    Qt::WindowFlags _toolBarWindowFlags;

    QTimer *_playerTimer;
    QTimer *_toolBarTimer;

    void showCurrentPicture (PictureView::PictureAnimationType anim = PictureView::PictureAnimationType::None);
    void toggleFullScreen();

    void startToolBarFullScreen ();
    void showToolBarFullScreen ();
    void hideToolBarFullScreen ();
    void endToolBarFullScreen ();

    void setMenuBarsVisibility (bool visible);
    void setStatusBarsVisibility (bool visible);
    void setToolBarsVisibility (bool visible);
};

#endif // PHOTOVIEWER_H
