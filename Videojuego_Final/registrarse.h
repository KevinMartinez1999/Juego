#ifndef REGISTRARSE_H
#define REGISTRARSE_H

#include <QWidget>
#include <fstream>
#include <iostream>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QLabel>
#include <QMovie>
#include <QMediaPlayer>
#include "widget.h"

using namespace std;

namespace Ui {
class Registrarse;
}

class Registrarse : public QWidget
{
    Q_OBJECT

public:
    explicit Registrarse(QWidget *parent = nullptr);
    ~Registrarse();

private slots:
    void on_registrarse_clicked();
    void on_volver_clicked();

    void on_mostrar_stateChanged(int arg1);

private:
    Ui::Registrarse *ui;
    QPixmap Pixmap_Cursor;
    QCursor cursor;
    QLabel *w;
    QMovie *movie;
    QMediaPlayer boton;
    QRegExp rx;
    QRegExpValidator * val;
};

#endif // REGISTRARSE_H
