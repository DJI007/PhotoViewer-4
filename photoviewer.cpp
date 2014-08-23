#include "photoviewer.h"
#include "ui_photoviewer.h"

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
}

PhotoViewer::~PhotoViewer()
{
    delete ui;
}
