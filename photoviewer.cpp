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

#include <exiv2/exiv2.hpp>

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

    QSettings settings;

    _currentDir = new QDir ();
    _currentDir->setFilter (QDir::Files | QDir::Readable);
    _currentDir->setSorting (QDir::Name);
    _currentDir->setPath(settings.value("last_directory", "~").toString());

    qDebug () << settings.value("last_directory", "~").toString();

    _currentFile = 0;

    _pictureScene = new QGraphicsScene(this);
    _pictureScene->setObjectName("gsScene");
    ui->gvPicture->setScene(_pictureScene);

    // qApp->installEventFilter(this);
    /*
    _pictureScene->installEventFilter(this);

    ui->mainToolBar->installEventFilter(this);
    ui->gvPicture->installEventFilter(this);
    */
    QObject::connect(ui->gvPicture,
                     SIGNAL(mouseDoubleClick(QMouseEvent*)),
                     this,
                     SLOT(on_pictureDoubleClick()));
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
        QGraphicsItem *item;
        qreal scale;
        int newWidth;
        int newHeight;

        item = new QGraphicsPixmapItem(QPixmap::fromImage(*img));

        scale =  ((qreal) (ui->gvPicture->width() - 4) / (qreal) img->width());
        if ((img->height() * scale) > ui->gvPicture->height()) {
            scale =  ((qreal) (ui->gvPicture->height() - 4) / (qreal) img->height());
        }
/*
        qDebug() << "Scale: " << QString::number(scale, 'f', 5) << ", img->width: " << img->width() << ", ui->gvPicture->width: " << ui->gvPicture->width();
        qDebug() << "Scale: " << QString::number(scale, 'f', 5) << ", img->height: " << img->height() << ", ui->gvPicture->height: " << ui->gvPicture->height();
*/
        item->setScale(scale);

        newWidth = img->width() * scale;
        newHeight = img->height() * scale;
        qreal newX;
        qreal newY;

        if (newWidth < ui->gvPicture->width()) {
            newX = (ui->gvPicture->width() / 2) - (newWidth / 2);
        }
        else {
            newX = 0;
        }

        if (newHeight < ui->gvPicture->height()) {
            newY = (ui->gvPicture->height() / 2) - (newHeight / 2);
        }
        else {
            newY = 0;
        }

        QTransform transform;

        transform.translate(newX, newY);

        //item->setTransform(transform, true);

        QRect rect;

        rect.adjust(_pictureScene->sceneRect().left(), _pictureScene->sceneRect().top(), newWidth, newHeight);
        _pictureScene->setSceneRect(rect);
        _pictureScene->clear();
        _pictureScene->addItem(item);

        // EXIF information
        Exiv2::Image::AutoPtr imageData = Exiv2::ImageFactory::open(fileName.toUtf8().constData());

        imageData->readMetadata ();

        Exiv2::ExifData &exifData = imageData->exifData();

        if (exifData.empty()) {
            QMessageBox::information(this,
                                     tr("Image Viewer"),
                                     tr("Cannot load %1 data.").arg(fileName));
        }
        else {
            Exiv2::ExifData::const_iterator end = exifData.end();

            /*
             * Interesting tags:
             * "Exif.Image.Rating" -> stars
             * Exif.Photo.DateTimeDigitized or Exif.Photo.DateTimeOriginal
             * Exif.Image.GPSTag, Exif.GPSInfo.GPSLatitude, Exif.GPSInfo.*
             */
            qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
            for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i) {
                const char* tn = i->typeName();

                qDebug () << QString::fromUtf8(i->key().c_str()) << " "
                          << i->tag() << " "
                          << (tn ? tn : "Unknown") << " "
                          << i->count() << "  "
                          << QString::fromUtf8(i->value().toString ().c_str());
            }
            qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";

            QGraphicsTextItem * io = new QGraphicsTextItem;

            io->setPos(150,70);
            io->setHtml("<span style=\"background-color: black; color: white; margin:5px 5px 5px 5px\">Hello world!</span>");

            _pictureScene->addRect(io->boundingRect(), QPen(QColor::fromRgb(0, 0, 0)));
            _pictureScene->addItem(io);
        }
   }

    delete img;
}

void PhotoViewer::on_pictureDoubleClick()
{
    qDebug () << "Picture double click";
    toggleFullScreen();
}

void PhotoViewer::toggleFullScreen()
{
    if (this->isFullScreen()) {
        this->showNormal();

        QList<QStatusBar *> statusbars;
        statusbars = this->findChildren<QStatusBar*> ();
        for (int i = 0; i < statusbars.length(); i++) {
            statusbars[i]->show();
        }
        QList<QMenuBar *> menubars;
        menubars = this->findChildren<QMenuBar*> ();
        for (int i = 0; i < menubars.length(); i++) {
            menubars[i]->show();
        }

    }
    else {
        this->showFullScreen();

        /*
        QList<QToolBar *> toolbars;
        toolbars = this->findChildren<QToolBar*> ();
        for (int i = 0; i < toolbars.length(); i++) {
            toolbars[i]->hide();
        }
*/

        QList<QStatusBar *> statusbars;
        statusbars = this->findChildren<QStatusBar*> ();
        for (int i = 0; i < statusbars.length(); i++) {
            statusbars[i]->hide();
        }
        QList<QMenuBar *> menubars;
        menubars = this->findChildren<QMenuBar*> ();
        for (int i = 0; i < menubars.length(); i++) {
            menubars[i]->hide();
        }
    }
}

void PhotoViewer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    showCurrentPicture();
}

