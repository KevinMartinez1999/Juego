#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <QRegExpValidator>
#include <QMovie>
#include <QLabel>
#include <QMediaPlayer>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_login_clicked();

    void on_registrarse_clicked();

    void on_mostrar_stateChanged(int arg1);

private:
    Ui::Widget *ui;
    QPixmap Pixmap_Cursor;
    QCursor cursor;
    QMovie *movie;
    QMediaPlayer boton;
    QLabel *w;
    QRegExp rx;
    QRegExpValidator * val;
};
#endif // WIDGET_H
