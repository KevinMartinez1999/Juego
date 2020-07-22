#ifndef MURO_H
#define MURO_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Muro : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Muro(QObject *parent = nullptr);

signals:

};

#endif // MURO_H
