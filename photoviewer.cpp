#include "photoviewer.h"
#include "ui_photoviewer.h"

#include <QFileDialog>
#include <QDir>
#include <QMimeDatabase>
#include <QMessageBox>
#include <QScrollArea>
#include <QDebug>
#include <QSettings>
#include <QToolBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QPropertyAnimation>

#include "settingshelper.h"
#include "starsaction.h"

/**
 * PhotoView application to view pictures in a simple mode
 *
 * @brief PhotoViewer::PhotoViewer
 * @param parent
 */
PhotoViewer::PhotoViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PhotoViewer)
{
    ui->setupUi(this);

    _playerTimer = new QTimer (this);
    _playerTimer->setInterval(PLAYER_TIMER_MILLISECONDS);
    connect (_playerTimer,
             SIGNAL(timeout()),
             this,
             SLOT(on_playerTimerTimeout()));

    _toolBarTimer = new QTimer (this);
    _toolBarTimer->setInterval(2000);
    connect (_toolBarTimer,
             SIGNAL(timeout()),
             this,
             SLOT(on_toolBarTimerTimeout()));

    StarsAction *action;

    action = new StarsAction (ui->mainToolBar);
    ui->mainToolBar->addAction(action);

    connect(action,
            SIGNAL(setRating(int)),
            ui->gvPicture,
            SLOT(setPictureRating(int)));

    QString lastDirectory;
    QStringList filters;

    filters << "*.gif" << "*.jpg" << "*.jpeg" << "*.png";

    lastDirectory = SettingsHelper::instance ().lastDirectory ();

    _currentDir = new QDir ();
    _currentDir->setFilter (QDir::Files | QDir::Readable);
    _currentDir->setSorting (QDir::Name);
    _currentDir->setPath(lastDirectory);
    _currentDir->setNameFilters(filters);

    _currentFile = 0;

    if (lastDirectory.compare("~") != 0) {
        showCurrentPicture();
    }

    connect(ui->gvPicture,
            SIGNAL(mouseDoubleClick(QMouseEvent*)),
            this,
            SLOT(on_pictureDoubleClick()));
    connect(ui->gvPicture,
            SIGNAL(mouseMove(QMouseEvent*)),
            this,
            SLOT(on_pictureMouseMove()));

}

PhotoViewer::~PhotoViewer()
{
    delete ui;
    delete _currentDir;
}

void PhotoViewer::showCurrentPicture(PictureView::PictureAnimationType anim)
{
    QString fileName;

    fileName = _currentDir->absoluteFilePath(_currentDir->entryList()[_currentFile]);

    ui->gvPicture->loadPicture (fileName);
    ui->gvPicture->showPicture (anim);
}

void PhotoViewer::on_pictureDoubleClick()
{
    toggleFullScreen();
}

void PhotoViewer::on_pictureMouseMove()
{
    if (isFullScreen()) {
        showToolBarFullScreen();
    }
}

void PhotoViewer::toggleFullScreen()
{
    if (this->isFullScreen()) {
        endToolBarFullScreen();

        setMenuBarsVisibility(true);
        setStatusBarsVisibility(true);
        setToolBarsVisibility(true);

        ui->gvPicture->setNormalBackground();

        QApplication::restoreOverrideCursor();
        if (_lastStatusMaximized) {
            showMaximized();
        }
        else {
            showNormal();
        }
    }
    else {
        startToolBarFullScreen();

        _lastStatusMaximized = this->isMaximized();

        setMenuBarsVisibility(false);
        setStatusBarsVisibility(false);
        setToolBarsVisibility(false);

        ui->gvPicture->setFullScreenBackground();

        QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
        showFullScreen();
    }
}

void PhotoViewer::setMenuBarsVisibility(bool visible)
{
    QList<QMenuBar *> menubars;

    menubars = this->findChildren<QMenuBar*> ();
    for (int i = 0; i < menubars.length(); i++) {
        if (visible) {
            menubars[i]->show();
        }
        else {
            menubars[i]->hide();
        }
    }
}

void PhotoViewer::setStatusBarsVisibility(bool visible)
{
    QList<QStatusBar *> statusbars;

    statusbars = this->findChildren<QStatusBar*> ();
    for (int i = 0; i < statusbars.length(); i++) {
        if (visible) {
            statusbars[i]->show();
        }
        else {
            statusbars[i]->hide();
        }
    }
}

