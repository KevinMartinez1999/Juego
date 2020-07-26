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
#include <QMessageBox>
#include <iostream>
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
    explicit Niveles(QWidget *parent = nullptr);
    ~Niveles();
    void NivelSetup();
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::Niveles *ui;
    QGraphicsScene *escena;
    QGraphicsPixmapItem *fondo;
    QMediaPlaylist Lista;
    QMediaPlayer musicaNivel;
    QTimer timer, bolas;
    bool pj2;

public slots:
    void spawn_bolas();
    void spawn_bolas2();

private slots:
    void muerte();
    void verificar_muerte();
    void Controles();
    void Level_Events();
    void on_Opciones_clicked();
};

#endif // NIVELES_H
