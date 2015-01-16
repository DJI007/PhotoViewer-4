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
#include "settingsdialog.h"

/**
 * PhotoView application to view pictures in a simple mode
 *
 * @brief PhotoViewer::PhotoViewer
 * @param parent
 */
PhotoViewer::PhotoViewer(QWidget *parent) :
    QMainWindow(parent) ,
    ui(new Ui::PhotoViewer)
{
    ui->setupUi(this);

    // dock map window
    //ui->dwMap->hide();

    connect(ui->mainToolBar, SIGNAL(visibilityChanged(bool)),
            this, SLOT(on_mainToolbarVisibilityChanged (bool)));

    connect(ui->gvPicture,
            SIGNAL(requestMapWindow(double,double,double)),
            this,
            SLOT(on_pictureRequestMapWindow(double,double,double)));

    connect(ui->gvPicture, SIGNAL(showTimeEnded()),
            this, SLOT(on_pictureShowTimeEnded()));

    connect(ui->gvPicture, SIGNAL(beginItemAnimationIn()),
            this, SLOT(on_beginItemAnimation()));
    connect(ui->gvPicture, SIGNAL(endItemAnimationIn()),
            this, SLOT(on_endItemAnimation()));

    // QWidget *container;

    _mapView = new MapView();
    // container = QWidget::createWindowContainer(_mapView, this);
    // ui->dwMap->setWidget(container);
    // _mapView->show();

    _lblStatusFileCount = new QLabel (this);
    _lblStatusFileCount->setObjectName("lblStatusFileCount");
    _lblStatusFileCount->show();

    _lblStatusPath= new QLabel (this);
    _lblStatusPath->setObjectName("lblStatusPath");
    _lblStatusPath->show();

    ui->statusBar->addWidget(_lblStatusPath, 1);
    ui->statusBar->addWidget(_lblStatusFileCount);

    _toolBarTimer = new QTimer (this);
    _toolBarTimer->setInterval(3000);
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

    connect(ui->gvPicture,
            SIGNAL(mouseDoubleClick(QMouseEvent*)),
            this,
            SLOT(on_pictureDoubleClick(QMouseEvent*)));
    connect(ui->gvPicture,
            SIGNAL(mouseMove(QMouseEvent*)),
            this,
            SLOT(on_pictureMouseMove(QMouseEvent*)));

    // Add actions to main window to preserve the shortcuts in fullscreen mode
    this->addAction(ui->actionFirst_picture);
    this->addAction(ui->actionPrevious_picture);
    this->addAction(ui->actionPlay);
    this->addAction(ui->actionNext_picture);
    this->addAction(ui->actionLast_picture);
    this->addAction(ui->actionSet_0_stars);
    this->addAction(ui->actionSet_1_star);
    this->addAction(ui->actionSet_2_stars);
    this->addAction(ui->actionSet_3_stars);
    this->addAction(ui->actionSet_4_stars);
    this->addAction(ui->actionSet_5_stars);
    this->addAction(ui->actionFull_screen);
    this->addAction(ui->actionExit_full_screen);
    this->addAction(ui->actionRotate_Left);
    this->addAction(ui->actionRotate_Right);

    QString lastDirectory;
    QStringList filters;

    filters << "*.gif" << "*.jpg" << "*.jpeg" << "*.png" << "*.mp4" << "*.nef" << "*.avi" << "*.mts" << "*.cr2";

    lastDirectory = SettingsHelper::instance ().lastDirectory ();

    _currentDir = new QDir ();
    _currentDir->setFilter (QDir::Files | QDir::Readable);
    _currentDir->setSorting (QDir::Name);
    _currentDir->setPath(lastDirectory);
    _currentDir->setNameFilters(filters);

    _currentFile = SettingsHelper::instance().lastFileIndex();
    if (_currentFile >= _currentDir->count()) {
        _currentFile = 0;
    }

    if (lastDirectory.compare("~") != 0) {
        showCurrentPicture();

        ui->gvPicture->setInfoVisible(true);
    }

    loadSettings ();
}

PhotoViewer::~PhotoViewer()
{
    delete ui;
    delete _currentDir;
    delete _mapView;
}

