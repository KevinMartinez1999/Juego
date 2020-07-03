#ifndef MENU_PARTIDA_H
#define MENU_PARTIDA_H

#include <QWidget>
#include <iostream>
#include <fstream>

using namespace std;

namespace Ui {
class Menu_partida;
}

class Menu_partida : public QWidget
{
    Q_OBJECT

public:
    explicit Menu_partida(QWidget *parent = nullptr);
    ~Menu_partida();

private slots:
    void on_volver_clicked();

    void on_nueva_partida_clicked();

    void on_cargar_partida_clicked();

private:
    Ui::Menu_partida *ui;
};

#endif // MENU_PARTIDA_H
