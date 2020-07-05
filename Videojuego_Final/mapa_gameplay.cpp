#include "mapa_gameplay.h"
#include "ui_mapa_gameplay.h"
#include <QDebug>

extern int num_jugadores;
extern QString user, pass;
Muro *muro;
Mapa_GamePlay::Mapa_GamePlay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mapa_GamePlay)
{
    ui->setupUi(this);
    nombre = user;

    escena=new QGraphicsScene(this);
    escena->setSceneRect(0, 0,2239,2235);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    setFixedSize(782,582);
    ui->graphicsView->setScene(escena);

    muro= new Muro;
    muro->setPos(0,0);
    escena->addItem(muro);

    mapa=new QGraphicsPixmapItem;
    mapa->setPos(0,0);
    mapa->setPixmap(QPixmap(":/Imagenes/MAPA.png"));
    escena->addItem(mapa);

    jugador = new Jugador;
    jugador->setRect(0,0,30,30);
    jugador->setPos(770,2155);
    jugador->setBrush(Qt::red);

    timer=new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(ActualizarEscena()));
    timer->start(25);

    escena->addItem(jugador);

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
        jugador->setBanUp();}
    else if (event->key() == Qt::Key_S){
        jugador->setBanDown();}
    else if (event->key() == Qt::Key_A){
        jugador->setBanLeft();}
    else if (event->key() == Qt::Key_D){
        jugador->setBanRight();}
}

void Mapa_GamePlay::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W){
        jugador->resetBanUp();}
    else if (event->key() == Qt::Key_S){
        jugador->resetBanDown();}
    else if (event->key() == Qt::Key_A){
        jugador->resetBanLeft();}
    else if (event->key() == Qt::Key_D){
        jugador->resetBanRight();}
}

void Mapa_GamePlay::ActualizarEscena()
{
    ui->graphicsView->centerOn(jugador->x(),jugador->y());
}


