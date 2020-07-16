#include "jugadorbatalla.h"
#include <QDebug>

JugadorBatalla::JugadorBatalla(QObject *parent) : QObject(parent)
{
    banLeft = false;
    banRight = false;

    QTimer *timer1 = new QTimer;
    connect(timer1, SIGNAL(timeout()), this, SLOT(moveLeft()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(moveRight()));
    timer1->start(30);

}

void JugadorBatalla::moveLeft()
{
    if (banLeft)
    {
        if(x()>0){
        setPos(x()-5,y());
        }
    }
}

void JugadorBatalla::moveRight()
{
    if (banRight)
    {
        if(x()<930){
        setPos(x()+5,y());
    }
    }
}
