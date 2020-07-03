#include "jugador.h"

Jugador::Jugador(QObject *parent) : QObject(parent)
{
    banLeft = false;
    banRight = false;
    banUp = false;
    banDown = false;

    QTimer *timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(moveLeft()));
    connect(timer, SIGNAL(timeout()), this, SLOT(moveRight()));
    connect(timer, SIGNAL(timeout()), this, SLOT(moveUp()));
    connect(timer, SIGNAL(timeout()), this, SLOT(moveDown()));
    timer->start(25);
}

void Jugador::moveLeft()
{
    if (banRight)
    {
        setPos(x()+5,y());
    }
}

void Jugador::moveRight()
{
    if (banLeft)
    {
        setPos(x()-5,y());
    }
}

void Jugador::moveUp()
{
    if (banUp)
    {
        setPos(x(),y()-5);
    }
}

void Jugador::moveDown()
{
    if (banDown)
    {
        setPos(x(),y()+5);
    }
}
