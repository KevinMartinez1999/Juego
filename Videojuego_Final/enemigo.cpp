#include "enemigo.h"
#include <QDebug>

#define paso 2.5

extern short int num_jugadores;
extern Jugador *jugador, *jugador2;
extern QList <Enemigo *> listaEnemigos;

Enemigo::Enemigo(QObject *parent) : QObject(parent)
{
    //Inicializando vida del enemigo
    health = 20;

    //Ancho y alto del sprite del jugador (inicialización de variables para el sprite)
    ancho = 64;
    alto  = 80;
    columnas = 0;
    fila = 0;
    pixmap = QPixmap(":/Imagenes/ENEMIGOS.png");

    //Barra de vida
    vida.setRect(0,0,health,5);
    vida.setBrush(Qt::blue);

    //Se crea el HitBox
    box.setRect(0,0,25,25);
    box.setPos(0,0);

    //Timers del enemigo
    connect(&at_enemigo, SIGNAL(timeout()), this, SLOT(ataque_enemigo()));
    at_enemigo.start(1000);

    //Timer para el ataque del enemigo
    connect(&at_jugador, SIGNAL(timeout()), this, SLOT(ataque_jugador()));
    at_jugador.start(600);

    //Timer para las actualización y dibujo del sprite.
    /*Este timer nos permitira la constante actualizacion de la imagen de nuestro jugador*/
    connect(&timer,SIGNAL(timeout()),this,SLOT(Actualizacion()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(detectar_enemigos()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(muerte()));
    timer.start(200);

    //Movimiento del enemigo
    connect(&mov_enemigo, SIGNAL(timeout()), this, SLOT(move()));
    mov_enemigo.start(60);

    //Sonidos
    JugadorAtacado.setMedia(QUrl("qrc:/Musica/PAIN.wav"));
    JugadorAtacado.setVolume(100);

    fantasma.setMedia(QUrl("qrc:/Musica/FANTASMA.wav"));
    fantasma.setVolume(100);

    connect(&SonidosTimer, SIGNAL(timeout()), this, SLOT(sonidos()));
    SonidosTimer.start(5000);

}

/*Verifica cuando los enemigos atacan al jugador por la espalda o de frente
 cuando se ataca de frente y su respectiva bandera esta activa entonces el enemigo muere
 pero si lo ataca por la espalda el jugador muere*/
bool Enemigo::verificar_golpe(Jugador *obj)
{
    if (obj->golpe_izq and x() <= obj->x())
        return true;
    else if (obj->golpe_der and x() >= obj->x())
        return true;
    else if (obj->golpe_arr and y() <= obj->y())
        return true;
    else if (obj->golpe_aba and y() >= obj->y())
        return true;
    else
        return false;
}

/*Dependiendo de la posicion del jugador los fantasmas van a seguirlo por todo el mapa*/
void Enemigo::follow(Jugador *obj)
{
    if (box.x() > obj->box.x() and box.y() > obj->box.y()){
        pixmap = QPixmap(":/Imagenes/ENEMIGOS.png");
        setPos(x()-paso, y()-paso);
    }
    else if  (box.x() > obj->box.x() and box.y() < obj->box.y()){
        pixmap = QPixmap(":/Imagenes/ENEMIGOS.png");
        setPos(x()-paso, y()+paso);
    }
    else if  (box.x() < obj->box.x() and box.y() > obj->box.y()){
        pixmap = QPixmap(":/Imagenes/ENEMIGOS2.png");
        setPos(x()+paso, y()-paso);
    }
    else if  (box.x() < obj->box.x() and box.y() < obj->box.y()){
        pixmap = QPixmap(":/Imagenes/ENEMIGOS2.png");
        setPos(x()+paso, y()+paso);
    }
    else if (box.x() > obj->box.x()){
        setPos(x()-paso, y());
    }
    else if (box.x() < obj->box.x()){
        setPos(x()+paso, y());
    }
    else if (box.y() > obj->box.y()){
        setPos(x(), y()-paso);
    }
    else if (box.y() < obj->box.y()){
        setPos(x(), y()+paso);
    }
    box.setPos(x()-15, y()+12);
    vida.setPos(x()-10, y()-30);
}

void Enemigo::ReiniciarTimers()
{
    mov_enemigo.start(60);
    at_enemigo.start(700);
    at_jugador.start(400);
    timer.start(200);
}

void Enemigo::PararTimers()
{
    mov_enemigo.stop();
    at_enemigo.stop();
    at_jugador.stop();
    timer.stop();
    SonidosTimer.stop();
}

QRectF Enemigo::boundingRect() const
{
    return QRectF(-ancho/2,-alto/2,ancho,alto);
}

void Enemigo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-ancho/2,-alto/2,pixmap,columnas,fila,ancho,alto);
}

