#ifndef JUGADORBATALLA_H
#define JUGADORBATALLA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <cmath>

class JugadorBatalla: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit JugadorBatalla(QObject *parent = nullptr);
    inline bool setBanLeft() {return banLeft = true;}
    inline bool setBanRight() {return banRight = true;}
    inline bool resetBanLeft() {fila=0; return banLeft = false;}
    inline bool resetBanRight() {fila=168; return banRight = false;}

    void setX0(double X){x0=X;}
    void setY0(double Y){y0=Y;}
    double GetX0(){return x0;}
    double GetY0(){return y0;}

    QTimer *timer;
    QPixmap *pixmap;
    float columnas,fila,ancho,alto;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
public slots:
    void setX();
    void Actualizacion(); //Actualiza el sprite
    //Señales para el movimiento del jugador
    void moveLeft();
    void moveRight();

private:
    double x0,y0,xFinal,vx;
    bool banLeft;
    bool banRight;
};

#endif // JUGADORBATALLA_H
