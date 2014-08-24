#include "photoviewer.h"
#include "ui_photoviewer.h"

#include <QFileDialog>
#include <QDir>
#include <QImage>
#include <QMimeDatabase>
#include <QMessageBox>
#include <QScrollArea>
#include <QGraphicsPixmapItem>

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
    QImage *_currentImage;
    // QFileDialog::getOpenFileName(this,
    //                                     tr("Open File"), QDir::currentPath());
    // QTreeView *tree = fd->findChild <QTreeView*>();

    // tree->setRootIsDecorated(true);
    // tree->setItemsExpandable(true);
    fd->setFileMode (QFileDialog::Directory);
    fd->setOption (QFileDialog::ShowDirsOnly);
    fd->setViewMode (QFileDialog::Detail);

    int result = fd->exec();
    if (result)
    {
        directory = fd->selectedFiles ()[0];

        _currentDir->setPath (fd->selectedFiles()[0]);
        _currentFile = 0;
        showCurrentPicture ();
    }

    delete fd;
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

        view = this->findChild<QGraphicsView*> ("gvPicture");

        item = new QGraphicsPixmapItem(QPixmap::fromImage(*img));
        item->setScale(0.2);

        _pictureScene->clear();
        _pictureScene->addItem(item);

        view->setScene(_pictureScene);
    }
}
















