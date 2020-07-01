#ifndef REGISTRARSE_H
#define REGISTRARSE_H

#include <QWidget>
#include <fstream>
#include <iostream>
#include <QMessageBox>

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

private:
    Ui::Registrarse *ui;
};

#endif // REGISTRARSE_H