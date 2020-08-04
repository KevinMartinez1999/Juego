#ifndef JUGADORBATALLA_H
#define JUGADORBATALLA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <cmath>
#include <QMediaPlayer>

/*Es clase es para los enemigos que aparecen por el mapa principal*/

class JugadorBatalla: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit JugadorBatalla(QObject *parent = nullptr);

    //Funciones para activar o desactivar las banderas al momento de un KeyEvent
    inline void setBanLeft() {banLeft = true;}
    inline void setBanRight() {banRight = true;}
    inline void resetBanLeft() {banLeft = false;}
    inline void resetBanRight() {banRight = false;}
    inline void setBanAttack() {banAttack = true;}
    inline void resetBanAttack() {banAttack = false;}
    inline void setBanSpell(){banSpell = true;}
    inline void resetBanSpell(){banSpell = false;}
    inline void setBanJump(){banJump = true;}

    //Funciones para determinar las posiciones iniciales del jugador
    void setX0(double X){x0=X;}
    void setY0(double Y){y0=Y;}
    double GetX0(){return x0;}
    double GetY0(){return y0;}
    int lim;

    //Funciones para parar o reiniciar los timers en caso de pausar el juego
    void PararTimers();
    void ReiniciarTimers();

    //Funcion para reiniciar las banderas de ataque en caso de realizar otra accion
    void reset_golpe();

    //Banderas de ataque que seran analizadas por la clase boss para poder infligir el daño
    bool golpe_izq = false;
    bool golpe_der = false;
    bool banAttack;

    //Bandera para determinar si el jugador esta muerto
    bool muerto;

    //Los sig. metodos y atributos son los necesarios para crear las animaciones de movimiento del jugador
    QPixmap *pixmap;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //Rectangulos de la barra de vida y hitBox para colisiones del jugador
    QGraphicsRectItem vida, box;
    int health;//Variable de la vida del jugador

    //Sonido del jugador siendo atacado
    QMediaPlayer JugadorAtacado;


signals:
public slots:
    void Actualizacion();//Actualiza el sprite del jugador dependiendo de la accion que se este haciendo

    //Señales para el movimiento del jugador
    void moveLeft();
    void moveRight();

    //Funcion que aplica el movimiento rectilineo acelerado en el jugador
    void setX();

    //Funcion que aplica el movimiento parabolico al jugador al momento de presionar la tecla.
    void Jump();

    //Funcion para activar las animaciones de ataque con espada y las banderas de ataque
    void Attack();

    //Funcion para activar la animacion de hechizo y generar la bola con MCU
    void Spell();

    //Funcion para reiniciar las animaciones al quedarse quieto
    void pos();

    //Funcion para reiniciar la capacidad de lanzar un hechizo luego de 5 segundos
    void tiempo(){TiempoHechizo=true;}
private:
    //Variables para el control de las animaciones
    float columnas,fila,ancho,alto;

    //Variables a ser aplicadas en las formulas fisicas.
    double x0,y0,xFinal,vx,t,tsalto;

    //Banderas para los hechizos, ataques y movimientos del jugador
    bool TiempoHechizo;
    bool banJump;
    bool banLeft;
    bool banRight;
    bool banSpell;

    short int ultimoEstado;//Variable auxiliar para saber a que lugar se estaba corriendo

    QMediaPlayer Hechizo, Ataque;//Sonidos de los hechizos y ataques

    QPoint posAnterior;//Variable para determinar si el jugador se encuentra quieto

    QTimer mov, timer;//Timers para el movimiento y actualizacion de animaciones del jugador
};

#endif // JUGADORBATALLA_H
