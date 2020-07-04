#include "widget.h"
#include "ui_widget.h"
#include "registrarse.h"
#include "menu_partida.h"

QString user, pass;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QFontDatabase::addApplicationFont(":/Font/Red Right Hand.ttf");
    QFont font = QFont("Red Right Hand", 12, 1);
    setFont(font);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_login_clicked()
{
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
        Menu_partida *menu = new Menu_partida;
        menu->show();
        close();
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
