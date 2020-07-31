#ifndef MENUPAUSA_H
#define MENUPAUSA_H

#include <QWidget>
#include <QCloseEvent>
#include "widget.h"

namespace Ui {
class MenuPausa;
}

class MenuPausa : public Widget
{
    Q_OBJECT

public:
    explicit MenuPausa(Widget *parent = nullptr);
    ~MenuPausa();
    void closeEvent(QCloseEvent *event);
    bool cerrar_sesion;

signals:
    void Cerrar_Sesion();
    void reanudar();

private slots:

    void on_Controles_clicked();

    void on_Cerrar_Sesion_clicked();

    void on_Reanudar_clicked();

private:
    Ui::MenuPausa *ui;
};

#endif // MENUPAUSA_H
