#include "registrarse.h"
#include "ui_registrarse.h"
#include "widget.h"

Registrarse::Registrarse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Registrarse)
{
    ui->setupUi(this);
    QFont font = QFont("Red Right Hand", 12, 1);
    setFont(font);
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

void Registrarse::on_volver_clicked()
{
    Widget *w = new Widget;
    w->show();
    close();
}
