#include "widget.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QFontDatabase>
#include <QMediaPlayer>
#include <QMediaPlaylist>

QMediaPlayer * musica;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QMediaPlaylist en la que se guardara la cancion del menu que sonara por las distintas ventanas
    QMediaPlaylist lista;
    lista.addMedia(QUrl("qrc:/Musica/Menu.mp3"));
    lista.setCurrentIndex(0);
    lista.setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    musica = new QMediaPlayer();
    musica->setPlaylist(&lista);
    musica->setVolume(80);
    musica->play();

    /*Esta es la fuente que se usara en el juego, es incluida desde el main para que asi este incluida en
    todas las ventanas de la aplicacion*/
    QFontDatabase::addApplicationFont(":/Font/Red Right Hand.ttf");
    QFont *font = new QFont("Red Right Hand", 14, 1);
    a.setFont(*font);

    //Con ayuda de la libreria QSplashScreen podremos mostrar una pantalla de carga con un logo png diseñado para el juego
    QSplashScreen splash;
    splash.setPixmap(QPixmap(":/Imagenes/LOGO.png").scaled(406,610));
    splash.show();

    Widget w;
    /*singleShot es una funcion que nos permite realizar una funcion determinada en un tiempo especificado.
    Lo que queremos es que por 5 segundos se muestre la imagen de carga y pasado medio segundo se abra la pantalla principal.*/
    QTimer::singleShot(5000,&splash,SLOT(close()));
    QTimer::singleShot(5500,&w,SLOT(show()));

    return a.exec();
}
