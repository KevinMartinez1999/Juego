#include "boss.h"
#include "niveles.h"
#include "bolafuego.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <QList>

#define pi 3.14159265
#define g 9.81
#define e 0.0375

QList<bolaFuego *> bolas;

extern short int num_jugadores;
extern JugadorBatalla *jugadorBatalla, *jugadorBatalla2;

Boss::Boss(QObject *parent,int tipo) : QObject(parent), tipoBoss(tipo)
{
    //Inicializacion de las variables del boss
    fila = 0;
    columnas = 0;
    health = 160;
    Boss_Derrotado=false;
    vida_real = 0;
    tipoAtaque = 0;

    //Se inicializa la posicion del Boss, su Sprite y tiempo de ataque segun la variable tipoBoss
    //Para cada valor de tipoBoss cambian todas las variables
    if(tipoBoss==0){
        tiempo_ataque = 20000;
        ancho = 256;
        alto  = 220;
        limiteSprite=1536;
        pixmap = new QPixmap(":/Imagenes/BOSS4.png");
        setPos(890,450);
        connect(&timer,SIGNAL(timeout()),this,SLOT(AnimacionMuerte()));
    }
    else if(tipoBoss==1){
        tiempo_ataque = 15000;
        ancho = 192;
        alto  = 224;
        limiteSprite=1344;
        pixmap = new QPixmap(":/Imagenes/BOSS2.png");
        setPos(900,490);
//        connect(&generar_ataque, SIGNAL(timeout()), this, SLOT(orbitas()));
    }
    else if(tipoBoss==2){
        tiempo_ataque = 10000;
        ancho = 165;
        alto  = 201;
        limiteSprite=825;
        pixmap = new QPixmap(":/Imagenes/BOSS3.png");
        setPos(875,363);
    }
    else if(tipoBoss==3){
        tiempo_ataque = 5000;
        ancho = 320;
        alto  = 288;
        limiteSprite=1600;
        pixmap = new QPixmap(":/Imagenes/BOSS1.png");
        setPos(875,450);
    }

    //Se añade la barra de vida
    vida.setRect(0,0,health,40);
    vida.setTransformOriginPoint(health/2,40/2);
    vida.setRotation(180);
    vida.setBrush(Qt::red);

    //Timers
    connect(&timer,SIGNAL(timeout()),this,SLOT(Actualizacion()));
    timer.start(100);

    connect(&at_jugador, SIGNAL(timeout()), this, SLOT(ataque_jugador()));
    at_jugador.start(450);

    connect(&ataques, SIGNAL(timeout()), this, SLOT(cambiar_ataque()));
    ataques.start(tiempo_ataque);

    connect(&generar_ataque, SIGNAL(timeout()), this, SLOT(elegir_ataque()));
    generar_ataque.start(1000);
}

//Verifica el lado por el que golpea el jugador
/*Si el jugador golpea hacia la izquierda y el enemigo esta a la derecha de el entonces
 el ataque no debe hacer ningun efecto sobre el enemigo, esto es lo que hace esta funcón*/
bool Boss::verificar_golpe(JugadorBatalla *obj)
{
    if (obj->golpe_izq and this->x() < obj->x())
        return true;
    else if (obj->golpe_der and this->x() > obj->x())
        return true;
    else
        return false;
}

QRectF Boss::boundingRect() const
{
    /*La funcion propia de qt bodingRect, crea y retorna el rectangulo que conforma la figura del jugador y en el cual pintaremos nuestra
    imagen. -ancho/2,-alto/2 permite que el punto de origen del rectangulo siempre sea le centro de la imagen y no la esquina superior.*/
    return QRectF(-ancho/2,-alto/2,ancho,alto);
}

void Boss::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void Boss::PararTimers()
{
    timer.stop();
    at_jugador.stop();
    ataques.stop();
    generar_ataque.stop();
}

void Boss::ReiniciarTimers()
{
    at_jugador.start(450);
    timer.start(100);
    ataques.start(tiempo_ataque);
    generar_ataque.start(1000);
}

void Boss::AnimacionMuerte()
{
    if(Boss_Derrotado){
        if(fila!=220){
            columnas=0;
        }
        fila=220;
        if(columnas==limiteSprite){
            hide();
            PararTimers();}
        else return;
    }
    else return;
}

void Boss::ataque_jugador()
{
    if (health <= 1)
        Boss_Derrotado = true;

    if (abs(this->x()-jugadorBatalla->x()) < 184 and jugadorBatalla->banAttack){
        if (verificar_golpe(jugadorBatalla)){
            vida_real++;

            /*este switch se usa para, dependiendo del Boss, los ataques tengan cada vez menos efecto sobre
              los Bosses dando la sensacion de dificultad*/
            switch (tipoBoss) {
            case 0:
                if (vida_real == 1){
                    health -= 2;
                    vida_real = 0;
                }
                break;
            case 1:
                if (vida_real == 2){
                    health -= 2;
                    vida_real = 0;
                }
                break;
            case 2:
                if (vida_real == 3){
                    health -= 2;
                    vida_real = 0;
                }
                break;
            case 3:
                if (vida_real == 4){
                    health -= 2;
                    vida_real = 0;
                }
                break;
            default:
                break;
            }
            vida.setRect(0,0,health,40);
        }
    }

    if (num_jugadores == 2){ //En caso de tener dos jugadores
        if (abs(this->x()-jugadorBatalla2->x()) < 184 and jugadorBatalla2->banAttack){
            if (verificar_golpe(jugadorBatalla2)){
                health -= 2;
                vida.setRect(0,0,health,40);
            }
        }
    }
}

