#include "photoviewer.h"
#include "ui_photoviewer.h"

#include <QFileDialog>
#include <QDir>
#include <QMimeDatabase>
#include <QMessageBox>
#include <QScrollArea>
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

    QSettings settings;

    _currentDir = new QDir ();
    _currentDir->setFilter (QDir::Files | QDir::Readable);
    _currentDir->setSorting (QDir::Name);
    _currentDir->setPath(settings.value("last_directory", "~").toString());

    qDebug () << settings.value("last_directory", "~").toString();

    _currentFile = 0;

    QObject::connect(ui->gvPicture,
                     SIGNAL(mouseDoubleClick(QMouseEvent*)),
                     this,
                     SLOT(on_pictureDoubleClick()));
}

PhotoViewer::~PhotoViewer()
{
    delete ui;
    delete _currentDir;
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
    // TODO: Use QMimeDatabase to get only supported files
    QString fileName;

    fileName = _currentDir->absoluteFilePath(_currentDir->entryList()[_currentFile]);
    ui->gvPicture->setPicture(fileName);
    if (!ui->gvPicture->hasPicture()) {
        QMessageBox::information(this,
                                 tr("Image Viewer"),
                                 tr("Cannot load %1.").arg(fileName));
    }
    else {
        ui->gvPicture->showPicture ();
    }
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

    if (this->ui->gvPicture->hasPicture()) {
        this->ui->gvPicture->showPicture();
    }
}

