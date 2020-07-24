#include "enemigo.h"

#define paso 2.5

extern short int num_jugadores;
extern Jugador *jugador, *jugador2;
extern QList <Enemigo *> lista;

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
    at_enemigo.start(700);

    //Timer para el ataque del enemigo
    connect(&at_jugador, SIGNAL(timeout()), this, SLOT(ataque_jugador()));
    connect(&at_jugador, SIGNAL(timeout()), this, SLOT(detectar_enemigos()));
    connect(&at_jugador, SIGNAL(timeout()), this, SLOT(muerte()));
    at_jugador.start(400);

    //Timer para las actualización y dibujo del sprite.
    /*Este timer nos permitira la constante actualizacion de la imagen de nuestro jugador*/
    connect(&timer,SIGNAL(timeout()),this,SLOT(Actualizacion()));
    timer.start(200);

    //Movimiento del enemigo
    connect(&mov_enemigo, SIGNAL(timeout()), this, SLOT(move()));
    mov_enemigo.start(60);
}

/*Verifica cuando los enemigos atacan al jugador por la espalda o de frente
 cuando se ataca de frente y su respectiva bandera esta activa entonces el enemigo muere
 pero si lo ataca por la espalda el jugador muere*/
bool Enemigo::verificar_golpe(Jugador *obj)
{
    if (obj->golpe_izq and this->x() < obj->box.x())
        return true;
    else if (obj->golpe_der and this->x() > obj->box.x())
        return true;
    else if (obj->golpe_arr and this->y() < obj->box.y())
        return true;
    else if (obj->golpe_aba and this->y() > obj->box.y())
        return true;
    else
        return false;
}

void Enemigo::follow(Jugador *obj)
{
    if (x() > obj->x() and y() > obj->y()){
        pixmap = QPixmap(":/Imagenes/ENEMIGOS.png");
        setPos(x()-paso, y()-paso);
    }
    else if  (x() > obj->x() and y() < obj->y()){
        pixmap = QPixmap(":/Imagenes/ENEMIGOS.png");
        setPos(x()-paso, y()+paso);
    }
    else if  (x() < obj->x() and y() > obj->y()){
        pixmap = QPixmap(":/Imagenes/ENEMIGOS2.png");
        setPos(x()+paso, y()-paso);
    }
    else if  (x() < obj->x() and y() < obj->y()){
        pixmap = QPixmap(":/Imagenes/ENEMIGOS2.png");
        setPos(x()+paso, y()+paso);
    }
    else if (x() > obj->x()){
        setPos(x()-paso, y());
    }
    else if (x() < obj->x()){
        setPos(x()+paso, y());
    }
    else if (y() > obj->y()){
        setPos(x(), y()-paso);
    }
    else if (y() < obj->y()){
        setPos(x(), y()+paso);
    }
    box.setPos(x()-10, y()+10);
    vida.setPos(x(), y());
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

void Enemigo::ataque_enemigo()
{
    if (box.collidesWithItem(&jugador->box) and jugador->health > 0){
        jugador->health -= 5;
        jugador->vida.setRect(0,0,jugador->health,5);
    }
    else{
        if (jugador->health < 25){
            jugador->health++;
            jugador->vida.setRect(0,0,jugador->health,5);
        }
    }
    if (num_jugadores == 2){
        if (box.collidesWithItem(&jugador2->box) and jugador2->health > 0){
            jugador2->health -= 5;
            jugador2->vida.setRect(0,0,jugador2->health,5);
        }
        else{
            if (jugador2->health < 25){
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
                lista.removeOne(this);
                delete this;
                return;
            }
        }
        else{
            if (abs(int(x()-jugador->x())) > 900 or abs(int(y()-jugador->y())) > 700){
                lista.removeOne(this);
                delete this;
                return;
            }
        }
    }
    else{
        if (abs(int(x()-jugador->x())) > 900 or abs(int(y()-jugador->y())) > 700){
            lista.removeOne(this);
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
    if (box.collidesWithItem(&jugador->box)){
        if (verificar_golpe(jugador)){
            health -= 5;
            vida.setRect(0,0,health,5);
        }
    }

    if (num_jugadores == 2){ //En caso de tener dos jugadores
        if (box.collidesWithItem(&jugador2->box)){
            if (verificar_golpe(jugador2)){
                health -= 5;
                vida.setRect(0,0,health,5);
            }
        }
    }

    if (health <= 1){
        lista.removeOne(this);
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
    }
    else{
        if (jugador->health <= 1){
            jugador->muerto = true;
            jugador->hide();
            jugador->vida.hide();
        }
    }
}

void Enemigo::move()
{
    if (num_jugadores == 2){
        if (abs(int(x() - jugador->x())) <= abs(int(x() - jugador2->x())) and !jugador->muerto){
            follow(jugador);
        }
        else{
            follow(jugador2);
        }
    }
    else{
        follow(jugador);
    }
}
