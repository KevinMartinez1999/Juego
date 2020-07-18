#ifndef JUGADOR_H
#define JUGADOR_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPainter>
#include "muro.h"
#include "hitbox.h"

class Jugador : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Jugador(QObject *parent = nullptr);

    // Estas funciones son las banderas de movimiento
    inline void setBanLeft() {banLeft = true;}
    inline void setBanRight() {banRight = true;}
    inline void setBanUp() {banUp= true;}
    inline void setBanDown() {banDown = true;}
    inline void setBanAttack() {banAttack = true;}

    inline void resetBanLeft() {banLeft = false;}
    inline void resetBanRight() {banRight = false;}
    inline void resetBanUp() {banUp = false;}
    inline void resetBanDown() {banDown = false;}
    inline void resetBanAttack() {banAttack = false;}

    HitBox *box;
    void crear_hitBox(); //Crea el HiteBox del jugador que sigue sus pies para las colisiones

    //Los sig. metodos y atributos son los necesarios para crear la animacion de
    //movimiento del jugador

    QTimer *timer;
    QPixmap *pixmap;
    float columnas,fila,ancho,alto;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

public slots:
    void Actualizacion(); //Actualiza el sprite

    //Señales para el movimiento del jugador

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void Attack();
    void pos();

private:
    bool banLeft;
    bool banRight;
    bool banUp;
    bool banDown;
    bool banAttack;
    short int ultimoEstado;
    QPoint posAnterior;
};

#endif // JUGADOR_H
