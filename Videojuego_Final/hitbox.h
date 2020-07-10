#ifndef HITEBOX_H
#define HITEBOX_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QTimer>

class HitBox : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit HitBox(QObject *parent = nullptr);

signals:

};

#endif // HITEBOX_H
