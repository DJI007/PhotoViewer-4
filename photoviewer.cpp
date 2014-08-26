#include "photoviewer.h"
#include "ui_photoviewer.h"

#include <QFileDialog>
#include <QDir>
#include <QImage>
#include <QMimeDatabase>
#include <QMessageBox>
#include <QScrollArea>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QCoreApplication>
#include <QSettings>
#include <QToolBar>
#include <QStatusBar>
#include <QMenuBar>

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

    QCoreApplication::setOrganizationName("Cachirulop");
    QCoreApplication::setOrganizationDomain("cachirulop.com");
    QCoreApplication::setApplicationName("Photo viewer");

    _currentDir = new QDir ();
    _currentDir->setFilter (QDir::Files | QDir::Readable);
    _currentDir->setSorting (QDir::Name);

    _currentFile = 0;

    _pictureScene = new QGraphicsScene(this);
}

PhotoViewer::~PhotoViewer()
{
    delete ui;
    delete _currentDir;
    delete _pictureScene;
}

void PhotoViewer::on_actionChange_folder_triggered()
{
    QString directory;
    QFileDialog *fd = new QFileDialog;
    QSettings settings;
    // QFileDialog::getOpenFileName(this,
    //                                     tr("Open File"), QDir::currentPath());
    // QTreeView *tree = fd->findChild <QTreeView*>();

    // tree->setRootIsDecorated(true);
    // tree->setItemsExpandable(true);
    fd->setDirectory(settings.value("last_directory", "~").toString());
    fd->setFileMode (QFileDialog::Directory);
    fd->setOption (QFileDialog::ShowDirsOnly);
    fd->setViewMode (QFileDialog::Detail);

    int result = fd->exec();
    if (result)
    {
        directory = fd->selectedFiles ()[0];
        settings.setValue("last_directory", directory);

        _currentDir->setPath (fd->selectedFiles()[0]);
        _currentFile = 0;
        showCurrentPicture ();
    }

    delete fd;
}

void PhotoViewer::on_actionNext_picture_triggered()
{
    if (_currentFile < _currentDir->entryList().count() - 1) {
        _currentFile++;
        showCurrentPicture();
    }
    else {
        QMessageBox::information(this,
                                 tr("Image Viewer"),
                                 tr("Last picture"));
    }
}

void PhotoViewer::on_actionPrevious_picture_triggered()
{

    if (_currentFile > 0) {
        _currentFile--;
        showCurrentPicture();
    }
    else {
        QMessageBox::information(this,
                                 tr("Image Viewer"),
                                 tr("First picture"));
    }
}

void PhotoViewer::showCurrentPicture()
{
    QString fileName;
    QImage *img;

    // TODO: Use QMimeDatabase to get only supported files

    fileName = _currentDir->absoluteFilePath(_currentDir->entryList()[_currentFile]);
    img = new QImage (fileName);
    if (img->isNull()) {
        QMessageBox::information(this,
                                 tr("Image Viewer"),
                                 tr("Cannot load %1.").arg(fileName));
     }
    else {
        QGraphicsView *view;
        QGraphicsItem *item;
        qreal scale;
        int newWidth;
        int newHeight;

        view = this->findChild<QGraphicsView*> ("gvPicture");
        item = new QGraphicsPixmapItem(QPixmap::fromImage(*img));

        scale =  ((qreal) (view->width() - 4) / (qreal) img->width());
        if ((img->height() * scale) > view->height()) {
            scale =  ((qreal) (view->height() - 4) / (qreal) img->height());
        }
/*
        qDebug() << "Scale: " << QString::number(scale, 'f', 5) << ", img->width: " << img->width() << ", view->width: " << view->width();
        qDebug() << "Scale: " << QString::number(scale, 'f', 5) << ", img->height: " << img->height() << ", view->height: " << view->height();
*/
        item->setScale(scale);

        newWidth = img->width() * scale;
        newHeight = img->height() * scale;
        qreal newX;
        qreal newY;

        if (newWidth < view->width()) {
            newX = (view->width() / 2) - (newWidth / 2);
        }
        else {
            newX = 0;
        }

        if (newHeight < view->height()) {
            newY = (view->height() / 2) - (newHeight / 2);
        }
        else {
            newY = 0;
        }

        QTransform transform;

        transform.translate(newX, newY);

        item->setTransform(transform, true);

        _pictureScene->clear();
        _pictureScene->addItem(item);

        view->setScene(_pictureScene);

        // view in fullscreen
        /*
        view->setParent(NULL);
        view->setWindowFlags(view->windowFlags() |
                             Qt::CustomizeWindowHint |
                             Qt::WindowStaysOnTopHint |
                             Qt::WindowMaximizeButtonHint |
                             Qt::WindowCloseButtonHint);
        view->setWindowState(view->windowState() | Qt::WindowFullScreen);
        view->show();
        */

        // Window in fullscreen
        this->showFullScreen();
        QList<QToolBar *> toolbars;
        toolbars = this->findChildren<QToolBar*> ();
        for (int i = 0; i < toolbars.length(); i++) {
            toolbars[i]->hide();
        }

/*
        QList<QStatusBar *> statusbars;
        statusbars = this->findChildren<QStatusBar*> ();
        for (int i = 0; i < statusbars.length(); i++) {
            statusbars[i]->hide();
        }
*/
        QList<QMenuBar *> menubars;
        menubars = this->findChildren<QMenuBar*> ();
        for (int i = 0; i < menubars.length(); i++) {
            menubars[i]->hide();
        }


    }

    delete img;
}