void PhotoViewer::showCurrentPicture(PictureView::PictureAnimationType anim)
{
    QString fileName;
    QFileInfo info;

    if (_currentDir->count() == 0) {
        QMessageBox::information(this,
                                 tr("Photo Viewer"),
                                 tr("There is no images in this folder"));
        ui->gvPicture->cleanPicture ();
    }
    else {
        info = _currentDir->entryInfoList() [_currentFile];

        SettingsHelper::instance().setLastFileIndex(_currentFile);

        fileName = info.absoluteFilePath();

        ui->gvPicture->loadPicture(fileName);
        ui->gvPicture->showPicture(anim);

        if (_mapView->isVisible()) {
            _mapView->setPosition(ui->gvPicture->pictureLatitude(), ui->gvPicture->pictureLongitude(), 0);
        }
    }

    updateStatusBar();
}

void PhotoViewer::on_pictureDoubleClick(QMouseEvent *event)
{
    Q_UNUSED (event);

    toggleFullScreen();
}

void PhotoViewer::on_pictureMouseMove(QMouseEvent *event)
{
    if (isFullScreen()) {
        bool showToolbar;

        showToolbar = (event->y() == 0);
        showToolBarFullScreen(showToolbar);
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
        ui->gvPicture->setInfoVisible(true);

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
        ui->gvPicture->setInfoVisible(false);

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

void PhotoViewer::on_pictureShowTimeEnded()
{
    if (_currentFile < _currentDir->count() - 1) {
        _currentFile++;
        if (isFullScreen()) {
            showCurrentPicture(PictureView::PictureAnimationType::Random);
        }
        else {
            showCurrentPicture(PictureView::PictureAnimationType::RightToLeft);
        }
    }
    else {
        if (isFullScreen()) {
            QApplication::restoreOverrideCursor();
        }

        QMessageBox::information(this,
                                 tr("Photo Viewer"),
                                 tr("End of slide show!"));

        ui->actionPlay->setChecked(false);

        if (isFullScreen()) {
            QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
        }
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

void PhotoViewer::showToolBarFullScreen (bool showToolbar)
{
    if (!_toolBarTimer->isActive()) {
        QApplication::restoreOverrideCursor();
    }

    if (showToolbar) {
        ui->mainToolBar->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
        ui->mainToolBar->show();
    }

    ui->gvPicture->setInfoVisible(true);

    _toolBarTimer->start();
}

void PhotoViewer::hideToolBarFullScreen ()
{
    QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
    ui->mainToolBar->hide();

    ui->gvPicture->setInfoVisible(false);
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
    // fd->setOption (QFileDialog::ShowDirsOnly);
    fd->setViewMode (QFileDialog::Detail);

    int result = fd->exec();
    if (result)
    {
        directory = fd->selectedFiles ()[0];
        SettingsHelper::instance().setLastDirectory (directory);

        _currentDir->setPath (fd->selectedFiles()[0]);
        _currentFile = 0;
        ui->gvPicture->resize();
        showCurrentPicture ();
    }

    delete fd;
}

void PhotoViewer::on_actionFirst_picture_triggered()
{
    if (_currentFile > 0) {
        _currentFile = 0;
        showCurrentPicture(PictureView::PictureAnimationType::LeftToRight);
    }
    else {
        QMessageBox::information(this,
                                 tr("Photo Viewer"),
                                 tr("First picture"));
    }
}

void PhotoViewer::on_actionPrevious_picture_triggered()
{
    if (_currentFile > 0) {
        _currentFile--;
        showCurrentPicture(PictureView::PictureAnimationType::LeftToRight);
    }
    else {
        QMessageBox::information(this,
                                 tr("Photo Viewer"),
                                 tr("First picture"));
    }
}

void PhotoViewer::on_actionPlay_triggered()
{
    if (_currentDir->count() == 0) {
        QMessageBox::information(this,
                                 tr("Photo Viewer"),
                                 tr("There is no images in this folder"));
        ui->actionPlay->setChecked(false);
    }
    else {
        if (ui->actionPlay->isChecked()) {
            ui->gvPicture->setShowTime(SettingsHelper::instance().presentationInterval() * 1000);
        }
        else {
            ui->gvPicture->setShowTime(0);
        }
    }
}

void PhotoViewer::on_actionNext_picture_triggered()
{
    if (_currentFile < _currentDir->count() - 1) {
        _currentFile++;
        showCurrentPicture(PictureView::PictureAnimationType::RightToLeft);
    }
    else {
        QMessageBox::information(this,
                                 tr("Photo Viewer"),
                                 tr("Last picture"));
    }
}

void PhotoViewer::on_actionLast_picture_triggered()
{
    if (_currentFile < _currentDir->count() - 1) {
        _currentFile = _currentDir->count() - 1;
        showCurrentPicture(PictureView::PictureAnimationType::RightToLeft);
    }
    else {
        QMessageBox::information(this,
                                 tr("Photo Viewer"),
                                 tr("Last picture"));
    }
}

void PhotoViewer::on_actionFull_screen_triggered()
{
    toggleFullScreen();
}

void PhotoViewer::on_actionExit_full_screen_triggered()
{
    if (isFullScreen()) {
        toggleFullScreen();
    }
}

/*
void PhotoViewer::on_starsAction_setRating_triggered(int rating)
{
    setRating (rating);
}
*/
void PhotoViewer::on_actionSet_0_stars_triggered()
{
    setRating (0);
}

void PhotoViewer::on_actionSet_1_star_triggered()
{
    setRating (1);
}

void PhotoViewer::on_actionSet_2_stars_triggered()
{
    setRating (2);
}

void PhotoViewer::on_actionSet_3_stars_triggered()
{
    setRating (3);
}

void PhotoViewer::on_actionSet_4_stars_triggered()
{
    setRating (4);
}

void PhotoViewer::on_actionSet_5_stars_triggered()
{
    setRating (5);
}

void PhotoViewer::setRating(int value)
{
    ui->gvPicture->setPictureRating(value);
    showToolBarFullScreen (false);
}

void PhotoViewer::updateStatusBar ()
{
    _lblStatusFileCount->setText(QString ("[%1/%2]").arg(_currentFile + 1).arg(_currentDir->count()));
    _lblStatusPath->setText(_currentDir->absolutePath());
}

void PhotoViewer::on_pictureRequestMapWindow (double latitude, double longitude, double altitude)
{
    // ui->dwMap->show();
    _mapView->show();
    _mapView->setPosition(latitude, longitude, altitude);
}


void PhotoViewer::on_actionRotate_Left_triggered()
{
    ui->gvPicture->rotatePictureLeft ();
}

void PhotoViewer::on_actionRotate_Right_triggered()
{
    ui->gvPicture->rotatePictureRight ();
}

void PhotoViewer::on_actionConfig_triggered()
{
    SettingsDialog *dlg;

    dlg = new SettingsDialog(this);
    dlg->setWindowModality(Qt::ApplicationModal);
    if (dlg->exec () == QDialog::Accepted) {
        dlg->saveSettings ();

        loadSettings ();

        ui->gvPicture->updateShowInformation ();
    }
}

void PhotoViewer::loadSettings()
{
    ui->actionShow_toolbar->setChecked(SettingsHelper::instance().showToolbar());
    ui->mainToolBar->setVisible(SettingsHelper::instance().showToolbar());
}

void PhotoViewer::on_actionShow_toolbar_toggled(bool arg1)
{
    ui->mainToolBar->setVisible(arg1);
    SettingsHelper::instance().setShowToolbar(arg1);
}

void PhotoViewer::on_mainToolbarVisibilityChanged(bool visible)
{
    ui->actionShow_toolbar->setChecked(visible);
}

void PhotoViewer::on_beginItemAnimation()
{
    ui->actionFirst_picture->setEnabled(false);
    ui->actionPrevious_picture->setEnabled(false);
    ui->actionNext_picture->setEnabled(false);
    ui->actionLast_picture->setEnabled(false);
}

void PhotoViewer::on_endItemAnimation()
{
    ui->actionFirst_picture->setEnabled(true);
    ui->actionPrevious_picture->setEnabled(true);
    ui->actionNext_picture->setEnabled(true);
    ui->actionLast_picture->setEnabled(true);
}
