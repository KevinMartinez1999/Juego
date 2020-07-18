#include "enemigo.h"
#include <QDebug>

extern int num_jugadores;
extern Jugador *jugador, *jugador2;

Enemigo::Enemigo(QObject *parent) : QObject(parent)
{
    vida = 20;

    setRect(0,0,25,25);
    setBrush(QBrush(Qt::green));
    setPos(770,2055);

    connect(&at_enemigo, SIGNAL(timeout()), this, SLOT(ataque_enemigo()));
    connect(&at_enemigo, SIGNAL(timeout()), this, SLOT(detectar_enemigos()));
    at_enemigo.start(1000);

    connect(&at_jugador, SIGNAL(timeout()), this, SLOT(ataque_jugador()));
    at_jugador.start(200);
}

/*Verifica cuando los enemigos atacan al jugador por la espalda o de frente
 cuando se ataca de frente y su respectiva bandera esta activa entonces el enemigo muere
 pero si lo ataca por la espalda el jugador muere*/
bool Enemigo::verificar_golpe(Jugador *obj)
{
    if (obj->golpe_izq and this->x() < obj->x())
        return true;
    else if (obj->golpe_der and this->x() > obj->x())
        return true;
    else if (obj->golpe_arr and this->y() < obj->y())
        return true;
    else if (obj->golpe_aba and this->y() > obj->y())
        return true;
    else
        return false;
}

void Enemigo::ataque_enemigo()
{
    if (collidesWithItem(&jugador->box)){
        jugador->vida.setRect(0,0,jugador->health -= 5,5);
    }
    if (num_jugadores == 2){
        if (collidesWithItem(&jugador2->box)){
            jugador2->vida.setRect(0,0,jugador2->health -= 5,5);
        }
    }
}

/*Cuando el enemigo esta muy lejos de la vista del jugador de elimina ya que ya
 no es necesario*/
void Enemigo::detectar_enemigos()
{
    if (abs(x()-jugador->x()) > 1000 or abs(y()-jugador->y()) > 1000){
        delete this;
    }
}

/*Aqui es la funcion de ataque del enemigo:
 si ataca de frente al jugador y el jugador esta atacando el enemigo muere
 pero si lo ataca por la espalda o por los laterales aunque el jugador esté
 atacando este muere; Esta verificación del ataque se hace en una función mas arriba*/
void Enemigo::ataque_jugador()
{
    if (vida <= 1)
        delete this;

    if (abs(this->x()-jugador->x()) < 84 and abs(this->y()-jugador->y()) < 84){
        if (verificar_golpe(jugador)){
            vida -= 2;
        }
    }

    if (num_jugadores == 2){ //En caso de tener dos jugadores
        if (abs(this->x()-jugador2->x()) < 84 and abs(this->y()-jugador2->y()) < 84){
            if (verificar_golpe(jugador2)){
                vida -= 2;
            }
        }
    }
}

void Enemigo::move()
{

}
