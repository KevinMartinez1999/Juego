#ifndef NIVELES_H
#define NIVELES_H

#include <QWidget>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include "jugadorbatalla.h"
#include "boss.h"

namespace Ui {
class Niveles;
}

class Niveles : public QWidget
{
    Q_OBJECT

public:
    explicit Niveles(QWidget *parent = nullptr);
    ~Niveles();
    void NivelSetup();
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::Niveles *ui;
    QGraphicsScene *escena;
    Boss *boss;
    QGraphicsPixmapItem *fondo;
    bool pj2;
};

#endif // NIVELES_H
