#include "widget.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QFontDatabase>
#include <QMediaPlayer>
#include <QMediaPlaylist>

QMediaPlayer * musica = new QMediaPlayer;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    musica->setMedia(QUrl("qrc:/Musica/Menu.mp3"));
    musica->setVolume(50);
    musica->play();

    QFontDatabase::addApplicationFont(":/Font/Red Right Hand.ttf");
    QFont *font = new QFont("Red Right Hand", 14, 1);
    a.setFont(*font);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/Imagenes/LOGO.png").scaled(406,610));
    splash->show();

    Widget w;
    QTimer::singleShot(5000,splash,SLOT(close()));
    QTimer::singleShot(5500,&w,SLOT(show()));

    return a.exec();
}
