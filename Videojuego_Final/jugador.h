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
    inline bool setBanLeft() {return banLeft = true;}
    inline bool setBanRight() {return banRight = true;}
    inline bool setBanUp() {return banUp= true;}
    inline bool setBanDown() {return banDown = true;}
    inline bool resetBanLeft() {fila=168; return banLeft = false;}
    inline bool resetBanRight() {fila=252; return banRight = false;}
    inline bool resetBanUp() {fila=84; return banUp = false;}
    inline bool resetBanDown() {fila=0; return banDown = false;}
    inline bool setBanAttack() {return banAttack = true;}
    inline bool resetBanAttack() {return banAttack = false;}

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

private:
    bool banLeft;
    bool banRight;
    bool banUp;
    bool banDown;
    bool banAttack;
    short int ultimoEstado;
};

#endif // JUGADOR_H