void PhotoViewer::setToolBarsVisibility(bool visible)
{
    QList<QToolBar *> toolbars;

    toolbars = this->findChildren<QToolBar*> ();
    for (int i = 0; i < toolbars.length(); i++) {
        if (visible) {
            toolbars[i]->show();
        }
        else {
            toolbars[i]->hide();
        }
    }
}

void PhotoViewer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (this->ui->gvPicture->hasPicture()) {
        this->ui->gvPicture->resize ();
    }
}

/*
void PhotoViewer::on_actionSet_1_star_hovered()
{
}

void PhotoViewer::on_actionSet_2_stars_hovered()
{
   this->ui->actionSet_1_star->setChecked(true);
}
*/

void PhotoViewer::on_playerTimerTimeout ()
{
    if (_currentFile < _currentDir->entryList().count() - 1) {
        _currentFile++;
        showCurrentPicture(PictureView::PictureAnimationType::RightToLeft);
    }
    else {
        QMessageBox::information(this,
                                 tr("Image Viewer"),
                                 tr("End of slide show!"));
        _playerTimer->stop();
    }
}

void PhotoViewer::on_toolBarTimerTimeout ()
{
    hideToolBarFullScreen();
}

void PhotoViewer::startToolBarFullScreen ()
{
    _toolBarWindowFlags = ui->mainToolBar->windowFlags();
}

void PhotoViewer::showToolBarFullScreen ()
{
   QApplication::restoreOverrideCursor();

    ui->mainToolBar->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    ui->mainToolBar->show();

    _toolBarTimer->start();
}

void PhotoViewer::hideToolBarFullScreen ()
{
    QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
    ui->mainToolBar->hide();
    _toolBarTimer->stop();
}

void PhotoViewer::endToolBarFullScreen ()
{
    ui->mainToolBar->setWindowFlags(_toolBarWindowFlags);
    _toolBarTimer->stop();
}

void PhotoViewer::on_actionChange_folder_triggered()
{
    QString directory;
    QFileDialog *fd = new QFileDialog;

    fd->setDirectory(SettingsHelper::instance ().lastDirectory ());
    fd->setFileMode (QFileDialog::Directory);
    fd->setOption (QFileDialog::ShowDirsOnly);
    fd->setViewMode (QFileDialog::Detail);

    int result = fd->exec();
    if (result)
    {
        directory = fd->selectedFiles ()[0];
        SettingsHelper::instance().setLastDirectory (directory);

        _currentDir->setPath (fd->selectedFiles()[0]);
        _currentFile = 0;
        showCurrentPicture ();
    }

    delete fd;
}

void PhotoViewer::on_actionPrevious_picture_triggered()
{

    if (_currentFile > 0) {
        _currentFile--;
        showCurrentPicture(PictureView::PictureAnimationType::LeftToRight);

        if (_playerTimer->isActive()) {
            _playerTimer->stop();
            _playerTimer->start();
        }
    }
    else {
        QMessageBox::information(this,
                                 tr("Image Viewer"),
                                 tr("First picture"));
    }
}

void PhotoViewer::on_actionPlay_triggered()
{
    if (_playerTimer->isActive()) {

        _playerTimer->stop();
    }
    else {
        _playerTimer->start();
    }
}

void PhotoViewer::on_actionNext_picture_triggered()
{
    if (_currentFile < _currentDir->entryList().count() - 1) {
        _currentFile++;
        showCurrentPicture(PictureView::PictureAnimationType::RightToLeft);

        if (_playerTimer->isActive()) {
            _playerTimer->stop();
            _playerTimer->start();
        }
    }
    else {
        QMessageBox::information(this,
                                 tr("Image Viewer"),
                                 tr("Last picture"));
    }
}

void PhotoViewer::on_actionSet_0_stars_triggered()
{
    ui->gvPicture->setPictureRating(0);
}

void PhotoViewer::on_actionSet_1_star_triggered()
{
    ui->gvPicture->setPictureRating(1);
}

void PhotoViewer::on_actionSet_2_stars_triggered()
{
    ui->gvPicture->setPictureRating(2);
}

void PhotoViewer::on_actionSet_3_stars_triggered()
{
    ui->gvPicture->setPictureRating(3);
}

void PhotoViewer::on_actionSet_4_stars_triggered()
{
    ui->gvPicture->setPictureRating(4);
}

void PhotoViewer::on_actionSet_5_stars_triggered()
{
    ui->gvPicture->setPictureRating(5);
}

