#include "registrarse.h"
#include "ui_registrarse.h"
#include "widget.h"

Registrarse::Registrarse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Registrarse)
{
    ui->setupUi(this);

    QPixmap Pixmap_Cursor = QPixmap(":/Imagenes/CURSOR.png");
    QCursor cursor = QCursor(Pixmap_Cursor,0,0);
    setCursor(cursor);

    QRegExp rx("^[\\w'\\-,.][^_!¡' '?÷?¿/\\+=@#$%ˆ&*(){}|~<>;:[\\]]{2,}$");
    QRegExpValidator * val = new QRegExpValidator(rx, this);
    ui->usuario->setValidator(val);
}

Registrarse::~Registrarse()
{
    delete ui;
}

void Registrarse::on_registrarse_clicked()
{
    QString user, pass;
    user = ui->usuario->text();
    pass = ui->clave->text();
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
