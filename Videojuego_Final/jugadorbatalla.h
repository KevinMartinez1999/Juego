#ifndef JUGADORBATALLA_H
#define JUGADORBATALLA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QPainter>

class JugadorBatalla: public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit JugadorBatalla(QObject *parent = nullptr);
    inline bool setBanLeft() {return banLeft = true;}
    inline bool setBanRight() {return banRight = true;}
    inline bool resetBanLeft() {return banLeft = false;}
    inline bool resetBanRight() {return banRight = false;}

    QTimer *timer;
signals:

public slots:
    //Señales para el movimiento del jugador
    void moveLeft();
    void moveRight();

private:
    bool banLeft;
    bool banRight;
};

#endif // JUGADORBATALLA_H
