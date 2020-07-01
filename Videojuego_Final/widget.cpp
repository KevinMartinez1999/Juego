#include "widget.h"
#include "ui_widget.h"
#include "registrarse.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_login_clicked()
{
    QString user, pass;
    user = ui->usuario->toPlainText();
    pass = ui->clave->toPlainText();
    ifstream file("../Videojuego_Final/Partidas/"+user.toUtf8()+".txt");
    if (!file.is_open())
    {
        QMessageBox::critical(this, "Login", "Login fallido");
        ui->usuario->clear();
        ui->clave->clear();
        return;
    }
    string usuario, clave;
    file>>usuario;
    file>>clave;
    file.close();
    if (user.toStdString() == usuario and pass.toStdString() == clave)
    {
        QMessageBox::information(this, "Login", "Login exitoso!");
    }
    else
    {
        QMessageBox::critical(this, "Login", "Login fallido");
        ui->usuario->clear();
        ui->clave->clear();
    }
}

void Widget::on_registrarse_clicked()
{
    Registrarse *registro = new Registrarse;
    registro->show();
    close();
}
