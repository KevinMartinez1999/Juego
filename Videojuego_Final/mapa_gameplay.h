#ifndef MAPA_GAMEPLAY_H
#define MAPA_GAMEPLAY_H

#include <QWidget>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QMediaPlayer>
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

    //Funciones detectoras de del teclado

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private: //Atributos de la clase
    Ui::Mapa_GamePlay *ui;
    QMediaPlayer ambiente;
    QTimer loop, timer;
    QString nombre;
    QGraphicsScene escena;
    QGraphicsPixmapItem *mapa;
    QGraphicsPixmapItem *objetos;
    bool pj2;

 private slots:
    void ActualizarEscena(); //Centra constantemente la grafica en el jugador
    void iniciar() {ambiente.play();}
};

#endif // MAPA_GAMEPLAY_H
