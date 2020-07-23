#include "bolafuego.h"
#include "boss.h"

extern Boss *boss;

bolaFuego::bolaFuego(QObject *parent, int estado) : QObject(parent), ultimoEstado(estado)
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
    pixmap = new QPixmap(":/Imagenes/BOLAFUEGO.png");

    //Definicion de los timers
    connect(&timer, SIGNAL(timeout()), this, SLOT(move()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(colision_con_boss()));
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
    painter->drawPixmap(-ancho/2,-alto/2,*pixmap,columnas,0,ancho,alto);
}

void bolaFuego::move()
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

void bolaFuego::Actualizacion()
{
    if (columnas >= 160)
        columnas = 0;
    else
        columnas += 40;
    this->update(-ancho/2,-alto/2,ancho,alto);
}


