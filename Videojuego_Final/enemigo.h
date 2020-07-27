#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QBrush>
#include <QTimer>
#include <cmath>
#include "jugador.h"

class Enemigo : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Enemigo(QObject *parent = nullptr);

    bool verificar_golpe(Jugador *obj);
    void follow(Jugador *obj);

    void PararTimers();
    void ReiniciarTimers();

    QGraphicsRectItem vida, box;

    //Los sig. metodos y atributos son los necesarios para crear la animacion de
    //movimiento del jugador
    QPixmap pixmap;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

public slots:
    void Actualizacion(); //Actualiza el sprite
    void sonidos();

    void ataque_enemigo();
    void detectar_enemigos();
    void ataque_jugador();
    void muerte();
    void move();

private:
    float columnas,fila,ancho,alto;
    QTimer at_enemigo, at_jugador, mov_enemigo, timer, SonidosTimer;
    int health;

    QMediaPlayer *JugadorAtacado;
    QMediaPlayer *JugadorMuerto;
    QMediaPlayer *fantasma;
};

#endif // ENEMIGO_H
