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
    inline bool resetBanLeft() {return banLeft = false;}
    inline bool resetBanRight() {return banRight = false;}
    inline void setBanAttack() {banAttack = true;}
    inline void resetBanAttack() {banAttack = false;}

    void setX0(double X){x0=X;}
    void setY0(double Y){y0=Y;}
    double GetX0(){return x0;}
    double GetY0(){return y0;}

    void reset_golpe();
    bool golpe_izq = false;
    bool golpe_der = false;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QTimer *timer;
    QPixmap *pixmap;
    QGraphicsRectItem vida;
    int health;

signals:
public slots:
    void setX();
    void Actualizacion(); //Actualiza el sprite
    //Señales para el movimiento del jugador
    void moveLeft();
    void moveRight();
    void Attack();
    void pos();

private:
    float columnas,fila,ancho,alto;
    double x0,y0,xFinal,vx;
    bool banLeft;
    bool banRight;

    bool banAttack;
    short int ultimoEstado;
    QPoint posAnterior;
    QTimer timer1;
};

#endif // JUGADORBATALLA_H
