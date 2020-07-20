#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QTimer>
#include <cmath>
#include <QDebug>
#include "jugador.h"

class Enemigo : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit Enemigo(QObject *parent = nullptr);

    bool verificar_golpe(Jugador *obj);

    QGraphicsRectItem vida;

signals:

public slots:
    void ataque_enemigo();
    void detectar_enemigos();
    void ataque_jugador();

private:
    QTimer at_enemigo, at_jugador;
    int health;
};

#endif // ENEMIGO_H
