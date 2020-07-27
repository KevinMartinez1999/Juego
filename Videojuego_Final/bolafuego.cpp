#include "bolafuego.h"
#include "boss.h"

#define g 9.81

extern Boss *boss;
extern JugadorBatalla *jugadorBatalla, *jugadorBatalla2;
extern short int num_jugadores;

bolaFuego::bolaFuego(QObject *parent, short int estado, short int tipo)
    : QObject(parent), ultimoEstado(estado), Tipo(tipo)
{
    //Inicializar constantes del mov
    X = 0.00;
    Y = 0.00;
    w = 25;
    r = 30;
    t = 0;
    m = 0;

    //Dimensiones del sprite
    columnas = 0;
    ancho = 40;
    alto = 40;

    //Definicion de los timers
    switch (Tipo) {
    case 1:
        connect(&timer, SIGNAL(timeout()), this, SLOT(move1()));
        connect(&timer, SIGNAL(timeout()), this, SLOT(colision_con_boss()));
        break;
    case 2:
        connect(&timer, SIGNAL(timeout()), this, SLOT(move2()));
        connect(&timer, SIGNAL(timeout()), this, SLOT(colision_con_jugador()));
        break;
    }
    timer.start(30);

    connect(&animacion, SIGNAL(timeout()), this, SLOT(Actualizacion()));
    animacion.start(150);


}

bool bolaFuego::colision(JugadorBatalla *obj)
{
    if (abs(int(x() - obj->x())) < 40 and obj->y() - y() < 50){
        if(obj->health>1)
            obj->JugadorAtacado->play();
        obj->health -= 5;
        obj->vida.setRect(0,0,obj->health,40);
        return true;
    }
    else return false;
}

QRectF bolaFuego::boundingRect() const
{
    return QRectF(-ancho/2,-alto/2,ancho,alto);
}

void bolaFuego::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-ancho/2,-alto/2,Pixmap,columnas,0,ancho,alto);
}

void bolaFuego::move1()
{
    t += 0.03;
    m += 5;
    X = r*cos(w*t)+m;
    Y = r*sin(w*t);
    if (ultimoEstado == 2)
        setPos(x0+X, y0-Y);
    else
        setPos(x0-X, y0-Y);
}

void bolaFuego::move2()
{
    t += 0.03;
    Y += g*t;
    setPos(x(), y0+Y);
    if (y() > 670)
        delete this;
}

void bolaFuego::colision_con_boss()
{
    if (abs(x() - boss->x()) < 50){
        boss->health -= 6;
        boss->vida.setRect(0,0,boss->health, 40);
        delete this;
    }
    else if (x() < -10 or x() > 1010)
        delete this;
}

void bolaFuego::colision_con_jugador()
{
    if (num_jugadores == 2){
        if(colision(jugadorBatalla) and colision(jugadorBatalla2))
            delete this;
        else if(colision(jugadorBatalla) or colision(jugadorBatalla2))
            delete this;
    }
    else
        if(colision(jugadorBatalla))
            delete this;
}

void bolaFuego::Actualizacion()
{
    if (columnas >= 160)
        columnas = 0;
    else
        columnas += 40;
    this->update(-ancho/2,-alto/2,ancho,alto);
}


