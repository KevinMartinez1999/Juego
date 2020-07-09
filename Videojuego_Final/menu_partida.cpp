#include "menu_partida.h"
#include "ui_menu_partida.h"
#include "widget.h"
#include "mapa_gameplay.h"

int num_jugadores;
extern QString user, pass;

Menu_partida::Menu_partida(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu_partida)
{
    ui->setupUi(this);

    QLabel *w = new QLabel(this);
    w->resize(1000,650);
    QMovie *movie = new QMovie(":/Imagenes/GIF1.gif");
    movie->setScaledSize(QSize(1000,650));
    w->setMovie(movie);
    movie->start();
    ui->interfaz->raise();
    ui->un_jugador->raise();
    ui->dos_jugadores->raise();
    ui->cargar_partida->raise();
    ui->nueva_partida->raise();
    ui->volver->raise();

    /*Creacion del cursor del videojuego: con ayuda de QCursor podremos brindarle al cursor la imagen que deseamos.
    Primeramente crearemos un pixmap que contiene la imagen, luego se creara una variable QCursor que recibira la imagen
    y los puntos de eje del click, luego con setCursor establecemos ese cursor para toda la ventana.*/
    QPixmap Pixmap_Cursor = QPixmap(":/Imagenes/CURSOR.png");
    QCursor cursor = QCursor(Pixmap_Cursor,0,0);
    setCursor(cursor);

    ui->un_jugador->setChecked(true);/*Por defecto siempre estara señalada la opcion de un jugador en caso
    de que el usuario no escoja.*/

    int cont = 0;

    ifstream file("../Videojuego_Final/Partidas/"+user.toUtf8()+".txt");
    if (!file.is_open())
        return;

    while(file.good())
        if(file.get()=='\n')
            cont++;

    if (cont <= 1)
        ui->cargar_partida->setEnabled(false);
    else
        ui->cargar_partida->setEnabled(true);
}

Menu_partida::~Menu_partida()
{
    delete ui;
}

void Menu_partida::on_volver_clicked()
{
    /*Si por alguna razon al entrar al menu de nueva-cargar partida deseamos cerrar sesion y volver al menu principal simplemente
    se cerrara la ventana y se creara un nuevo widget y se abrira.*/
    Widget *w = new Widget;
    w->show();
    close();
}

void Menu_partida::on_nueva_partida_clicked()
{
    if(ui->un_jugador->isChecked())
        num_jugadores = 1;
    else if(ui->dos_jugadores->isChecked())
        num_jugadores = 2;

    Mapa_GamePlay *mapa = new Mapa_GamePlay();
    mapa->show();
    close();
}

void Menu_partida::on_cargar_partida_clicked()
{

}
