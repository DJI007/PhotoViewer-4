#ifndef ZOOMACTION_H
#define ZOOMACTION_H

#include <QWidgetAction>
#include <QSlider>
#include <QToolButton>
#include <QPixmap>
#include <QLabel>

class ZoomAction : public QWidgetAction
{
    Q_OBJECT
public:
    explicit ZoomAction(QWidget *parent = 0);

    qreal value ();
    void setValue (qreal value);

    qreal minimum ();
    void setMinimum (qreal min);

    qreal maximum ();
    void setMaximum (qreal max);

signals:
    void zoomChanged (qreal zoomPercent);

public slots:

private slots:
    void onZoomInClicked ();
    void onZoomOutClicked ();
    void onZoomValueChanged (int value);

private:
    QSlider *_slider;
    QToolButton *_zoomIn;
    QToolButton *_zoomOut;
    QLabel *_percent;

    QWidget *createWidget (QWidget *parent);
};

#endif // ZOOMACTION_H
