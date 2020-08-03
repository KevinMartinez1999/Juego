#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <fstream>
#include <iostream>
#include <QRegExpValidator>
#include <QMovie>
#include <QLabel>
#include <QMediaPlayer>
#include <QDir>
#include <QCompleter>
#include <QMessageBox>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
    /*Esta clase contiene distintas librerias que nos permitiran personalizar la ventana con un gif y con los
    diseños de la interfaz, para no repetir estas librerias y variables en otra ventanas, esta clase sera heredada
    por las demas para que simplemente hagan uso de las variables y librerias como se desee.*/
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QMediaPlayer boton;//Sonido para los botones.

    QPixmap Pixmap_Cursor;//Imagen del cursor que se usara en la ventana.
    QCursor cursor;//Variable QCursor que usaremos para aplicar el cursor personalizado en las ventanas.

    /*Variable en la que almacenaremos la ruta de la carpeta de las partidas, es necesaria que sea un QBytearray
    para que sea compatible con otros elementos como el usuario que es un qstring que se convertira con el .toUtf8*/
    QByteArray ruta;

    QMovie *movie;//Movie en el que se almacenara el gif de fondo.
    QLabel *w;//QLabel que permitira mostrar en la escena el QMovie y reproducirlo

    /*Variables QRegExp que permitiran crear una lista de caracteres que queramos que no sean validos en el registro
    e inicio de sesion, primero se guardara la lista y posteriormente se creara el Validator para ser aplicado
    a los lineedits que existan.*/
    QRegExp rx;
    QRegExpValidator * val;

private slots:
    /*Boton que recogera los datos ingresados por el usuario y procedera a verificar si son validos, en
    caso de que lo sean se iniciara sesion y se le permitira al usuario cargar partida o crear una nueva.*/
    void on_login_clicked();

    /*Boton que abrira la nueva ventana que nos permitira registrarnos en el sistema.*/
    void on_registrarse_clicked();

    /*CheckButton que nos permitira cambiar de la forma de mostrar la contraseña como letras o como asteriscos.*/
    void on_mostrar_stateChanged(int arg1);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
