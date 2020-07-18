#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QTimer>
#include <cmath>
#include "jugador.h"

class Enemigo : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit Enemigo(QObject *parent = nullptr);

    bool verificar_golpe(Jugador *obj);

signals:

public slots:
    void ataque_enemigo();
    void detectar_enemigos();
    void ataque_jugador();
    void move();

private:
    QTimer at_enemigo, at_jugador;
    int vida;
};

#endif // ENEMIGO_H
