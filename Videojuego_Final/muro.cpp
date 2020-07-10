#include "muro.h"

Muro::Muro(QObject *parent) : QObject(parent)
{
    /*La clase muro contiene una imagen png diseñada para representar las determinadas partes del mapa en las que se puede colisionar.
     Se incluye este objeto para verificar si el jugador con su Hitbox esta colisionando con las paredes/objetos.
    */
    setPixmap(QPixmap(":/Imagenes/MUROS.png"));
}
