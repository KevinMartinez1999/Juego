#include "bolafuego.h"
#include "boss.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define g 9.81
#define b 0.47
#define pi 3.14159265
#define e 0.7
#define u 0.8
#define G 1
#define T 1
#define es 0.0375

extern Boss *boss;
extern JugadorBatalla *jugadorBatalla, *jugadorBatalla2;
extern short int num_jugadores;

bolaFuego::bolaFuego(QObject *parent, short int estado, short int tipo, short int suelo)
    : QObject(parent), ultimoEstado(estado), Tipo(tipo), SueloNivel(suelo)
{
    //Inicializar constantes del mov
    t = 0;
    m = 5;
    masa = 0.1;
    r = 15;
    w = 7;

    //Dimensiones del sprite
    columnas = 0;

    //crear box
    box.setRect(0,0,10,10);

    /*Aqui dependiendo de que valor sele pasoa la bola por el constructor se va a decidir que tipo
        de movimiento van a hacer las bolas y si va a ser lanzada por el jugador o por el enemigo.
        las bolas tambien tienen un daño especifico, cada bola baja vida distinto a otra*/
    switch (Tipo) {
    case 1:
        ancho = 40;
        alto = 40;
        limiteSprite=160;
        connect(&timer, SIGNAL(timeout()), this, SLOT(move1()));
        connect(&timer, SIGNAL(timeout()), this, SLOT(colision_con_boss()));
        dano = 6;
        break;
    case 2:
        ancho=20;
        alto=56;
        limiteSprite=180;
        connect(&timer, SIGNAL(timeout()), this, SLOT(move2()));
        connect(&timer, SIGNAL(timeout()), this, SLOT(colision_con_jugador()));
        dano = 5;
        break;
    case 3:
        ancho = 40;
        alto = 40;
        limiteSprite=160;
        connect(&timer, SIGNAL(timeout()), this, SLOT(move3()));
        connect(&timer, SIGNAL(timeout()), this, SLOT(colision_con_jugador()));
        dano = 6;
        break;
    case 4:
        ancho = 40;
        alto = 40;
        limiteSprite=160;
        connect(&timer, SIGNAL(timeout()), this, SLOT(move4()));
        connect(&timer, SIGNAL(timeout()), this, SLOT(colision_con_jugador()));
        dano = 8;
        break;
    case 5:
        ancho = 40;
        alto = 40;
        limiteSprite=160;
        connect(&timer, SIGNAL(timeout()), this, SLOT(move5()));
        connect(&timer, SIGNAL(timeout()), this, SLOT(colision_con_jugador()));
        dano = 10;
        break;
    }
    timer.start(30);

    connect(&animacion, SIGNAL(timeout()), this, SLOT(Actualizacion()));
    animacion.start(125);
}

/*La funcion propia de qt bodingRect, crea y retorna el rectangulo
 que conforma la figura del jugador y en el cual pintaremos nuestra
 imagen. -ancho/2,-alto/2 permite que el punto de origen del rectangulo
 siempre sea le centro de la imagen y no la esquina superior.*/
QRectF bolaFuego::boundingRect() const
{
    return QRectF(-ancho/2,-alto/2,ancho,alto);
}

/*Funcion propia de Qt que nos permite dibujar dentro del boundingRect la imagen
 que queramos, para hacer esto debemos introducirle al drawPixmap algunos datos como el
 punto en el que queremos que empiece a dibujar, el pixmap a dibujar, el ancho y alto de
 lo que se dibujara y tambien, se le pasara constantemente en la funcion actualizar la columna
 que representara el cuadro 84x84 que se dibujara en el momento. La variable fila representa
 el grupo de frames que se quiere realizar dependiendo a las acciones del
 jugador, esta variable cambia cuando el usuario activa un KeyEvent.*/
void bolaFuego::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-ancho/2,-alto/2,Pixmap,columnas,0,ancho,alto);
}

/*Para conseguir las escuaciones de movimiento em los ejes X e Y que es como lo maneja Qt
se debe pasar las escuaciones del movimiento a parametricas, el parametro siempre es t*/

