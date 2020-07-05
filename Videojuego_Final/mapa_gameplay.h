#ifndef MAPA_GAMEPLAY_H
#define MAPA_GAMEPLAY_H

#include <QWidget>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "jugador.h"
#include "muro.h"

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
    QTimer *timer;
    QString nombre;
    QGraphicsScene *escena;
    QGraphicsPixmapItem *mapa;
    QGraphicsPixmapItem *objetos;
    bool pj2;

 private slots:
    void ActualizarEscena();
};

#endif // MAPA_GAMEPLAY_H
