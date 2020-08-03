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
/*La clase Mapa_Gameplay es la clase en la cual podremos interactuar cono todo un mapa en el cual deberemos
cumplir los objetivos de cada nivel, que seran asesinar a los enemigos indicados, en caso de cumplir los
objetivos podremos ingresar a los distintos niveles en los que pelearemos contra los bosses.*/
public:
    explicit Mapa_GamePlay(QWidget *parent = nullptr, bool nuevaPartida = 1);
    ~Mapa_GamePlay();

    /*Lista en la que se almacenaran los enemigos para asi tener control de cuantos existiran al mismo tiempo
    para evitar problemas de memoria.*/
    QList <Enemigo *> listaEnemigos;

    /*Funcion que nos permitira determinar las posiciones iniciales de los jugadores y el nivel en el que se
    encuentran*/
    void CargarPartida();

    /*Funcion que nos permitira cambiar las distintas imagenes de ayuda durante el tutorial*/
    void Tutorial();

    /*Funciones detectoras del teclado que permitiran el movimiento de los jugadores, sus ataques y
     su interaccion con el menu de pausa y otras opciones. */
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    /*Esta funcion nos permitira detectar si el jugador ha minimizado el juego con el objetivo de evitar
    errores.*/
    void changeEvent(QEvent *event);


private: //Atributos de la clase
    Ui::Mapa_GamePlay *ui;

    QMediaPlaylist lista;//Lista en la que se guardara la cancion de ambiete y se mantendra en loop

    QMediaPlayer botonSound, ambiente, jugadorMuerto;//Distintos Sonidos que seran usados

    QTimer timer, dead, enemigos;//Timers para actualizar la escena, verificar muerte y generar enemigos

    QPushButton boton;//Boton que se mostrara al momento de querer ingresar un nivel

    QCursor cursor;//Variable QCursor que nos permitira aplicar un cursor personalizado a la ventana

    QPixmap Pixmap_Cursor;//Imagen del cursor

    QGraphicsScene *escena;//Escena de la ventana

    QGraphicsPixmapItem * muros;//Objeto pixmap en el que guardaremos las distintas colisiones de los jugadores

    QGraphicsPixmapItem *mapa;//Variable en la cual se guardara la imagen del mapa

    QGraphicsPixmapItem *objetos;//Variable en la cual se guardara una imagen que genera perspectiva en el mapa

    QLabel *aviso;//Label en el cual se mostrara la imagen de ingreso al nivel o advertencia de objetivos no completados

    /*Variable en la que almacenaremos la ruta de la carpeta de las partidas, es necesaria que sea un QBytearray
    para que sea compatible con otros elementos como el usuario que es un qstring que se convertira con el .toUtf8*/
    QByteArray ruta;

    short int BossesMuertos;//Con esta variable determinaremos el nivel que se cargara

    short int PosX0,PosY0,PosX02,PosY02,cont=0;//Variables auxiliares para poner posiciones de cargado al jugador

    short int Xpos,YPos;//Variables auxiliares para verificar posicion del jugador

    short int EnemigosTotales;//Variable auxiliar para verificar los enemigos totales a crear

    //Variables para el control de enemigos a matar y niveles que se cargaran o estan siendo cargados
    short int Enemigos_Asesinar, EnemigosCreados, nivel, nivelActual;

    //Bools para determinar si el jugador esta en el tutorial o en una nueva partida
    bool tutorial, ObjetivosCumplidos, nueva_partida;

    bool ObjetivosCompletados;//Bool que determinara si el jugador debe matar a los enemigos o no

    QList<QPoint> nivel1, nivel2, nivel3;//Listas de QPoints que se rellenaran con los puntos de spawn de enemigos

 private slots:
    /*Este slot es encargado de eliminar de la lista de enemigos a cada enemigo que es asesinado, ademas
    verifica si el enemigo eliminado es el ultimo enemigo que se debe asesinar o si el enemigo fue eliminado
    debido a que se encuentra muy lejos del jugador.*/
    void EliminarEnemigos(Enemigo *obj, bool estado);

    /*Boton con el cual podremos pausar el juego y abrir el menu de pausa.*/
    void on_Opciones_clicked();

    /*Slot que permite Aumentar el nivel desde la clase Nivel y asi cargar correctamente los datos al ganar
    el nivel*/
    void aumentarNivel();

    /*Funcion con la cual podremos reanudar todos los timers de los jugadores y enemigos al presionar la tecla
     de  reanudar en el menu de pausa-opciones. Este slot estara conectado a una signal en menuPausa*/
    void reanudarTimers();

    /*Con esta funcion si presionamos el boton de ingresar al nivel se analizara las posiciones en las que
    estamos y se nos ingresara al nivel indicado.*/
    void Nivel();

    /*Funcion que permite la generacion de enemigos*/
    void spawn();

    /*Esta funcion permite crear las entradas a los distintos niveles del mapa.*/
    void ingreso_batalla();

    /*Esta funcion se encarga de verificar si ya todos los enemigos han sido asesinados para guardar la partida
    y para actualizar el contador de enemigos en la escena.*/
    void Contador_Enemigos();

    /*Esta funcion constantemente centrara la escena en el jugador, en el caso de dos jugadores en el jugador 2
    si el jugador 1 esta muerto.*/
    void ActualizarEscena();

    /*Funcion que constantemente verifica si los jugadores han muerto para volver a empezar la partida*/
    void verificar_muerte();

    /*Slot que estara conectado a una signal de MenuPausa para que en caso de que el jugador desee cerrar
    sesion todas las ventanas se cierren completamente y se abra un nuevo Widget*/
    void Cerrar_Ventana();

};

#endif // MAPA_GAMEPLAY_H
