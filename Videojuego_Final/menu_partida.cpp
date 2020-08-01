#include "menu_partida.h"
#include "ui_menu_partida.h"
#include "mapa_gameplay.h"

short int num_jugadores;

extern QString user, pass;
extern QMediaPlayer * musica;

Menu_partida::Menu_partida(Widget *parent) :
    Widget(parent),
    ui(new Ui::Menu_partida)
{
    ui->setupUi(this);

    //Sonido al presionar los botones
    boton.setMedia(QUrl("qrc:/Musica/ESPADA.mp3"));
    boton.setVolume(100);

    /*Sistema de reproducción de gif en el menú:
    Para reproducir un gif primeramente se creara un nuevo QLabel al cual le asignaremos las dimensiones de la ventana, posterior a eso
    crearemos una variable QMovie con el gif a reproducir y también le asignaremos el tamaño de la pantalla, luego con
    la función setMovie le asignaremos al Label que contenga el gif y se reproduzca.*/
    w = new QLabel(this);
    w->resize(1000,650);//Tamaño de la ventana.
    movie = new QMovie(this);
    movie->setFileName(":/Imagenes/GIF2.gif");
    movie->setScaledSize(QSize(1000,650));//Tamaño de la ventana.
    w->setMovie(movie);//Asignamos el gif al Label.
    movie->start();//Iniciamos la reproducción del gif.

    /*Ya que estamos creando el gif por código, siempre aparecerá en la ventana por encima de todos los elementos agregados en el .ui,
    para arreglar esto haremos uso de la función .raise(); que traerá al frente los elementos de la interfaz grafica
    (Labels, botones, lineEdits, etc). Primero traeremos al frente el label Interfaz que contiene la imagen diseñada para el menú,
    y luego se traerá al frente los demás elementos.*/
    ui->interfaz->raise();
    ui->un_jugador->raise();
    ui->dos_jugadores->raise();
    ui->cargar_partida->raise();
    ui->nueva_partida->raise();
    ui->volver->raise();

    /*Creacion del cursor del videojuego: con ayuda de QCursor podremos brindarle al cursor la imagen que deseamos.
    Primeramente crearemos un pixmap que contiene la imagen, luego se creara una variable QCursor que recibira la imagen
    y los puntos de eje del click, luego con setCursor establecemos ese cursor para toda la ventana.*/
    Pixmap_Cursor = QPixmap(":/Imagenes/CURSOR.png");
    cursor = QCursor(Pixmap_Cursor,0,0);
    setCursor(cursor);

    ui->un_jugador->setChecked(true);/*Por defecto siempre estará señalada la opción de un jugador en caso de que el usuario no escoja.*/

    /*A continuacion lo que se hace es verificar si la sesión que se inicio tiene una partida ya
     guardada o no, en caso de no tenerla el boton de cargar partida va a estar inactivo y viceversa.
     lo que hacen las siguientes lineas de codigo es verificar cuantos saltos de linea tiene al archivo que
     contiene los datos de la partida, si solo contiene un salto de linea es porque la partida es nueva,
     de tener mas saltos de linea es porque ya lleva un progreso en la partida*/
    int cont = 0;

    ifstream file("../Videojuego_Final/Partidas/"+user.toUtf8()+".txt");
    if (!file.is_open())
        return;

    while(file.good())
        if(file.get()=='\n')
            cont++;
    file.close();

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
    boton.play(); //Sonido del botón

    delete w;
    delete movie;
    /*Si por alguna razon al entrar al menu de nueva-cargar partida deseamos cerrar sesion y volver al menu principal simplemente
    se cerrara la ventana y se creara un nuevo widget y se abrira.*/
    Widget *w = new Widget;
    w->show();
    close();
}

void Menu_partida::on_nueva_partida_clicked()
{
    boton.play();//Sonido del botón
    musica->pause();
    delete w;
    delete movie;

    /*Cuando le damos a una nueva partida anteriormente seleccionamos cuantas pesonas
     van a jugar, por defecto una, y al iniciar se toman esos datos para saber cuantos
     personajes crear*/
    if(ui->un_jugador->isChecked())
        num_jugadores = 1;
    else if(ui->dos_jugadores->isChecked())
        num_jugadores = 2;

    fstream file("../Videojuego_Final/Partidas/"+user.toUtf8()+".txt");
    if (!file.is_open())
        return;
    file<<user.toStdString()<<"\n"<<pass.toStdString();
    file<<'\n'<<num_jugadores<<'\n'<<"0"<<'\n'<<"0";
    file.flush();
    file.close();

    Mapa_GamePlay *mapa = new Mapa_GamePlay(nullptr, 1);
    mapa->show();
    delete this;
}

void Menu_partida::on_cargar_partida_clicked()
{
    boton.play();//Sonido del botón

    /*Botón en el que se implementara la función que nos permitirá leer los datos guardados del
     jugador y así generar una partida exactamente igual a como se guardó.*/
    musica->pause();
    delete w;
    delete movie;

    Mapa_GamePlay *mapa = new Mapa_GamePlay(nullptr, 0);
    mapa->show();
    delete this;
}
