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
    pj2 = false;

    QCursor cursor = QCursor(Qt::BlankCursor);
    setCursor(cursor);

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
        jugadorBatalla->pixmap = new QPixmap(":/Imagenes/SPRITEBATALLA.png");    
    }
    else if (num_jugadores == 2){ //Dos jugadores
        pj2 = true;
        jugadorBatalla = new JugadorBatalla(this);
        jugadorBatalla->pixmap = new QPixmap(":/Imagenes/SPRITEBATALLA.png");

        jugadorBatalla2 = new JugadorBatalla(jugadorBatalla);
        jugadorBatalla2->pixmap = new QPixmap(":/Imagenes/SPRITEBATALLA2.png");
    }
    NivelSetup();
}

Niveles::~Niveles()
{
    delete ui;
}

void Niveles::NivelSetup()
{
    if(nivel==0){
        fondo->setPixmap(QPixmap(":/Imagenes/TUTORIAL.png").scaled(1132,650));
        jugadorBatalla->setX0(170);
        jugadorBatalla->setY0(455);
        if(num_jugadores==2){
            jugadorBatalla2->setX0(65);
            jugadorBatalla2->setY0(455);
        }       
    }
    else if(nivel==1){
        fondo->setPixmap(QPixmap(":/Imagenes/NIVEL1.png").scaled(1000,650));
        jugadorBatalla->setX0(170);
        jugadorBatalla->setY0(515); 
        if(num_jugadores==2){
            jugadorBatalla2->setX0(70);
            jugadorBatalla2->setY0(515);    
        }
    }
    else if(nivel==2){
        fondo->setPixmap(QPixmap(":/Imagenes/NIVEL2.png").scaled(1000,650));
        jugadorBatalla->setX0(175);
        jugadorBatalla->setY0(378);   
        if(num_jugadores==2){
            jugadorBatalla2->setX0(60);
            jugadorBatalla2->setY0(378);
        }
    }
    else if(nivel==3){
        fondo->setPixmap(QPixmap(":/Imagenes/NIVEL3.png").scaled(1000,650));   
        jugadorBatalla->setX0(160);
        jugadorBatalla->setY0(480);
        if(num_jugadores==2){
            jugadorBatalla2->setX0(45);
            jugadorBatalla2->setY0(480);  
        }
    }
    boss = new Boss(this,nivel);
    escena->addItem(boss);
    jugadorBatalla->setPos(jugadorBatalla->GetX0(),jugadorBatalla->GetY0());
    escena->addItem(jugadorBatalla);
    if(num_jugadores==2){
        jugadorBatalla2->setPos(jugadorBatalla2->GetX0(),jugadorBatalla2->GetY0());
        escena->addItem(jugadorBatalla2);
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
