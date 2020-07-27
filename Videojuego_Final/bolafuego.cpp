#include "bolafuego.h"
#include "boss.h"

#define g 9.81
#define masa 0.5
#define b 0.47

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

    //crear box
    box.setRect(0,0,20,20);

    //Definicion de los timers
    switch (Tipo) {
    case 1:
        connect(&timer, SIGNAL(timeout()), this, SLOT(move1()));
        connect(&timer, SIGNAL(timeout()), this, SLOT(colision_con_boss()));
        dano = 6;
        break;
    case 2:
        connect(&timer, SIGNAL(timeout()), this, SLOT(move2()));
        connect(&timer, SIGNAL(timeout()), this, SLOT(colision_con_jugador()));
        dano = 6;
        break;
    case 3:
        connect(&timer, SIGNAL(timeout()), this, SLOT(move3()));
        connect(&timer, SIGNAL(timeout()), this, SLOT(colision_con_jugador()));
        dano = 10;
        break;
    case 4:
        connect(&timer, SIGNAL(timeout()), this, SLOT(move4()));
        connect(&timer, SIGNAL(timeout()), this, SLOT(colision_con_jugador()));
        dano = 10;
        break;
    }
    timer.start(30);

    connect(&animacion, SIGNAL(timeout()), this, SLOT(Actualizacion()));
    animacion.start(150);


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
    m += 5;
    X = r*cos(w*t)+m;
    Y = r*sin(w*t);
    if (ultimoEstado == 2)
        setPos(x0+X, y0-Y);
    else
        setPos(x0-X, y0-Y);
    box.setPos(x()-10, y()-10);
    t += 0.03;
}

void bolaFuego::move2()
{
    Y = (masa*g/b)*(1 - exp(-t*b/masa));
    setPos(x(), y()+Y);
    box.setPos(x()-10, y()-10);
    t += 0.3;
    if (y() > 670)
        delete this;
}

void bolaFuego::move3()
{
    X = (masa*g/b)*(1 - exp(-t*b/masa));
    setPos(x()-X, y());
    box.setPos(x()-10, y()-10);
    t += 0.03;
    if (x() < -40)
        delete this;
}

void bolaFuego::move4()
{
    double X = (v0*sqrt(2)/2*t); // => cos(45°)
    double Y = (v0*sqrt(2)/2*t)-(0.5*9.81*pow(t,2)); // => sen(45°)
    setPos(x()-X,y()-Y);
    box.setPos(x()-10, y()-10);
    t += 0.03;
}

void bolaFuego::colision_con_boss()
{
    if (abs(x() - boss->x()) < 50){
        boss->health -= dano;
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

bool bolaFuego::colision(JugadorBatalla *obj)
{
    if (box.collidesWithItem(&obj->box)){
        if(obj->health>1)
            obj->JugadorAtacado->play();
        obj->health -= dano;
        obj->vida.setRect(0,0,obj->health,40);
        return true;
    }
    else return false;
}

void bolaFuego::Actualizacion()
{
    if (columnas >= 160)
        columnas = 0;
    else
        columnas += 40;
    this->update(-ancho/2,-alto/2,ancho,alto);
}


