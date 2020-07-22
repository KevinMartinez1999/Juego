#include "menupausa.h"
#include "ui_menupausa.h"
#include "widget.h"

MenuPausa::MenuPausa(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuPausa)
{
    ui->setupUi(this);
    cerrar_sesion=false;
    QPixmap Pixmap_Cursor = QPixmap(":/Imagenes/CURSOR.png");
    QCursor cursor = QCursor(Pixmap_Cursor,0,0);
    setCursor(cursor);

    QLabel *w = new QLabel(this);
    w->resize(440,440);//Tamaño de la ventana.
    movie = new QMovie(this);
    movie->setFileName(":/Imagenes/GIF4.gif");
    movie->setScaledSize(QSize(440,440));//Tamaño de la ventana.
    w->setMovie(movie);//Asignamos el gif al Label.
    movie->start();

    ui->Marco->raise();
    ui->Reanudar->raise();
    ui->Guardar->raise();
    ui->Controles->raise();
    ui->Cerrar_Sesion->raise();
}

MenuPausa::~MenuPausa()
{
    delete ui;
}

void MenuPausa::on_Guardar_clicked()
{

}

void MenuPausa::on_Controles_clicked()
{

}

void MenuPausa::on_Cerrar_Sesion_clicked()
{

}
