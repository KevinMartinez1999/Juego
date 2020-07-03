#ifndef JUGADOR_H
#define JUGADOR_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QTimer>

class Jugador : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit Jugador(QObject *parent = nullptr);
    inline bool setBanLeft() {return banLeft = true;}
    inline bool setBanRight() {return banRight = true;}
    inline bool setBanUp() {return banUp= true;}
    inline bool setBanDown() {return banDown = true;}
    inline bool resetBanLeft() {return banLeft = false;}
    inline bool resetBanRight() {return banRight = false;}
    inline bool resetBanUp() {return banUp = false;}
    inline bool resetBanDown() {return banDown = false;}

signals:

public slots:
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

private:
    bool banLeft;
    bool banRight;
    bool banUp;
    bool banDown;

};

#endif // JUGADOR_H
