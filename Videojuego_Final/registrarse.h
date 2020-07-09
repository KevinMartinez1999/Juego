#ifndef REGISTRARSE_H
#define REGISTRARSE_H

#include <QWidget>
#include <fstream>
#include <iostream>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QLabel>
#include <QMovie>

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

private:
    Ui::Registrarse *ui;
    QMovie *movie;
};

#endif // REGISTRARSE_H
