#ifndef REGISTRARSE_H
#define REGISTRARSE_H

#include "widget.h"

using namespace std;

namespace Ui {
class Registrarse;
}

class Registrarse : public Widget
{
    Q_OBJECT
    /*Esta clase hereda a widget, por lo tanto hereda sus atributos principales y librerias, como son las varia-
    bles que nos permiten generar el cursor, los fondos animados con el uso de labels y movies y demas atributos
    para customizar la ventana.*/
public:
    explicit Registrarse(Widget *parent = nullptr);
    ~Registrarse();

private slots:
    /*Boton que recogera los datos ingresados por el jugador y los registrara en la carpeta de usuarios.*/
    void on_registro_clicked();

    /*Boton que nos permitira regresar al menu principal en caso de que el jugador lo desee.*/
    void on_volver_clicked();

    /*CheckButton que nos permitira cambiar de la forma de mostrar la contraseña como letras o como asteriscos.*/
    void on_mirar_stateChanged(int arg1);

private:
    Ui::Registrarse *ui;
};

#endif // REGISTRARSE_H
