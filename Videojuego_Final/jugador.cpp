#include "jugador.h"
extern Muro *muro;
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

    if (banLeft)
    { 
        if(x()>0){
            setPos(x()-5,y());
            if(collidesWithItem(muro)){
                setPos(x()+5,y());
            }
        }
    }
}

void Jugador::moveRight()
{
    if (banRight)
    {
        if(x()<2209){
            setPos(x()+5,y());
            if(collidesWithItem(muro)){
                setPos(x()-5,y());
            }
        }
    }
}

void Jugador::moveUp()
{
    if (banUp)
    {
        if(y()>0){
            setPos(x(),y()-5);
            if(collidesWithItem(muro)){
                setPos(x(),y()+5);
            }
        }
    }
}

void Jugador::moveDown()
{
    if (banDown)
    {
        if(y()<2205){
            setPos(x(),y()+5);
            if(collidesWithItem(muro)){
                setPos(x(),y()-5);
            }
        }
    }
}
