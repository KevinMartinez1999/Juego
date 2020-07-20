#include "enemigo.h"

extern int num_jugadores;
extern Jugador *jugador, *jugador2;

Enemigo::Enemigo(QObject *parent) : QObject(parent)
{
    //Inicializando vida del enemigo
    health = 20;

    //Dimendiones del enemigo
    setRect(0,0,25,25);
    setBrush(QBrush(Qt::green));
    setPos(770,2055);

    //Timers del enemigo
    connect(&at_enemigo, SIGNAL(timeout()), this, SLOT(ataque_enemigo()));
    connect(&at_enemigo, SIGNAL(timeout()), this, SLOT(detectar_enemigos()));
    at_enemigo.start(1000);

    connect(&at_jugador, SIGNAL(timeout()), this, SLOT(ataque_jugador()));
    at_jugador.start(200);

    //Barra de vida
    vida.setRect(0,0,health,5);
    vida.setBrush(Qt::blue);
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
    if (jugador->health <= 1){
        delete jugador;
        return;
    }

    if (collidesWithItem(&jugador->box) and jugador->health > 0){
        jugador->health -= 5;
        jugador->vida.setRect(0,0,jugador->health,5);
    }
    if (num_jugadores == 2){
        if (collidesWithItem(&jugador2->box) and jugador2->health > 0){
            jugador->health -= 5;
            jugador2->vida.setRect(0,0,jugador2->health,5);
        }
    }
}

/*Cuando el enemigo esta muy lejos de la vista del jugador de elimina ya que ya
 no es necesario*/
void Enemigo::detectar_enemigos()
{
    if (abs(int(x()-jugador->x())) > 1000 or abs(int(y()-jugador->y())) > 1000){
        delete this;
        return;
    }
}

/*Aqui es la funcion de ataque del enemigo:
 si ataca de frente al jugador y el jugador esta atacando el enemigo muere
 pero si lo ataca por la espalda o por los laterales aunque el jugador esté
 atacando este muere; Esta verificación del ataque se hace en una función mas arriba*/
void Enemigo::ataque_jugador()
{
    if (collidesWithItem(&jugador->box)){
        if (verificar_golpe(jugador)){
            health -= 2;
            vida.setRect(0,0,health,5);
        }
    }

    if (num_jugadores == 2){ //En caso de tener dos jugadores
        if (collidesWithItem(&jugador2->box)){
            if (verificar_golpe(jugador2)){
                health -= 2;
                vida.setRect(0,0,health,5);
            }
        }
    }

    if (health <= 1)
        delete this;
}
