#ifndef JUGADOR_H
#define JUGADOR_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QMediaPlayer>
#include <QPainter>
#include <cmath>

/*Aqui esta el jugador que se muestra en el mapa*/

class Jugador : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Jugador(QObject *parent = nullptr);

    // Estas funciones son las banderas de movimiento
    inline void setBanLeft() {banLeft = true;}
    inline void setBanRight() {banRight = true;}
    inline void setBanUp() {banUp= true;}
    inline void setBanDown() {banDown = true;}
    inline void setBanAttack() {banAttack = true;}

    inline void resetBanLeft() {banLeft = false;}
    inline void resetBanRight() {banRight = false;}
    inline void resetBanUp() {banUp = false;}
    inline void resetBanDown() {banDown = false;}
    inline void resetBanAttack() {banAttack = false;}

    //Banderas de movimiento y de los golpes
    void reset_golpe();
    bool golpe_izq = false;
    bool golpe_der = false;
    bool golpe_arr = false;
    bool golpe_aba = false;
    bool banAttack;

    QTimer timer, timer1;

    QList <QGraphicsPixmapItem *> Muros;

    QGraphicsRectItem box, vida;
    int health;
    bool muerto;

    void PararTimers();
    void ReiniciarTimers();

    //Los sig. metodos y atributos son los necesarios para crear la animacion de
    //movimiento del jugador
    QPixmap *pixmap;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

public slots:
    void Actualizacion(); //Actualiza el sprite

    //Señales para el movimiento del jugador
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    //Slots que se usaran para poder atacar
    void Attack();
    void pos();


private:
    QMediaPlayer *Ataque;//Sonido de ataque con espada

    //Banderas de movimiento que se activaran con los keyEvent
    bool banLeft;
    bool banRight;
    bool banUp;
    bool banDown;


    short int ultimoEstado, cont = 0;

    //Variable QPoint que nos permitira verificar si el jugador se encuentra quieto para poder atacar
    QPoint posAnterior;

    //Variables destinadas para el manejo del sprite y animaciones
    float columnas,fila,ancho,alto;
};

#endif // JUGADOR_H
