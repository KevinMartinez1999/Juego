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
    ui->un_jugador->setChecked(true);

    QPixmap Pixmap_Cursor = QPixmap(":/Imagenes/CURSOR.png");
    QCursor cursor = QCursor(Pixmap_Cursor,0,0);
    setCursor(cursor);

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
