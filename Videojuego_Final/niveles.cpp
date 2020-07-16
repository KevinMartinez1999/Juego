#include "niveles.h"
#include "ui_niveles.h"
#include <mapa_gameplay.h>

extern int nivel;
extern int num_jugadores;
JugadorBatalla *jugadorBatalla, *jugadorBatalla2;

Niveles::Niveles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Niveles)
{
    ui->setupUi(this);

    QCursor cursor = QCursor(Qt::BlankCursor);
    setCursor(cursor);

    pj2 = false;

    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0,982,632);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setScene(escena);
    fondo = new QGraphicsPixmapItem;
    fondo->setPos(0,0);
    if(nivel==1)fondo->setPixmap(QPixmap(":/Imagenes/TUTORIAL.png").scaled(1132,650));
    if(nivel==2)fondo->setPixmap(QPixmap(":/Imagenes/NIVEL1.png").scaled(1000,632));
    if(nivel==3)fondo->setPixmap(QPixmap(":/Imagenes/NIVEL2.png").scaled(1000,650));
    escena->addItem(fondo);

    if (num_jugadores == 1){ //Solo un jugador
        jugadorBatalla= new JugadorBatalla(this);
        jugadorBatalla->setRect(0,550,50,50);
        if(nivel==1) jugadorBatalla->setPos(120,-60);
        if(nivel==2) jugadorBatalla->setPos(50,-20);
        if(nivel==3) jugadorBatalla->setPos(20,-140);
        jugadorBatalla->setBrush(QBrush(Qt::red));
        escena->addItem(jugadorBatalla);
    }
    else if (num_jugadores == 2){ //Dos jugadores
        pj2 = true;
        jugadorBatalla = new JugadorBatalla(this);
        jugadorBatalla->setBrush(QBrush(Qt::red));
        jugadorBatalla->setRect(0,550,50,50);
        if(nivel==1) jugadorBatalla->setPos(120,-60);
        if(nivel==2) jugadorBatalla->setPos(50,-20);
        if(nivel==3) jugadorBatalla->setPos(20,-140);
        escena->addItem(jugadorBatalla);

        jugadorBatalla2 = new JugadorBatalla(jugadorBatalla);
        jugadorBatalla2->setBrush(QBrush(Qt::blue));
        jugadorBatalla2->setRect(0,550,50,50);
        if(nivel==1) jugadorBatalla2->setPos(250,-60);
        if(nivel==2) jugadorBatalla2->setPos(140,-20);
        if(nivel==3) jugadorBatalla2->setPos(135,-140);
        escena->addItem(jugadorBatalla2);

    }
}

Niveles::~Niveles()
{
    delete ui;
}

void Niveles::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A){
        jugadorBatalla->setBanLeft();
    }
    else if (event->key() == Qt::Key_D){
        jugadorBatalla->setBanRight();
    }
    else if(event->key()==Qt::Key_J){
        if(pj2)
            jugadorBatalla2->setBanLeft();
    }
    else if(event->key()==Qt::Key_L){
        if(pj2)
            jugadorBatalla2->setBanRight();
    }

}

void Niveles::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A){
            jugadorBatalla->resetBanLeft();
        }
    else if (event->key() == Qt::Key_D){
        jugadorBatalla->resetBanRight();
    }
    else if(event->key()==Qt::Key_J){
        if(pj2)
            jugadorBatalla2->resetBanLeft();
    }
    else if(event->key()==Qt::Key_L){
        if(pj2)
            jugadorBatalla2->resetBanRight();

    }
}
