#ifndef MENUPAUSA_H
#define MENUPAUSA_H

#include "widget.h"

namespace Ui {
class MenuPausa;
}

class MenuPausa : public Widget
{
    Q_OBJECT
    /*Esta clase hereda a widget, por lo tanto hereda sus atributos principales y librerias, como son las varia-
    bles que nos permiten generar el cursor, los fondos animados con el uso de labels y movies y demas atributos
    para customizar la ventana.*/
public:
    explicit MenuPausa(Widget *parent = nullptr);
    ~MenuPausa();

    /*Funcion que nos permitira ejecutar comandos al momento de que el jugador presione la X para salir de la
    ventana.*/
    void closeEvent(QCloseEvent *event);

signals:
    /*Señal para cerrar sesion que estara conectada a alguna de las dos ventanas principales*/
    void Cerrar_Sesion();

    /*Señal para quitar la pausa que estara conectada a alguna de las dos ventanas principales*/
    void reanudar();

private slots:
    //Boton que nos permitira ver los controles del mapa y de los niveles
    void on_Controles_clicked();

    //Boton para poder cerrar sesion
    void on_Cerrar_Sesion_clicked();

    //Boton para poder reanudar la partida junto con todos los timers
    void on_Reanudar_clicked();

    //Boton para cerrar toda la aplicacion
    void on_Salir_clicked();

    //Boton para poder cambiar entre las distintas imagenes de tutorial
    void on_Siguiente_clicked();

private:
    Ui::MenuPausa *ui;
    //Contador que nos permitirá cambiar las imagenes de teclas cada que presionemos el boton
    int contador;
};

#endif // MENUPAUSA_H
