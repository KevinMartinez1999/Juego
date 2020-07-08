 #include "jugador.h"
#include "mapa_gameplay.h"
extern int Fila;
extern Muro *muro;
Jugador::Jugador(QObject *parent) : QObject(parent)
{
    banLeft = false;
    banRight = false;
    banUp = false;
    banDown = false;

    QTimer *timer1 = new QTimer;
    connect(timer1, SIGNAL(timeout()), this, SLOT(moveLeft()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(moveRight()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(moveUp()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(moveDown()));
    timer1->start(25);

    timer = new QTimer();
    columnas = 0;
    pixmap = new QPixmap(":/Imagenes/SpritePlayer.png");
    ancho = 84;
    alto  = 84;
    connect(timer,SIGNAL(timeout()),this,SLOT(Actualizacion()));
    timer->start(100);


}

QRectF Jugador::boundingRect() const
{
    return QRectF(-ancho/2,-alto/2,ancho,alto);
}

void Jugador::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(-ancho/2,-alto/2,*pixmap,columnas,Fila,ancho,alto);
}

void Jugador::Actualizacion()
{
    columnas +=84;
    if(columnas >=336)
    {
        columnas =0;
    }
    this->update(-ancho/2,-alto/2,ancho,alto);
}

void Jugador::moveLeft()
{

    if (banLeft)
    {
        Fila = 420;
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
        Fila = 504;
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
        Fila = 588;
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
        Fila = 336;
        if(y()<2205){
            setPos(x(),y()+5);
            if(collidesWithItem(muro)){
                setPos(x(),y()-5);
            }
        }
    }
}