//Actualizacion del sprite
void Boss::Actualizacion()
{
    /*La imagen sprite del jugador es una imagen que estaba dividida por filas y por columnas, cada fila determina un movimiento o
    accion diferente hecha por el jugador, y las columnas son frames que permiten que esa accion se vea con movimiento, entonces mediante
    un timer estaremos constantemente interactuando en las columnas de determinada fila para asi ir generando una animacion fluida y
    continua.*/
    if(columnas >= limiteSprite)
    {
        columnas = 0;
    }
    else{
        columnas += ancho;
    }
    this->update(-ancho/2,-alto/2,ancho,alto);/*La funcion update constantemente actualiza el boundingRect del jugador para que su
    origen siempre sea la mitad de la imagen actual.*/
}

/*Aqui el boss elige un ataque y lo lanza por cierta cantidad de tiempo hasta que lo cambia
  por otro. La variable tipo ataque es la que hace posible esto, cuando tipoAtaque cambia el
  switch de esta funcion hace que el boss haga un ataque nuevo*/
void Boss::elegir_ataque()
{
    switch (tipoAtaque) {
    case 0:{
        bolaFuego * bola = new bolaFuego(this, 1, 2);
        if(tipoBoss==0 or tipoBoss==4)
            bola->Pixmap = QPixmap(":/Imagenes/FUEGO2.png");
        else
            bola->Pixmap = QPixmap(":/Imagenes/FUEGO.png");
        int x = 1+(rand()%1000), y = 0;
        bola->X = x;
        bola->Y = y;
        bola->setPos(bola->X, bola->Y);
        scene()->addItem(bola);
        bola->box.setPos(bola->x()-5, bola->y()-5);
        break;
    }
    case 1:{
        bolaFuego * bola = new bolaFuego(this, 1, 3);
        bola->Pixmap = QPixmap(":/Imagenes/BOLAFUEGO.png");
        bola->X = x() - 20;
        bola->Y = y() + 60;
        bola->Vx = 1;
        bola->Vy = 1;
        bola->Ax = 0.8;
        bola->Ay = 0.8;
        bola->setPos(bola->X, bola->Y);
        scene()->addItem(bola);
        bola->box.setPos(bola->x()-5, bola->y()-5);
        break;
    }
    case 2:
        bolaFuego * bola = new bolaFuego(this, 1, 4);
        bola->Pixmap = QPixmap(":/Imagenes/BOLAFUEGO.png");
        bola->X = x() - 20;
        bola->Y = y();
        double V = pow(0.03*g*(x() - jugadorBatalla->x()),0.5);
        bola->Vx = V*cos(pi/4);
        bola->Vy = V*sin(pi/4);
        bola->setPos(bola->X, bola->Y);
        scene()->addItem(bola);
        bola->box.setPos(bola->x()-5, bola->y()-5);
        break;
    }
}

/*Aqui se elige cada cierto tiempo un ataque nuevo para que el Boss lo haga.
  el tiempo esta determinado por un timer que se repite cada cierto tiempo y ese tiempo
  depende del Boss contra el que estamos peleando. Así, cada vez los ataques son mas impredecibles
  y dificiles de esquivar para el jugador*/
void Boss::cambiar_ataque()
{
    tipoAtaque++;
    if (tipoAtaque > 0 and tipoBoss==0)
        tipoAtaque = 0;
    else if(tipoAtaque>1 and tipoBoss==1)
        tipoAtaque = 0;
    else if(tipoAtaque>2 and tipoBoss==2)
        tipoAtaque=0;
    else if (tipoAtaque>3 and tipoBoss>2)
        tipoAtaque=0;

    //Aqui se le da a acada ataque un tiempo de repeticion entre
    //un lanzamiento y otro
    switch (tipoAtaque) {
    case 0:
        generar_ataque.stop();
        generar_ataque.start(1000);
        break;
    case 1:
        generar_ataque.stop();
        generar_ataque.start(2000);
        break;
    case 2:
        generar_ataque.stop();
        generar_ataque.start(4000);
        break;
    case 3:
        generar_ataque.stop();
        emit orbitas();
        break;
    }
}

/*Aqui hay una habilidad especial que consiste en unas bolas que orbitan al rededor
  del enemigo y lo protegen de ser golpeado*/
void Boss::orbitas()
{
    if (bolas.count() == 4)
        return;

    bolaFuego * bola = new bolaFuego(this, 1, 5);
    bola->Pixmap = QPixmap(":/Imagenes/BOLAFUEGO.png");
    switch (cont) {
    case 0:
        bola->X = x();
        bola->Y = y() - 4000*e;
        bola->Vx = -3.2;
        bola->Vy = 0;
        bola->masa = 70*e;
        cont++;
        break;
    case 1:
        bola->X = x();
        bola->Y = y() + 4000*e;
        bola->Vx = 3.2;
        bola->Vy = 0;
        bola->masa = 70*e;
        cont++;
        break;
    case 2:
        bola->X = x() + 4000*e;
        bola->Y = y();
        bola->Vx = 0;
        bola->Vy = -3.2;
        bola->masa = 70*e;
        cont++;
        break;
    case 3:
        bola->X = x() - 4000*e;
        bola->Y = y();
        bola->Vx = 0;
        bola->Vy = 3.2;
        bola->masa = 70*e;
        cont = 0;
        break;
    }
    scene()->addItem(bola);
    bola->box.setPos(bola->x(), bola->y());
    bolas.append(bola);
}