/*Cuando se genera una bola con este movimiento la bola va a tener un movimiento
circular uniforme y vava ir hacia el enemigo (esta bola es generada por el jugador).
Para faciltar las ecuaciones en terminos de un parametro t pasamos a coordenadas polares y se toma
un punto en la circunferencias de radio r deseado, en este casi r=15; luego se procede a encontrar
los catetos del triangulo rectangulo que se forma, serian:
* sen(k)=y/r
* cos(k)=x/r
De las anteriores ecuaciones se despeja X e Y y se tiene que
* Y=rsen(k)
* X=rcos(K), donde k es el angulo, pero w=k/t [rad/s], y despejando k
* Y(t)=rsen(wt)
* X(t)=rcos(wt), donde w:cte y t es el parametro.
Y asi ya se tienen las funciones para mover en el plano XY de Qt.
*/
void bolaFuego::move1() //Golpe del jugador (movimiento circular)
{
    t += 0.1;
    if(ultimoEstado==2){
        X += r*cos(w*t) + m;
        Y += -r*sin(w*t);
        setPos(X,Y);}
    else{
        X += r*cos(w*t) - m;
        Y += -r*sin(w*t);
        setPos(X,Y);
        if(x()<-40)
            delete this;
    }
    box.setPos(x()-5, y()-5);
}

/*Para el movimiento dos las bolas siguen una trayectoria en caida libre por lo que no
hay movimiento en el eje X entonces X(t)=0 y solo hay mov. en Y.
Como este movimiento lleva friccion con el aire la ecuacion de toda la vida:
Y=Y0+V0*t-1/2*g*t^2 NO es valida.
Partiendo de la segunda ley de newton F=ma, las dos fuerzas que actuan en el sistema son
el peso y la friccion del aire y la aceleracion es la de la gravedad, entonces sería:
ma=mg+ff, donde ff es la fuerza de friccion con el aire y es -bv, b es el coeficiente de
rozamiento y v la velocidad, y a=dv/dt (primera derivada de la posicion);
resolviendo la ecuacion diferencial: dv/dt = g - (b/m)*v se tiene que:
v=(mg/b)(1 - exp(-bt/m)) y como v=dy/dt => dy/dt=(mg/b)(1 - exp(-bt/m)) y resolviendo la nueva
ecuacion diferencial se tiene que:
Y(t) = (masa*masa*g/(b*b))*((b*t/masa) + exp(-t*b/masa) - 1); y asi tenemos la ecuacion de posicion de una bola en caida libre con friccion con el aire
donde la velocidad limite Vlim=mg/b
*/
void bolaFuego::move2() //Caida libre con rozamiento con el aire
{
    t += 0.1;
    Y += (masa*masa*g/(b*b))*((b*t/masa) + exp(-t*b/masa) - 1);
    setY(Y);
    box.setPos(x()-5, y()-5);
    if (y() > 670)
        delete this;
}

/*Este moviemnto es un mov rectilineo uniformemente acelerado y a=dv/dt => v=a*t
derivando la velocidad dx/dt=at:
X=X0+v*t+(1/2)*a*t^2 y esta es la ecacion de moviemnto de la pelota; no hay ecuacion para Y porque
no hay moviento en ese eje Y=0
*/
void bolaFuego::move3() //Bolas del enemigo con MRUA
{
    t += 0.1;
    X += -((Vx*t) + (0.5*Ax*t*t));
    setX(X);
    box.setPos(x()-5, y()-5);
    if (x() < -40)
        delete this;
}

/*El tiro parabolico tiene las mismas ecuacion del moviemnto acelerado anterior pero con unas
condiciones, en el eje X no hay aceleracion por lo que:
X=v*t pero la velocidad es un vector con ahora componentes en X e Y asi que haciendo potagoras tenemos que
Vx=V*cos(k)
Vy=V*sen(k), donde k es el angulo d disparo
X=V*cos(k)*t
como en Y si hay aceleraciion y es la de la gravedad
Y=V*sen(k)*t - (1/2)*g*t^2
*/
void bolaFuego::move4() //Bolas parabolicas del enemigo
{
    if (y() > SueloNivel){
        setY(SueloNivel-1);
        t = 0.5;
        Vy *= e;
        Vx *= u;
        if (abs(Vx) < 0.1)
            delete this;
    }
    /*Se pone Vx y Vy y no V*cos(k) y Vsen(k) ya que anteriormente se habian declarado
      las variables Vx y Vy de esa forma*/
    t += 0.1;
    X = (Vx*t); // => cos(45°)
    Y = (Vy*t)-(0.5*g*t*t); // => sen(45°)
    setPos(x()-X,y()-Y);
    box.setPos(x()-5, y()-5);
}