void Enemigo::Actualizacion()
{
    /*La imagen sprite del jugador es una imagen que estaba dividida por filas y por columnas, cada fila
     determina un movimiento o accion diferente hecha por el jugador, y las columnas son frames que
     permiten que esa accion se vea con movimiento, entonces mediante un timer estaremos constantemente
     interactuando en las columnas de determinada fila para asi ir generando una animacion fluida y continua.*/
    if(columnas >= 384)
        //El archivo consta de 6 columnas de 84x84, cuando se llegue a la sexta columna se iniciara de nuevo
    {
        columnas = 0;
    }
    else{
        columnas += 64;
    }
    /*La funcion update constantemente actualiza el boundingRect del jugador para que su
     origen siempre sea la mitad de la imagen actual.*/
    this->update(-ancho/2,-alto/2,ancho,alto);
}

//Sonidos de los fantasmas mientras van por el jugador
void Enemigo::sonidos()
{
    if(num_jugadores==2){
        if(jugador->muerto and jugador2->muerto)
            fantasma.stop();
        else fantasma.play();
    }
    else{
        if(jugador->muerto)
            fantasma.stop();
        else
             fantasma.play();
    }
}

//Aqui los enemigos hacen daño cuando tocan al jugador
void Enemigo::ataque_enemigo()
{
    if (box.collidesWithItem(&jugador->box) and !jugador->muerto){
        jugador->health -= 5;
        jugador->vida.setRect(0,0,jugador->health,5);
        if(!jugador->muerto)
            JugadorAtacado.play();
    }
    else{
        //Si el jugador tiene menos de la mitad de la vida
        //a modo de ayuda la vida se le va a recuperar de a poco
        //mientras no sea atacado
        if (jugador->health < 25){
            jugador->health++;
            jugador->vida.setRect(0,0,jugador->health,5);
        }
    }
    if (num_jugadores == 2){
        if (box.collidesWithItem(&jugador2->box) and !jugador2->muerto){
            jugador2->health -= 5;
            jugador2->vida.setRect(0,0,jugador2->health,5);
            if(!jugador2->muerto)
                JugadorAtacado.play();;
        }
        else{
            if (jugador2->health <  25){
                jugador2->health++;
                jugador2->vida.setRect(0,0,jugador2->health,5);
            }
        }
    }
}

/*Cuando el enemigo esta muy lejos de la vista del jugador de elimina ya que ya
 no es necesario*/
void Enemigo::detectar_enemigos()
{
    if (num_jugadores == 2){
        if (jugador->muerto){
            if (abs(int(x()-jugador2->x())) > 900 or abs(int(y()-jugador2->y())) > 700){
                listaEnemigos.removeOne(this);
                delete this;
                return;
            }
        }
        else{
            if (abs(int(x()-jugador->x())) > 900 or abs(int(y()-jugador->y())) > 700){
                listaEnemigos.removeOne(this);
                delete this;
                return;
            }
        }
    }
    else{
        if (abs(int(x()-jugador->x())) > 900 or abs(int(y()-jugador->y())) > 700){
            listaEnemigos.removeOne(this);
            //qDebug()<<"Enemigo fuera de rango";
            delete this;
            return;
        }
    }
}

/*Aqui es la funcion de ataque del enemigo:
 si ataca de frente al jugador y el jugador esta atacando el enemigo muere
 pero si lo ataca por la espalda o por los laterales aunque el jugador esté
 atacando este muere; Esta verificación del ataque se hace en una función mas arriba*/
void Enemigo::ataque_jugador()
{
    if (box.collidesWithItem(&jugador->box) and jugador->banAttack){
        if (verificar_golpe(jugador)){
            health -= 5;
            vida.setRect(0,0,health,5);
        }
    }

    if (num_jugadores == 2){ //En caso de tener dos jugadores
        if (box.collidesWithItem(&jugador2->box) and jugador2->banAttack){
            if (verificar_golpe(jugador2)){
                health -= 5;
                vida.setRect(0,0,health,5);
            }
        }
    }

    if (health <= 1){
        listaEnemigos.removeOne(this);
        delete this;
    }
}

void Enemigo::muerte()
{
    if (num_jugadores == 2){
        if (jugador->health <= 1){
            jugador->muerto = true;
            jugador->hide();
            jugador->vida.hide();
        }
        if (jugador2->health <= 1){
            jugador2->muerto = true;
            jugador2->hide();
            jugador2->vida.hide();
        }
        if(jugador->muerto and jugador2->muerto)
            PararTimers();
    }
    else{
        if (jugador->health <= 1){
            jugador->muerto = true;
            jugador->hide();
            jugador->vida.hide();
        }
        if(jugador->muerto)
            PararTimers();
    }
}

void Enemigo::move()
{
    if (num_jugadores == 2){
        if (jugador->muerto)
            follow(jugador2);
        else if (jugador2->muerto)
            follow(jugador);
        else if (abs(int(x() - jugador->x())) <= abs(int(x() - jugador2->x())))
            follow(jugador);
        else
            follow(jugador2);
    }
    else{
        follow(jugador);
    }
}
