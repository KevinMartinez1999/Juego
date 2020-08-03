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

    //Sonido al presionar los botones
    boton.setMedia(QUrl("qrc:/Musica/ESPADA.mp3"));
    boton.setVolume(100);

    /*Creacion del cursor del videojuego: con ayuda de QCursor podremos brindarle al cursor la imagen que deseamos.
    Primeramente crearemos un pixmap que contiene la imagen, luego se creara una variable QCursor que recibira la imagen
    y los puntos de eje del click, luego con setCursor establecemos ese cursor para toda la ventana.*/
    Pixmap_Cursor = QPixmap(":/Imagenes/CURSOR.png");
    cursor = QCursor(Pixmap_Cursor,0,0);
    setCursor(cursor);

    /*Sistema de reproducción de gif en el menú:
    Para reproducir un gif primeramente se creara un nuevo QLabel al cual le asignaremos las dimensiones de la ventana, posterior a eso
    crearemos una variable QMovie con el gif a reproducir y también le asignaremos el tamaño de la pantalla, luego con
    la función setMovie le asignaremos al Label que contenga el gif y se reproduzca.*/
    w = new QLabel(this);
    w->resize(440,440);//Tamaño de la ventana.
    movie = new QMovie(this);
    movie->setFileName(":/Imagenes/GIF4.gif");
    movie->setScaledSize(QSize(440,440));//Tamaño de la ventana.
    w->setMovie(movie);//Asignamos el gif al Label.
    movie->start();

    /*Ya que estamos creando el gif por código, siempre aparecerá en la ventana por encima de todos los elementos agregados en el .ui,
    para arreglar esto haremos uso de la función .raise(); que traerá al frente los elementos de la interfaz grafica
    (Labels, botones, lineEdits, etc). Primero traeremos al frente el label Interfaz que contiene la imagen diseñada para el menú,
    y luego se traerá al frente los demás elementos.*/
    ui->Marco->raise();
    ui->Reanudar->raise();
    ui->Controles->raise();
    ui->Cerrar_Sesion->raise();
    ui->Salir->raise();
    ui->ControlesLabel->raise();
    ui->Siguiente->raise();

    /*El label en el que se encuentran las imagenes de los controles y el boton para cambiar de imagen
      por defecto se encontraran escondidos*/
    ui->ControlesLabel->hide();
    ui->Siguiente->hide();
}

MenuPausa::~MenuPausa()
{
    delete ui;
}

void MenuPausa::closeEvent(QCloseEvent *event)
{
    /*En caso de que queramos reanudar la partida habran dos casos, un caso en el que el jugador para salirse
    del menu de pausa desee presionar el boton X de salida de la ventana, para este caso, tambien se deben
    realizar las mismas instrucciones que se harian al presionar el boton de reanudar.
    Es por esto que incluiremos la libreria QCloseEvent junto con la funcion propia de qt closeEvent
    con estos recursos seremos capaces de detectar un evento de cerrado en la ventana y podremos agregar
    las instrucciones que queramos antes de que se confirme el cerrado.*/
    boton.play();
    /*Emitiremos la signal reanudar, que estara conectada a alguna de las dos posibles ventanas pausadas, estas
    ventanas tienen un slot que haran que vuelvan a empezar todos los timers tanto de los jugadores como de los
    enemigos.*/
    emit reanudar();
    event->accept();
}

void MenuPausa::on_Reanudar_clicked()
{
    /*En el caso de que presionemos el boton de reanudar y no la X.*/
    boton.play();
    /*Emitiremos la signal reanudar, que estara conectada a alguna de las dos posibles ventanas pausadas, estas
    ventanas tienen un slot que haran que vuelvan a empezar todos los timers tanto de los jugadores como de los
    enemigos.*/;
    close();
    delete this;
}

void MenuPausa::on_Controles_clicked()
{
    /*Si presionamos el boton de controles se nos mostrara un label en toda la pantalla que nos mostrara los
    controles en el mapa y los controles en las batallas. Tambien nos se nos mostrara un boton que nos permitira
    intercambiar entre estas imagenes.*/
    ui->Siguiente->show();
    ui->ControlesLabel->show();
    ui->ControlesLabel->setPixmap(QPixmap(":/Imagenes/TECLASMAPA.png"));
}

void MenuPausa::on_Cerrar_Sesion_clicked()
{
    musica->stop();
    /*Al cerrar sesion deberemos abrir un nuevo widget y cerrar no solo el menu de pausa sino la ventana pausada
    ya sea mapagameplay o niveles, entonces al momento de crear el menu de pausa se conectara la señal
    cerrar sesion con un slot respectivo de la ventana pausada para asi eliminar y cerrar las dos ventanas al
    mismo tiempo.*/
    Widget *w = new Widget;
    w->show();
    close();
    //Se emite la señal para cerrar la ventana pausada
    emit Cerrar_Sesion();
    delete this;
}

void MenuPausa::on_Salir_clicked()
{
    /*De esta forma podremos cerrar toda la aplicacion de forma segura sin sacar ningun tipo de error ni cortar
    todos los procesos de golpe.*/
    QCoreApplication::quit();
}

void MenuPausa::on_Siguiente_clicked()
{
    /*Con este contador podremos cambiar las dos imagenes que nos mostraran las teclas en la batalla o en el mapa
    cuando es presionado una vez nos mostrara la siguiente imagen, cuando es presionado dos veces escondera
    el label usado y el boton para luego ser llamados otra vez.*/
    contador++;
    if(contador==1){
        ui->ControlesLabel->setPixmap(QPixmap(":/Imagenes/TECLAS.png"));
        ui->Siguiente->setText("Volver");}
    else if(contador==2){
        //Se esconde el label y el boton
        ui->ControlesLabel->hide();
        ui->Siguiente->setText("Siguiente");
        ui->Siguiente->hide();
        contador=0;
    }
}
