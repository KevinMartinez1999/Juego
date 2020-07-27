#ifndef JUGADORBATALLA_H
#define JUGADORBATALLA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <cmath>
#include <QMediaPlayer>

class JugadorBatalla: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit JugadorBatalla(QObject *parent = nullptr);
    inline void setBanLeft() {banLeft = true;}
    inline void setBanRight() {banRight = true;}
    inline void resetBanLeft() {banLeft = false;}
    inline void resetBanRight() {banRight = false;}
    inline void setBanAttack() {banAttack = true;}
    inline void resetBanAttack() {banAttack = false;}
    inline void setBanSpell(){banSpell = true;}
    inline void resetBanSpell(){banSpell = false;}
    inline void setBanJump(){banJump = true;}

    void setX0(double X){x0=X;}
    void setY0(double Y){y0=Y;}
    double GetX0(){return x0;}
    double GetY0(){return y0;}

    void PararTimers();
    void ReiniciarTimers();

    void reset_golpe();
    bool golpe_izq = false;
    bool golpe_der = false;

    bool banAttack;
    bool muerto;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QPixmap *pixmap;
    QGraphicsRectItem vida, box;
    QMediaPlayer *JugadorAtacado;
    int health;

signals:
public slots:
    void setX();
    void Actualizacion(); //Actualiza el sprite
    void moveLeft();//Señales para el movimiento del jugador
    void moveRight();
    void Jump();
    void Attack();
    void Spell();
    void pos();
    void tiempo(){TiempoHechizo=true;}
private:
    float columnas,fila,ancho,alto;
    double x0,y0,xFinal,vx,t,tsalto;
    bool TiempoHechizo;
    bool banJump;
    bool banLeft;
    bool banRight;
    bool banSpell;
    short int ultimoEstado;
    QMediaPlayer *Hechizo;
    QMediaPlayer *Ataque;
    QPoint posAnterior;
    QTimer mov, timer;
};

#endif // JUGADORBATALLA_H
