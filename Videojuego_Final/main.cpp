#include "widget.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/Imagenes/LOGO.png").scaled(406,610));
    splash->show();
    Widget w;
    QTimer::singleShot(5000,splash,SLOT(close()));
    QTimer::singleShot(5500,&w,SLOT(show()));

    QFontDatabase::addApplicationFont(":/Font/Red Right Hand.ttf");
    QFont font = QFont("Red Right Hand", 12, 1);
    a.setFont(font);

    return a.exec();
}
