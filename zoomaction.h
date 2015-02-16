#ifndef ZOOMACTION_H
#define ZOOMACTION_H

#include <QWidgetAction>
#include <QSlider>
#include <QPixmap>

class ZoomAction : public QWidgetAction
{
    Q_OBJECT
public:
    explicit ZoomAction(QObject *parent = 0);

    qreal value ();
    void setValue (qreal value);

    qreal minimum ();
    void setMinimum (qreal min);

    qreal maximum ();
    void setMaximum (qreal max);

signals:
    void setZoom (qreal zoomPercent);

public slots:

private:
    QSlider *_slider;

    QWidget *createWidget (QWidget *parent);
};

#endif // ZOOMACTION_H
