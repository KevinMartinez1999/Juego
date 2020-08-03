#ifndef MENU_PARTIDA_H
#define MENU_PARTIDA_H

#include "widget.h"

using namespace std;

namespace Ui {
class Menu_partida;
}

class Menu_partida : public Widget
{
    Q_OBJECT
    /*Esta clase hereda a widget, por lo tanto hereda sus atributos principales y librerias, como son las varia-
    bles que nos permiten generar el cursor, los fondos animados con el uso de labels y movies y demas atributos
    para customizar la ventana.*/
public:
    explicit Menu_partida(Widget *parent = nullptr);
    ~Menu_partida();

private slots:
    /*Boton que nos permitira volver al menu de inicio de sesion si asi lo deseamos*/
    void on_volver_clicked();

    /*Boton que nos creara una nueva partida tomando en cuenta la cantidad de jugadores que hemos seleccionado*/
    void on_nueva_partida_clicked();

    /*Boton que nos permitira cargar una partida guardada en los archivos .txt si existe.*/
    void on_cargar_partida_clicked();

private:
    Ui::Menu_partida *ui;
};

#endif // MENU_PARTIDA_H
