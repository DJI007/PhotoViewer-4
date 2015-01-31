#ifndef PHOTOVIEWER_H
#define PHOTOVIEWER_H

#include <QMainWindow>
#include <QDir>
#include <QTimer>
#include <QLabel>

#include "pictureview.h"
#include "mapview.h"

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

    void on_actionFirst_picture_triggered();
    void on_actionPrevious_picture_triggered();
    void on_actionPlay_triggered();
    void on_actionNext_picture_triggered();
    void on_actionLast_picture_triggered();

    void on_actionSet_0_stars_triggered();
    void on_actionSet_1_star_triggered();
    void on_actionSet_2_stars_triggered();
    void on_actionSet_3_stars_triggered();
    void on_actionSet_4_stars_triggered();
    void on_actionSet_5_stars_triggered();

    void on_pictureDoubleClick(QMouseEvent *event);
    void on_pictureMouseMove(QMouseEvent *event);

    void on_pictureShowTimeEnded ();

    void on_toolBarTimerTimeout();

    void on_pictureRequestMapWindow (double latitude, double longitude, double altitude);

    void on_actionFull_screen_triggered();
    void on_actionExit_full_screen_triggered();

    void on_actionRotate_Left_triggered();
    void on_actionRotate_Right_triggered();

    void on_actionConfig_triggered();

    void on_actionShow_toolbar_toggled(bool arg1);

    void on_mainToolbarVisibilityChanged(bool visible);

    void on_beginItemAnimation ();
    void on_endItemAnimation ();

    void on_actionGo_to_picture_triggered();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::PhotoViewer *ui;

    QDir *_currentDir;
    uint _currentFile;
    bool _lastStatusMaximized;
    Qt::WindowFlags _toolBarWindowFlags;

    QTimer *_toolBarTimer;

    QLabel *_lblStatusFileCount;
    QLabel *_lblStatusPath;

    MapView *_mapView;

    void showCurrentPicture (PictureView::PictureAnimationType anim = PictureView::PictureAnimationType::None);
    void toggleFullScreen();

    void startToolBarFullScreen ();
    void showToolBarFullScreen (bool showToolbar);
    void hideToolBarFullScreen ();
    void endToolBarFullScreen ();

    void setMenuBarsVisibility (bool visible);
    void setStatusBarsVisibility (bool visible);
    void setToolBarsVisibility (bool visible);

    void setRating (int value);

    void updateStatusBar ();
    void updateShowInfo ();

    void loadSettings ();
};

#endif // PHOTOVIEWER_H
