#include "registrarse.h"
#include "ui_registrarse.h"
#include "widget.h"

Registrarse::Registrarse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Registrarse)
{
    ui->setupUi(this);
}

Registrarse::~Registrarse()
{
    delete ui;
}

void Registrarse::on_registrarse_clicked()
{
    QString user, pass;
    user = ui->usuario->toPlainText();
    pass = ui->clave->toPlainText();
    ofstream file("../Videojuego_Final/Partidas/"+user.toUtf8()+".txt");
    file<<user.toStdString()<<"\n"<<pass.toStdString();
    file.flush();
    file.close();
    QMessageBox::information(this, "Registro", "Registrado correctamente");
    Widget *w = new Widget;
    w->show();
    close();
}
