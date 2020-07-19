#ifndef BOSS_H
#define BOSS_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPainter>

class Boss: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Boss(QObject *parent = nullptr,int tipo=0);
    QTimer *timer;
    QPixmap *pixmap;
    float columnas,fila,ancho,alto,limiteSprite;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private slots:
    void Actualizacion();
};

#endif // BOSS_H
