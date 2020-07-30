#ifndef MENUPAUSA_H
#define MENUPAUSA_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QMediaPlayer>
#include <QCloseEvent>
namespace Ui {
class MenuPausa;
}

class MenuPausa : public QWidget
{
    Q_OBJECT

public:
    explicit MenuPausa(QWidget *parent = nullptr,bool ventana=0);
    ~MenuPausa();
    void closeEvent(QCloseEvent *event);
    bool cerrar_sesion;
    bool VentanaPausada;

signals:
    void Cerrar_Sesion();

private slots:

    void on_Controles_clicked();

    void on_Cerrar_Sesion_clicked();

    void on_Reanudar_clicked();

private:
    Ui::MenuPausa *ui;
    QMovie *movie;
    QLabel *w;
    QMediaPlayer boton;
};

#endif // MENUPAUSA_H
