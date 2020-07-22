#include "jugadorbatalla.h"
#include <QDebug>
#define dt 0.03

JugadorBatalla::JugadorBatalla(QObject *parent) : QObject(parent)
{
    vx=5;
    xFinal=x0;

    banLeft = false;
    banRight = false;
    banAttack = false;
    banSpell = false;
    TiempoHechizo=true;
    ultimoEstado = 1;
    posAnterior = QPoint(0,0);

    health=120;

    //Timers para los slots de movimiento y fisicas del jugador
    QTimer *timer1 = new QTimer;
    connect(timer1, SIGNAL(timeout()), this, SLOT(moveLeft()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(moveRight()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(setX()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(Attack()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(Spell()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(pos()));
    timer1->start(30);

    //Timer para las actualización y dibujo del sprite.
    timer = new QTimer(this);
    columnas = 0;
    fila = 0;
    //Ancho y alto del sprite del jugador
    ancho = 168;
    alto  = 168;
    /*Este timer nos permitira la constante actualizacion de la imagen de nuestro jugador*/
    connect(timer,SIGNAL(timeout()),this,SLOT(Actualizacion()));
    timer->start(150);
    //Barra de vida del jugador
    vida.setRect(0,0,health,40);
    vida.setBrush(Qt::red);

    Hechizo = new QMediaPlayer(this);
    Hechizo->setMedia(QUrl("qrc:/Musica/FUEGO.wav"));
    Hechizo->setVolume(100);

}

void JugadorBatalla::reset_golpe()
{
    golpe_izq = false;
    golpe_der = false;
}

QRectF JugadorBatalla::boundingRect() const
{
    /*La funcion propia de qt bodingRect, crea y retorna el rectangulo que conforma la figura del jugador y en el cual pintaremos nuestra
    imagen. -ancho/2,-alto/2 permite que el punto de origen del rectangulo siempre sea le centro de la imagen y no la esquina superior.*/
    return QRectF(-ancho/2,-alto/2,ancho,alto);
}

void JugadorBatalla::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /*Funcion propia de Qt que nos permite dibujar dentro del boundingRect la imagen que queramos, para hacer esto debemos introducirle
    al drawPixmap algunos datos como el punto en el que queremos que empiece a dibujar, el pixmap a dibujar, el ancho y alto de
    lo que se dibujara y tambien, se le pasara constantemente en la funcion actualizar la columna que representara el cuadro 84x84
    que se dibujara en el momento. La variable fila representa el grupo de frames que se quiere realizar dependiendo a las acciones del
    jugador, esta variable cambia cuando el usuario activa un KeyEvent.*/
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-ancho/2,-alto/2,*pixmap,columnas,fila,ancho,alto);
}

void JugadorBatalla::setX(){
    if(xFinal>=5){
        return;
    }
    xFinal+=(vx*dt)-(0.5*pow(dt,2));
    if(fila==0 or fila==168) xFinal=0;
}

void JugadorBatalla::Actualizacion()
{
    /*La imagen sprite del jugador es una imagen que estaba dividida por filas y por columnas, cada fila determina un movimiento o
    accion diferente hecha por el jugador, y las columnas son frames que permiten que esa accion se vea con movimiento, entonces mediante
    un timer estaremos constantemente interactuando en las columnas de determinada fila para asi ir generando una animacion fluida y
    continua.*/
    if(columnas >= 504 or(fila == 672 and columnas >= 336)or(fila == 840 and columnas >= 336)or(fila>=1008 and columnas>=504))
    {
        columnas = 0;
    }
    else{
        columnas += 168;
    }
    this->update(-ancho/2,-alto/2,ancho,alto);/*La funcion update constantemente actualiza el boundingRect del jugador para que su
    origen siempre sea la mitad de la imagen actual.*/
}

void JugadorBatalla::moveLeft()
{
    if (banLeft)
    {
        reset_golpe();
        ultimoEstado = 1;
        if(fila!=336 and fila!=0) xFinal=0;
        fila=336;
        if(x()>42){
        setPos(x()-xFinal,y());
        }
    }
}

void JugadorBatalla::moveRight()
{
    if (banRight)
    {
        reset_golpe();
        ultimoEstado = 2;
        if(fila!=504 and fila!=168) xFinal=0;
        fila = 504;
        if(x()<930){
        setPos(x()+xFinal,y());
    }
    }
}

void JugadorBatalla::Attack()
{
    if (banAttack){
        if (fila != 672 and fila != 840)
            columnas = 0;
        switch (ultimoEstado) {
        case 1:
            golpe_izq = true;
            fila = 672;
            break;
        case 2:
            golpe_der = true;
            fila = 840;
            break;
        default:
            break;
        }
    }
}

void JugadorBatalla::Spell()
{
    if(TiempoHechizo){
    if(banSpell and !banAttack){
        if (fila != 1008 and fila != 1176)
            columnas = 0;
        switch (ultimoEstado) {
        case 1:
            fila = 1008;
            break;
        case 2:
            golpe_der = true;
            fila = 1176;
            break;
        default:
            break;
        }
        if (columnas==504){
            Hechizo->play();
            TiempoHechizo=false;
            QTimer::singleShot(5000,this,SLOT(tiempo()));
        }
    }
    }
}

void JugadorBatalla::pos()
{
    if (banAttack or banSpell) //Cuando este atacando no debe hacer la animacion de estar quieto
        return;
    if (posAnterior == QPoint(x(),y())){ //Si se cumple es porque el jugador está quieto
        switch (ultimoEstado) {
        case 1:
            fila=0;
            break;
        case 2:
            fila=168;
            break;
        default:
            break;
        }
    }
    posAnterior = QPoint(x(),y());
}
