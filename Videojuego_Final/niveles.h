#ifndef NIVELES_H
#define NIVELES_H

#include <QWidget>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <fstream>
#include <iostream>
#include <QMessageBox>
#include "jugadorbatalla.h"
#include "boss.h"

using namespace std;

namespace Ui {
class Niveles;
}

class Niveles : public QWidget
{
    Q_OBJECT

public:
    explicit Niveles(QWidget *parent = nullptr, short int lvl = 0);
    ~Niveles();

    /*Funcion que cargara el nivel deseado dependiendo de las variables pasadas desde mapa_Gameplay*/
    void NivelSetup();

     /*Funcion que nos permitira cambiar las distintas imagenes de ayuda durante el tutorial*/
    void Tutorial();

    /*Funciones detectoras del teclado que permitiran el movimiento de los jugadores, sus ataques y
     su interaccion con el menu de pausa y otras opciones. */
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent *event);

    /*Esta funcion nos permitira detectar si el jugador ha minimizado el juego con el objetivo de evitar
    errores.*/
    void changeEvent(QEvent *event);

private:
    Ui::Niveles *ui;
    QGraphicsScene *escena;//Escena de la ventana

    QGraphicsPixmapItem *fondo;//Aqui se guardara el fondo del nivel

    //Playlist en la cual se almacenara la cancion del nivel y se pondra el modo loop
    QMediaPlaylist Lista;
    //Diferentes sonidos del nivel
    QMediaPlayer musicaNivel, JugadorMuerto, victoria;

    //Timer para revisar periodicamente los distintos eventos del nivel
    QTimer timer;

    int cont=0;//Variable auxiliar para cambiar de imagenes en el tutorial

    bool tutorial;//Variable para determinar si cargar o no las imagenes de tutorial

    //Variable con la cual determinaremos que cosas cargar dependiendo del numero del nivel
    short int nivel;

    /*Variable en la que almacenaremos la ruta de la carpeta de las partidas, es necesaria que sea un QBytearray
    para que sea compatible con otros elementos como el usuario que es un qstring que se convertira con el .toUtf8*/
    QByteArray ruta;

signals:
    /*Señal que se envia a Mapa_Gameplay para actualizar la variable de nivel actual en caso de victoria*/
    void aumentar();

private slots:  
    //Verifica si los jugadores estan muertos
    void muerte();

    //En caso de que los jugadores si se encuentren muertos esta funcion genera un nuevo mapa y termina todo el nivel
    void verificar_muerte();

    //Verifica si el boss fue derrotado para guardar los datos y cambiar de nivel el mapa
    void Level_Events();

    /*Boton con el cual podremos pausar el juego y abrir el menu de pausa.*/
    void on_Opciones_clicked();

    /*Slot que estara conectado a una signal de MenuPausa para que en caso de que el jugador desee cerrar
    sesion todas las ventanas se cierren completamente y se abra un nuevo Widget*/
    void Cerrar_Ventana();

    /*Funcion con la cual podremos reanudar todos los timers de los jugadores y enemigos al presionar la tecla
     de  reanudar en el menu de pausa-opciones. Este slot estara conectado a una signal en menuPausa*/
    void reanudarTimers();
};

#endif // NIVELES_H
