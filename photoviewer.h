#ifndef PHOTOVIEWER_H
#define PHOTOVIEWER_H

#include <QMainWindow>

namespace Ui {
class PhotoViewer;
}

class PhotoViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit PhotoViewer(QWidget *parent = 0);
    ~PhotoViewer();

private:
    Ui::PhotoViewer *ui;
};

#endif // PHOTOVIEWER_H
