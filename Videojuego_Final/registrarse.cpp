#include "registrarse.h"
#include "ui_registrarse.h"
#include "widget.h"

Registrarse::Registrarse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Registrarse)
{
    ui->setupUi(this);
    /*Sistema de reproducción de gif en el menú:
    Para reproducir un gif primeramente se creara un nuevo QLabel al cual le asignaremos las dimensiones de la ventana, posterior a eso
    crearemos una variable QMovie con el gif a reproducir y también le asignaremos el tamaño de la pantalla, luego con
    la función setMovie le asignaremos al Label que contenga el gif y se reproduzca.*/
    QLabel *w = new QLabel(this);
    w->resize(1000,650);//Tamaño de la ventana.
    movie = new QMovie(":/Imagenes/GIF1.gif");
    movie->setScaledSize(QSize(1000,650));//Tamaño de la ventana.
    w->setMovie(movie);//Asignamos el gif al Label.
    movie->start();//Iniciamos la reproducción del gif.
    /*Ya que estamos creando el gif por código, siempre aparecerá en la ventana por encima de todos los elementos agregados en el .ui,
    para arreglar esto haremos uso de la función .raise(); que traerá al frente los elementos de la interfaz grafica
    (Labels, botones, lineEdits, etc). Primero traeremos al frente el label Interfaz que contiene la imagen diseñada para el menú,
    y luego se traerá al frente los demás elementos.*/
    ui->Interfaz->raise();
    ui->clave->raise();
    ui->usuario->raise();
    ui->label->raise();
    ui->label_2->raise();
    ui->registrarse->raise();
    ui->volver->raise();

    /*Creacion del cursor del videojuego: con ayuda de QCursor podremos brindarle al cursor la imagen que deseamos.
    Primeramente crearemos un pixmap que contiene la imagen, luego se creara una variable QCursor que recibira la imagen
    y los puntos de eje del click, luego con setCursor establecemos ese cursor para toda la ventana.*/
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
    movie->stop();//Se detendrá la reproducción de la variable QMovie que contiene al gif del menú para evitar problemas de memoria.
    close();
}

void Registrarse::on_volver_clicked()
{
    /*Si deseamos registrarnos en el sistema, al presionar el botón se procederá a cerrar la ventana actual y se  creara una
    nueva ventana de registro y se abrirá.*/
    Widget *w = new Widget;
    w->show();
    movie->stop();//Se detendrá la reproducción de la variable QMovie que contiene al gif del menú para evitar problemas de memoria.
    close();
}
