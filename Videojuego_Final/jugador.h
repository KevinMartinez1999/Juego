#ifndef JUGADOR_H
#define JUGADOR_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QMediaPlayer>
#include <QPainter>
#include <cmath>

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

    void reset_golpe();
    bool golpe_izq = false;
    bool golpe_der = false;
    bool golpe_arr = false;
    bool golpe_aba = false;
    bool banAttack;

    QTimer timer, timer1;

    QGraphicsRectItem box, vida;
    int health;
    bool muerto;

    void PararTimers();
    void ReiniciarTimers();

    //Los sig. metodos y atributos son los necesarios para crear la animacion de
    //movimiento del jugador
    QPixmap *pixmap;
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
    QMediaPlayer *Ataque;
    bool banLeft;
    bool banRight;
    bool banUp;
    bool banDown;
    short int ultimoEstado, cont = 0;
    QPoint posAnterior;
    float columnas,fila,ancho,alto;
};

#endif // JUGADOR_H
