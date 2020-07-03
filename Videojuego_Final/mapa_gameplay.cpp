#include "mapa_gameplay.h"
#include "ui_mapa_gameplay.h"

#include <QGraphicsRectItem>

extern int num_jugadores;
extern QString user, pass;

Mapa_GamePlay::Mapa_GamePlay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mapa_GamePlay)
{
    ui->setupUi(this);
    nombre = user;

    escena=new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 782, 582);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(782,582);
    ui->graphicsView->setScene(escena);

    jugador = new Jugador;
    jugador->setRect(0,0,50,50);
    jugador->setPos(escena->width()/2, escena->height()/2);
    escena->addItem(jugador);
}

Mapa_GamePlay::~Mapa_GamePlay()
{
    delete ui;
}

void Mapa_GamePlay::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W)
        jugador->setBanUp();
    else if (event->key() == Qt::Key_S)
        jugador->setBanDown();
    else if (event->key() == Qt::Key_A)
        jugador->setBanLeft();
    else if (event->key() == Qt::Key_D)
        jugador->setBanRight();
}

void Mapa_GamePlay::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W)
        jugador->resetBanUp();
    else if (event->key() == Qt::Key_S)
        jugador->resetBanDown();
    else if (event->key() == Qt::Key_A)
        jugador->resetBanLeft();
    else if (event->key() == Qt::Key_D)
        jugador->resetBanRight();
}


