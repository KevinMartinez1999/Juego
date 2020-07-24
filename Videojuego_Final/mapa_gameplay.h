#ifndef MAPA_GAMEPLAY_H
#define MAPA_GAMEPLAY_H

#include <QWidget>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QMediaPlayer>
#include <QPushButton>
#include <QLabel>
#include "jugador.h"
#include "muro.h"
#include <QMediaPlaylist>
#include <QMessageBox>

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
    QMediaPlayer * botonSound;
    QMediaPlaylist lista;
    QMediaPlayer ambiente;
    QCursor cursor;
    QGraphicsScene *escena;
    QTimer timer, dead;
    QString nombre;
    QGraphicsPixmapItem *mapa;
    QGraphicsPixmapItem *objetos;  
    QLabel *aviso;
    QPushButton *boton;
    int PosX,PosY,PosX2,PosY2;
    int Xpos,YPos;
    bool pj2;

 private slots:
    void Controles();
    void Nivel();
    void ingreso_batalla();
    void ActualizarEscena(); //Centra constantemente la grafica en el jugador
    void verificar_muerte();
    void on_Opciones_clicked();
};

#endif // MAPA_GAMEPLAY_H
