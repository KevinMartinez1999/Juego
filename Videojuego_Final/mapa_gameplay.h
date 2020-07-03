#ifndef MAPA_GAMEPLAY_H
#define MAPA_GAMEPLAY_H

#include <QWidget>
#include <QGraphicsScene>
#include <QKeyEvent>

#include "jugador.h"

namespace Ui {
class Mapa_GamePlay;
}

class Mapa_GamePlay : public QWidget
{
    Q_OBJECT

public:
    explicit Mapa_GamePlay(QWidget *parent = nullptr);
    ~Mapa_GamePlay();
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::Mapa_GamePlay *ui;
    QString nombre;
    QGraphicsScene *escena;
    Jugador *jugador;
};

#endif // MAPA_GAMEPLAY_H
