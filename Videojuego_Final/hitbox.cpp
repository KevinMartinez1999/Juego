#include "hitbox.h"

HitBox::HitBox(QObject *parent) : QObject(parent)
{
    /*Hitbox es una funcion auxiliar que nos permite crear un rectangulo que esconderemos de la escena pero que usaremos para verificar
    las colisiones de los personajes con los muros y objetos del mapa.
    Este rectangulo se situara entre la cadera y los pies del personaje y estara constantemente actualizando su posicion para mantenerse
    siguiendo siempre al jugador.*/
    setRect(0,0,25,25);
}
