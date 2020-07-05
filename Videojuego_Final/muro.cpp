#include "muro.h"

Muro::Muro(QObject *parent) : QObject(parent)
{
    setPixmap(QPixmap(":/Imagenes/MUROS.png"));
}