/*Este movimiento es planetario entonces F=ma=GMm/r^2 y se cancela la masa y queda:
a=GM/r^2
como estamos en el plano XY esa aceleracion tiene componentes en X e Y.
Haciendi pitagoras tenemos que ax=acos(k) y ay=asen(k).
cos(k)=x/r y sen(k)=y/r asi pues las componentes de la aceleracion en X e Y serian:
ax=GMx/r^3
ay=GMY/r^3
así vx=ax*t
    vy=ay*t
    x=vx*t
    y=vy*t
*/
void bolaFuego::move5() //Movimiento planetario al rededor del Boss
{
    double ax = 0.00, ay = 0.00;
    double r = sqrt(pow(boss->x()-X, 2)+pow(boss->y()-Y, 2));
    ax += G*(50000*es)*(boss->x()-X)/(pow(r, 3));
    ay += G*(50000*es)*(boss->y()-Y)/(pow(r, 3));
    Vx += ax*T;
    Vy += ay*T;
    X += Vx*T;
    Y += Vy*T;
    setPos(X,Y);
    box.setPos(x()-5, y()-5);

    /*Lo siguiente es para el caso que queramos que todos los planetas interactuen
    entre ellos*/

//    double ax = 0.00, ay = 0.00;
//    int len = bolas.size();
//    for (int i = 0; i < len; i++)
//    {
//        if (bolas[i] == 0){
//            setPos(boss->x(), boss->y());
//            return;
//        }
//        if (bolas[i] != this)
//        {
//            double r = sqrt(pow(bolas[i]->X-X, 2)+pow(bolas[i]->Y-Y, 2));
//            ax += G*bolas[i]->masa*(bolas[i]->X-X)/(pow(r, 3));
//            ay += G*bolas[i]->masa*(bolas[i]->Y-Y)/(pow(r, 3));
//        }
//    }
//    Vx += ax*T;
//    Vy += ay*T;
//    X += Vx*T;
//    Y += Vy*T;
//    setPos(X,Y);
//    box.setPos(x()-5, y()-5);
}

/*Esta funcion verifica cuando los ataques del jugador golpean al
 enemigo y le bajan vida; Luego de golpearlo elimina el objeto bolaFuego*/
void bolaFuego::colision_con_boss()
{
    if (abs(x() - boss->x()) < 50){
        boss->health -= dano;
        boss->vida.setRect(0,0,boss->health, 40);
        delete this;
    }
    else if (x() < -10 or x() > 1010)
        delete this;
}

/*Esta funcion se llama cuando el jugador es golpeado por una bola ya sea de las que lanza
 el enemigo o de las que aparecen aleatoriamente pero jamas lo golpea la misma que el jugador dispara*/
void bolaFuego::colision_con_jugador()
{
    if (num_jugadores == 2){
        if(colision(jugadorBatalla) and colision(jugadorBatalla2))
            delete this;
        else if(colision(jugadorBatalla) or colision(jugadorBatalla2))
            delete this;
    }
    else
        if(colision(jugadorBatalla))
            delete this;
}

/*Verifica cuando hay una colision entre el jugador y una bola*/
bool bolaFuego::colision(JugadorBatalla *obj)
{
    if (box.collidesWithItem(&obj->box)){
        if(obj->health>1)
            obj->JugadorAtacado.play();
        obj->health -= dano;
        obj->vida.setRect(0,0,obj->health,40);
        if(!obj->muerto)
            return true;
        else
            return false;
    }
    else
        return false;
}

//Actualiza el Sprite de la bola
void bolaFuego::Actualizacion()
{
    if (columnas >= limiteSprite)
        columnas = 0;
    else
        columnas += ancho;
    update(-ancho/2,-alto/2,ancho,alto);
}


