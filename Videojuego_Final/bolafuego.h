#ifndef BOLAFUEGO_H
#define BOLAFUEGO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QBrush>
#include <QTimer>
#include <QMediaPlayer>
#include "jugadorbatalla.h"

/*Esta clase genera unas bolas de fuego que lanzan los enemigos y tambien el jugador;
dependiendo de que tipo de bola se elija hace un movimiento distinto, entre los movimientos de
estas bolas estan las orbitas, caida libre, tiro parabolico y otros; las bolas tambien tienen
los coeficientes de restitucion, friccion con el suelo y rozamiento con el aire*/

class bolaFuego : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit bolaFuego(QObject *parent = nullptr,
                       short int estado = 2,
                       short int tipo = 1,
                       short int suelo=0);

    bool colision(JugadorBatalla * obj);

    QGraphicsRectItem box;

    //Aqui las variables fisicas de los objetos
    double Ax, Ay, Vx, Vy, X, Y, t, angulo;
    double r, w, m, masa;

    //Las funciones que hacen posible las animaciones ¿
    QPixmap Pixmap;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

public slots:
    //Movimiento circular de las bolas que lanza el enemigo
    void move1();

    //Movimiento en caida libre con friccion del aire
    void move2();

    //Movimiento acelerado de las bolas del enemigo
    void move3();

    //Movimiento parabolico de las bolas que lanza el enemigo
    void move4();

    //Movimiento planetario de las bolas que orbitan al Boss
    void move5();

    //Verifica cuando colisiona con el Boss
    void colision_con_boss();

    //Verifica cuando colisiona con el Jugador
    void colision_con_jugador();

    //Actualizacion de Sprite
    void Actualizacion();

private:
    //Timers para las verificaciones de colisiones y actualizaciones de sprite
    QTimer timer, animacion;

    //Vatiables para los movimentos fisicos y para seleccionar el tipo de ataque
    short int ultimoEstado, Tipo, SueloNivel;

    //Variables que manejan el sprite y el daño de los ataques
    float columnas,ancho,alto, limiteSprite, dano;
};

#endif // BOLAFUEGO_H
