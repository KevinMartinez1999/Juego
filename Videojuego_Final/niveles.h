#ifndef NIVELES_H
#define NIVELES_H

#include <QWidget>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include "jugadorbatalla.h"
#include "boss.h"


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
    QTimer timer;
    bool pj2;
private slots:
    void Controles();
    void Level_Events();
    void on_Opciones_clicked();
};

#endif // NIVELES_H
