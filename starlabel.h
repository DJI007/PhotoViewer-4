#ifndef STARLABEL_H
#define STARLABEL_H

#include <QLabel>

class StarLabel : public QLabel
{
    Q_OBJECT
public:
    explicit StarLabel(QWidget *parent = 0);
    ~StarLabel ();

    bool eventFilter(QObject *, QEvent *);

    void setOn ();
    void setOff ();

signals:
    void mouseEnter (StarLabel *sender);
    void mouseLeave (StarLabel *sender);

public slots:

private:
    QPixmap _pixmapStarOn;
    QPixmap _pixmapStarOff;
};

#endif // STARLABEL_H
