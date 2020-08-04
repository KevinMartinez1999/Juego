#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QBrush>
#include <QTimer>
#include <cmath>
#include "jugador.h"

/*Es clase es para los enemigos que aparecen por el mapa principal*/

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
    //Sirve para eliminar los enemigos que ya no son necesarios en el mapa
    void EliminarDeLista(Enemigo *, bool);

public slots:
    void Actualizacion(); //Actualiza el sprite
    void sonidos();
    void ataque_enemigo();
    void detectar_enemigos();
    void ataque_jugador();
    void muerte(); //Se verifica cuando muere el jugador
    void move(); //Movimiento del enemigo que sigue al jugador

private:
    QMediaPlayer JugadorAtacado, JugadorMuerto, fantasma;
    QTimer at_enemigo, at_jugador, mov_enemigo, timer, SonidosTimer;
    float columnas,fila,ancho,alto;
    int health;
};

#endif // ENEMIGO_H
