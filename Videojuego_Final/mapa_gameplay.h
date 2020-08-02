#ifndef MAPA_GAMEPLAY_H
#define MAPA_GAMEPLAY_H

#include <QWidget>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QEvent>
#include <fstream>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QMediaPlayer>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include <QMediaPlaylist>
#include <QMessageBox>
#include "jugador.h"
#include "enemigo.h"

using namespace std;

namespace Ui {
class Mapa_GamePlay;
}

class Mapa_GamePlay : public QWidget
{
    Q_OBJECT

public:
    explicit Mapa_GamePlay(QWidget *parent = nullptr, bool nuevaPartida = 1);
    ~Mapa_GamePlay();

    QList <Enemigo *> listaEnemigos;

    void CargarPartida();
    void Tutorial();

    //Funciones detectoras de del teclado
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void changeEvent(QEvent *event);


private: //Atributos de la clase
    Ui::Mapa_GamePlay *ui;
    QMediaPlayer botonSound, ambiente, jugadorMuerto;
    QTimer timer, dead, enemigos;
    QPushButton boton;
    QMediaPlaylist lista;
    QCursor cursor;
    QPixmap Pixmap_Cursor;
    QGraphicsScene *escena;
    QGraphicsPixmapItem * muros;
    QGraphicsPixmapItem *mapa;
    QGraphicsPixmapItem *objetos;
    QLabel *aviso,*advertencia;
    short int BossesMuertos;
    short int PosX0,PosY0,PosX02,PosY02,cont=0;
    short int Xpos,YPos;
    short int EnemigosTotales;
    short int Enemigos_Asesinar, EnemigosCreados, nivel, nivelActual;
    bool tutorial, ObjetivosCumplidos, nueva_partida;
    bool ObjetivosCompletados;
    QList<QPoint> nivel1, nivel2, nivel3;

 private slots:
    void EliminarEnemigos(Enemigo *obj, bool estado);
    void on_Opciones_clicked();
    void aumentarNivel();
    void reanudarTimers();
    void Nivel();
    void spawn();
    void ingreso_batalla();
    void Contador_Enemigos();
    void ActualizarEscena(); //Centra constantemente la grafica en el jugador
    void verificar_muerte();
    void Cerrar_Ventana();

};

#endif // MAPA_GAMEPLAY_H
