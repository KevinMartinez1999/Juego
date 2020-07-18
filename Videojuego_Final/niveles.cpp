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
    escena->addItem(fondo);
    if (num_jugadores == 1){ //Solo un jugador
        jugadorBatalla = new JugadorBatalla(this);
        jugadorBatalla->pixmap = new QPixmap(":/Imagenes/SPRITEPLAYER.png");
        escena->addItem(jugadorBatalla);
        NivelSetup();
    }
    else if (num_jugadores == 2){ //Dos jugadores
        pj2 = true;
        jugadorBatalla = new JugadorBatalla(this);
        jugadorBatalla->pixmap = new QPixmap(":/Imagenes/SPRITEPLAYER.png");
        escena->addItem(jugadorBatalla);

        jugadorBatalla2 = new JugadorBatalla(jugadorBatalla);
        jugadorBatalla2->pixmap = new QPixmap(":/Imagenes/SPRITEPLAYER2.png");
        escena->addItem(jugadorBatalla2);
        NivelSetup();
    }
}

Niveles::~Niveles()
{
    delete ui;
}

void Niveles::NivelSetup()
{
    if(nivel==1){
        fondo->setPixmap(QPixmap(":/Imagenes/TUTORIAL.png").scaled(1132,650));
        jugadorBatalla->setPos(120,490);
        if(num_jugadores==2)jugadorBatalla2->setPos(250,490);
        boss = new Boss(this);
        boss->pixmap = new QPixmap(":/Imagenes/BOSS1.png");
        boss->setPos(900,490);
        escena->addItem(boss);
    }
    if(nivel==2){
        fondo->setPixmap(QPixmap(":/Imagenes/NIVEL1.png").scaled(1000,650));
        jugadorBatalla->setPos(50,545);
        if(num_jugadores==2)jugadorBatalla2->setPos(140,545);
    }
    if(nivel==3){
        fondo->setPixmap(QPixmap(":/Imagenes/NIVEL2.png").scaled(1000,650));
        jugadorBatalla->setPos(20,408);
        if(num_jugadores==2)jugadorBatalla2->setPos(135,408);
    }
    if(nivel==4){
        fondo->setPixmap(QPixmap(":/Imagenes/NIVEL3.png").scaled(1000,650));
        jugadorBatalla->setPos(20,550);
        if(num_jugadores==2)jugadorBatalla2->setPos(135,550);
    }
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
