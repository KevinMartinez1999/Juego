#ifndef BOSS_H
#define BOSS_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPainter>
#include <QGraphicsTextItem>
#include "jugadorbatalla.h"

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
    QGraphicsRectItem vida;
    int vida_real;
    int health;
    bool Boss_Derrotado=true;
signals:
private slots:
    void ataque_jugador();
    void Actualizacion();
private:
    QTimer at_jugador, timer;
    float columnas,fila,ancho,alto,limiteSprite;
    int tipoBoss;
};

#endif // BOSS_H
