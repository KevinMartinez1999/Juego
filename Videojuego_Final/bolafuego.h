#ifndef BOLAFUEGO_H
#define BOLAFUEGO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QBrush>
#include <QTimer>
#include <QMediaPlayer>
#include "jugadorbatalla.h"

class bolaFuego : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit bolaFuego(QObject *parent = nullptr,
                       short int estado = 2,
                       short int tipo = 1);

    bool colision(JugadorBatalla * obj);

    QGraphicsRectItem box;
    double Ax, Ay, Vx, Vy, X, Y, t, angulo;
    double r, w, m, masa = 5;

    QPixmap Pixmap;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

public slots:
    void move1();
    void move2();
    void move3();
    void move4();
    void move5();
    void colision_con_boss();
    void colision_con_jugador();
    void Actualizacion();

private:
    QTimer timer, animacion;
    short int ultimoEstado, Tipo;
    float columnas,ancho,alto, limiteSprite, dano;
};

#endif // BOLAFUEGO_H
