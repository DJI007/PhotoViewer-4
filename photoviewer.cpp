#include "photoviewer.h"
#include "ui_photoviewer.h"

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
