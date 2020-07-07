#include "mapa_gameplay.h"
#include "ui_mapa_gameplay.h"
#include <QDebug>

extern int num_jugadores;
extern QString user, pass;
Muro *muro;
Jugador *jugador, *jugador2;

Mapa_GamePlay::Mapa_GamePlay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mapa_GamePlay)
{
    ui->setupUi(this);

    QPixmap Pixmap_Cursor = QPixmap(":/Imagenes/CURSOR.png");
    QCursor cursor = QCursor(Pixmap_Cursor,0,0);
    setCursor(cursor);

    nombre = user;
    pj2 = false;

    escena=new QGraphicsScene(this);
    escena->setSceneRect(0, 0,2239,2235);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setScene(escena);

    muro= new Muro;
    muro->setPos(0,0);
    escena->addItem(muro);

    mapa=new QGraphicsPixmapItem;
    mapa->setPos(0,0);
    mapa->setPixmap(QPixmap(":/Imagenes/MAPA.png"));
    escena->addItem(mapa);

    if (num_jugadores == 1){
        jugador = new Jugador(this);
        jugador->setRect(0,0,30,30);
        jugador->setPos(770,2155);
        jugador->setBrush(Qt::red);
        escena->addItem(jugador);
    }
    else if (num_jugadores == 2){
        pj2 = true;

        jugador = new Jugador(this);
        jugador->setRect(0,0,30,30);
        jugador->setPos(770,2155);
        jugador->setBrush(Qt::red);
        escena->addItem(jugador);

        jugador2 = new Jugador(jugador);
        jugador2->setRect(0,0,30,30);
        jugador2->setPos(820,2155);
        jugador2->setBrush(Qt::blue);
        escena->addItem(jugador2);
    }

    timer=new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(ActualizarEscena()));
    timer->start();

    objetos=new QGraphicsPixmapItem;
    objetos->setPos(0,0);
    objetos->setPixmap(QPixmap(":/Imagenes/OBJETOS.png"));
    escena->addItem(objetos);
}

Mapa_GamePlay::~Mapa_GamePlay()
{
    delete ui;
}

void Mapa_GamePlay::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W){
        jugador->setBanUp();
    }
    else if (event->key() == Qt::Key_S){
        jugador->setBanDown();
    }
    else if (event->key() == Qt::Key_A){
        jugador->setBanLeft();
    }
    else if (event->key() == Qt::Key_D){
        jugador->setBanRight();
    }
    else if(event->key()==Qt::Key_J){
        if(pj2)
            jugador2->setBanLeft();
    }
    else if(event->key()==Qt::Key_L){
        if(pj2)
            jugador2->setBanRight();
    }
    else if(event->key()==Qt::Key_I){
        if(pj2)
            jugador2->setBanUp();
    }
    else if(event->key()==Qt::Key_K){
        if(pj2)
            jugador2->setBanDown();
    }
}

void Mapa_GamePlay::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W){
        jugador->resetBanUp();
    }
    else if (event->key() == Qt::Key_S){
        jugador->resetBanDown();
    }
    else if (event->key() == Qt::Key_A){
        jugador->resetBanLeft();
    }
    else if (event->key() == Qt::Key_D){
        jugador->resetBanRight();
    }
    else if(event->key()==Qt::Key_J){
        if(pj2)
            jugador2->resetBanLeft();
    }
    else if(event->key()==Qt::Key_L){
        if(pj2)
            jugador2->resetBanRight();
    }
    else if(event->key()==Qt::Key_I){
        if(pj2)
            jugador2->resetBanUp();
    }
    else if(event->key()==Qt::Key_K){
        if(pj2)
            jugador2->resetBanDown();
    }
}

void Mapa_GamePlay::ActualizarEscena()
{
    ui->graphicsView->centerOn(jugador);
}


