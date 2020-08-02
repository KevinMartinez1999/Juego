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
    void NivelSetup();
    void Tutorial();
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent *event);
    void changeEvent(QEvent *event);

private:
    Ui::Niveles *ui;
    QGraphicsScene *escena;
    QGraphicsPixmapItem *fondo;
    QMediaPlaylist Lista;
    QMediaPlayer musicaNivel, JugadorMuerto, victoria;
    QTimer timer;
    int cont=0;
    bool tutorial;
    short int nivel;
    QByteArray ruta;

signals:
    void aumentar();

private slots:
    void muerte();
    void verificar_muerte();
    void Level_Events();
    void on_Opciones_clicked();
    void Cerrar_Ventana();
    void reanudarTimers();
};

#endif // NIVELES_H
