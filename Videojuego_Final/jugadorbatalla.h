#ifndef JUGADORBATALLA_H
#define JUGADORBATALLA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPainter>

class JugadorBatalla: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit JugadorBatalla(QObject *parent = nullptr);
    inline bool setBanLeft() {return banLeft = true;}
    inline bool setBanRight() {return banRight = true;}
    inline bool resetBanLeft() {fila=168; return banLeft = false;}
    inline bool resetBanRight() {fila=252; return banRight = false;}

    QTimer *timer;
    QPixmap *pixmap;
    float columnas,fila,ancho,alto;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:

public slots:
    void Actualizacion(); //Actualiza el sprite
    //Señales para el movimiento del jugador
    void moveLeft();
    void moveRight();

private:
    bool banLeft;
    bool banRight;
};

#endif // JUGADORBATALLA_H
