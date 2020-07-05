#include "widget.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen *splash=new QSplashScreen;
    splash->setPixmap(QPixmap(":/Imagenes/LOGO.png").scaled(406,610));
    splash->show();
    Widget w;
    QTimer::singleShot(5000,splash,SLOT(close()));
    QTimer::singleShot(5500,&w,SLOT(show()));
    return a.exec();
}
