#ifndef ANIMATEDITEMTEXT_H
#define ANIMATEDITEMTEXT_H

#include <QGraphicsTextItem>

class AnimatedItemText : public QGraphicsTextItem
{
    Q_OBJECT

    // Change picture position property for QPropertyAnimation
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:

signals:

public slots:

};

#endif // ANIMATEDITEMTEXT_H
