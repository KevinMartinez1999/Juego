#include "menupausa.h"
#include "ui_menupausa.h"
#include "widget.h"
#include "mapa_gameplay.h"
#include "niveles.h"

extern JugadorBatalla *jugadorBatalla, *jugadorBatalla2;
extern Jugador *jugador, *jugador2;
extern short int num_jugadores;

MenuPausa::MenuPausa(QWidget *parent,bool ventana) :
    QWidget(parent), VentanaPausada(ventana),
    ui(new Ui::MenuPausa)
{
    ui->setupUi(this);
    cerrar_sesion=false;
    QPixmap Pixmap_Cursor = QPixmap(":/Imagenes/CURSOR.png");
    QCursor cursor = QCursor(Pixmap_Cursor,0,0);
    setCursor(cursor);

    w = new QLabel(this);
    w->resize(440,440);//Tamaño de la ventana.
    movie = new QMovie(this);
    movie->setFileName(":/Imagenes/GIF4.gif");
    movie->setScaledSize(QSize(440,440));//Tamaño de la ventana.
    w->setMovie(movie);//Asignamos el gif al Label.
    movie->start();

    ui->Marco->raise();
    ui->Reanudar->raise();
    ui->Controles->raise();
    ui->Cerrar_Sesion->raise();
}

MenuPausa::~MenuPausa()
{
    delete ui;
}

void MenuPausa::closeEvent(QCloseEvent *event)
{
    if(VentanaPausada==0){
        jugador->ReiniciarTimers();
        if (num_jugadores == 2)
            jugador2->ReiniciarTimers();}
    else{
        jugadorBatalla->ReiniciarTimers();
        if (num_jugadores == 2)
            jugadorBatalla2->ReiniciarTimers();}
    event->accept();
}

void MenuPausa::on_Reanudar_clicked()
{
    if(VentanaPausada==0){
        jugador->ReiniciarTimers();
        if (num_jugadores == 2)
            jugador2->ReiniciarTimers();}
    else{
        jugadorBatalla->ReiniciarTimers();
        if (num_jugadores == 2)
            jugadorBatalla2->ReiniciarTimers();}
    delete this;
}

void MenuPausa::on_Controles_clicked()
{

}

void MenuPausa::on_Cerrar_Sesion_clicked()
{

}

