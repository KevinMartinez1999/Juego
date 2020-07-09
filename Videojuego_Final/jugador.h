#ifndef JUGADOR_H
#define JUGADOR_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPainter>
#include "muro.h"
#include "hitebox.h"

class Jugador : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Jugador(QObject *parent = nullptr);

    // Estas funciones son las banderas de movimiento

    inline bool setBanLeft() {return banLeft = true;}
    inline bool setBanRight() {return banRight = true;}
    inline bool setBanUp() {return banUp= true;}
    inline bool setBanDown() {return banDown = true;}
    inline bool resetBanLeft() {return banLeft = false;}
    inline bool resetBanRight() {return banRight = false;}
    inline bool resetBanUp() {return banUp = false;}
    inline bool resetBanDown() {return banDown = false;}

    HiteBox *box;
    void crear_hiteBox(); //Crea el HiteBox del jugador que sigue sus pies para las colisiones

    //Los sig. metodos y atributos son los necesarios para crear la animacion de
    //movimiento del jugador

    QTimer *timer;
    QPixmap *pixmap;
    float columnas,ancho,alto;
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

private:
    bool banLeft;
    bool banRight;
    bool banUp;
    bool banDown;
};

#endif // JUGADOR_H
