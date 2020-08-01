#ifndef BOSS_H
#define BOSS_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPainter>
#include <QGraphicsTextItem>
#include "jugadorbatalla.h"
#include "bolafuego.h"

class Boss: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Boss(QObject *parent = nullptr,int tipo=0);
    bool verificar_golpe(JugadorBatalla *obj);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void PararTimers();
    void ReiniciarTimers();

    QPixmap *pixmap;

    int tipoBoss;
    //Variables que serviran para el manejo de la vida y muerte del boss
    int vida_real;
    int health;
    bool Boss_Derrotado=true;
    QGraphicsRectItem vida;

signals:
private slots:
    void ataque_jugador();
    void Actualizacion();
    void AnimacionMuerte();
    void MovimientoBoss1();
    void MovimientoBoss3();
    void elegir_ataque();
    void cambiar_ataque();
    void orbitas();

private:
    //Timers destinados para las determinadas funciones del bosss
    QTimer at_jugador, timer, ataques, generar_ataque, Movimiento;

    QList<bolaFuego *> bolas;

    //Variables para el control del sprite del boss
    float columnas,fila,ancho,alto,limiteSprite;

    //Variables para el sistema de seleccion de ataque de los distintos bosses
    short int tipoAtaque, tiempo_ataque, cont = 0;

    //Variables para el movimiento Circular Uniforme del Boss 1
    double r, w, m, t, X, Y;
};

#endif // BOSS_H
