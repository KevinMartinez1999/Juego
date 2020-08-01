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

public:
    explicit Registrarse(Widget *parent = nullptr);
    ~Registrarse();

private slots:
    void on_registrarse_clicked();
    void on_volver_clicked();

    void on_mostrar_stateChanged(int arg1);

private:
    Ui::Registrarse *ui;
};

#endif // REGISTRARSE_H
