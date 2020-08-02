#include "menupausa.h"
#include "ui_menupausa.h"
#include "mapa_gameplay.h"
#include "niveles.h"
#include "enemigo.h"

extern QMediaPlayer * musica;

MenuPausa::MenuPausa(Widget *parent) : Widget(parent),
    ui(new Ui::MenuPausa)
{
    ui->setupUi(this);

    contador=0;
    cerrar_sesion=false;

    //Sonido al presionar los botones
    boton.setMedia(QUrl("qrc:/Musica/ESPADA.mp3"));
    boton.setVolume(100);

    Pixmap_Cursor = QPixmap(":/Imagenes/CURSOR.png");
    cursor = QCursor(Pixmap_Cursor,0,0);
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
    ui->Salir->raise();
    ui->ControlesLabel->raise();
    ui->Siguiente->raise();

    ui->ControlesLabel->hide();
    ui->Siguiente->hide();
}

MenuPausa::~MenuPausa()
{
    delete ui;
}

void MenuPausa::closeEvent(QCloseEvent *event)
{
    boton.play();
    emit reanudar();
    event->accept();
}

void MenuPausa::on_Reanudar_clicked()
{
    boton.play();
    emit reanudar();
    close();
    delete this;
}

void MenuPausa::on_Controles_clicked()
{
    ui->Siguiente->show();
    ui->ControlesLabel->show();
    ui->ControlesLabel->setPixmap(QPixmap(":/Imagenes/TECLASMAPA.png"));
}

void MenuPausa::on_Cerrar_Sesion_clicked()
{
    musica->stop();
    Widget *w = new Widget;
    w->show();
    close();
    emit Cerrar_Sesion();
    delete this;
}

void MenuPausa::on_Salir_clicked()
{
    QCoreApplication::quit();
}

void MenuPausa::on_Siguiente_clicked()
{
    contador++;
    if(contador==1){
        ui->ControlesLabel->setPixmap(QPixmap(":/Imagenes/TECLAS.png"));
        ui->Siguiente->setText("Volver");}
    else if(contador==2){
        ui->ControlesLabel->hide();
        ui->Siguiente->setText("Siguiente");
        ui->Siguiente->hide();
        contador=0;
    }
}
