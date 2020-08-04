#include "jugadorbatalla.h"
#include "niveles.h"
#include "bolafuego.h"

#define g 9.81

JugadorBatalla::JugadorBatalla(QObject *parent) : QObject(parent)
{
    //Variables fisicas
    vx=5;
    xFinal = 0;
    t = 0;
    lim = 930;

    //Variables para la animacion del personaje
    columnas = 0;
    fila = 0;
    ancho = 168;
    alto  = 168;

    box.setRect(0,0,50, 140);

    //Inicializacion de banderas de movimiento
    banLeft = false;
    banRight = false;
    banAttack = false;
    banSpell = false;
    banJump = false;
    TiempoHechizo=true;
    muerto=false;
    ultimoEstado = 1;
    posAnterior = QPoint(0,0);

    //Vida del jugador
    health=120;

    //Timers para los slots de movimiento y fisicas del jugador
    connect(&mov, SIGNAL(timeout()), this, SLOT(moveLeft()));
    connect(&mov, SIGNAL(timeout()), this, SLOT(moveRight()));
    connect(&mov, SIGNAL(timeout()), this, SLOT(Jump()));
    connect(&mov, SIGNAL(timeout()), this, SLOT(setX()));
    connect(&mov, SIGNAL(timeout()), this, SLOT(Attack()));
    connect(&mov, SIGNAL(timeout()), this, SLOT(Spell()));
    connect(&mov, SIGNAL(timeout()), this, SLOT(pos()));
    mov.start(30);

    /*Este timer nos permitira la constante actualizacion de la imagen de nuestro jugador*/
    connect(&timer,SIGNAL(timeout()),this,SLOT(Actualizacion()));
    timer.start(150);
    //Barra de vida del jugador
    vida.setRect(0,0,health,40);
    vida.setBrush(Qt::red);

    //Sonido del lanzamiento de la bola de fuego
    Hechizo.setMedia(QUrl("qrc:/Musica/FUEGO.wav"));
    Hechizo.setVolume(100);

    //Sonido de ataque
    Ataque.setMedia(QUrl("qrc:/Musica/ESPADA.mp3"));
    Ataque.setVolume(80);

    //Sonido de daño
    JugadorAtacado.setMedia(QUrl("qrc:/Musica/PAIN.wav"));
    JugadorAtacado.setVolume(80);
}

void JugadorBatalla::PararTimers()
{
    /*En caso que se pause la partida se deben desactivar todas las banderas y parar los timers*/
    reset_golpe();
    resetBanLeft();
    resetBanRight();
    resetBanSpell();
    resetBanAttack();
    mov.stop();
    timer.stop();
}

void JugadorBatalla::ReiniciarTimers()
{
    //En caso de reanudar la partida se deben reactivar todos los timers del jugador
    mov.start(30);
    timer.start(150);
}

void JugadorBatalla::reset_golpe()
{
    //En caso del jugador moverse se deben resetear las banderas de ataque
    Ataque.stop();
    golpe_izq = false;
    golpe_der = false;
}

QRectF JugadorBatalla::boundingRect() const
{
    /*La funcion propia de qt bodingRect, crea y retorna el rectangulo que conforma la figura del jugador y en el cual pintaremos nuestra
    imagen. -ancho/2,-alto/2 permite que el punto de origen del rectangulo siempre sea le centro de la imagen y no la esquina superior.*/
    return QRectF(-ancho/2,-alto/2,ancho,alto);
}

void JugadorBatalla::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /*Funcion propia de Qt que nos permite dibujar dentro del boundingRect la imagen que queramos, para hacer esto debemos introducirle
    al drawPixmap algunos datos como el punto en el que queremos que empiece a dibujar, el pixmap a dibujar, el ancho y alto de
    lo que se dibujara y tambien, se le pasara constantemente en la funcion actualizar la columna que representara el cuadro 84x84
    que se dibujara en el momento. La variable fila representa el grupo de frames que se quiere realizar dependiendo a las acciones del
    jugador, esta variable cambia cuando el usuario activa un KeyEvent.*/
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-ancho/2,-alto/2,*pixmap,columnas,fila,ancho,alto);
}

/*Este moviemnto es un mov rectilineo uniformemente acelerado y a=dv/dt => v=a*t
derivando la velocidad dx/dt=at:
X=X0+v*t+(1/2)*a*t^2 y esta es la ecacion de moviemnto del jugador; no hay ecuacion para Y porque
no hay moviento en ese eje Y=0*/
void JugadorBatalla::setX(){
    if(fila==0 or fila==168){
        t = 0.3;
        columnas = 0;
    }
    else if(t >= 1.5)
        return;
    xFinal = (vx*t)-(0.5*pow(t,2));
    t += 0.03;
}

void JugadorBatalla::Actualizacion()
{
    /*La imagen sprite del jugador es una imagen que estaba dividida por filas y por columnas, cada fila determina un movimiento o
    accion diferente hecha por el jugador, y las columnas son frames que permiten que esa accion se vea con movimiento, entonces mediante
    un timer estaremos constantemente interactuando en las columnas de determinada fila para asi ir generando una animacion fluida y
    continua.*/
    if ((fila == 672 or fila == 840) and columnas >= 336){
        columnas = 0;
    }
    else if(columnas >= 672)
    {
        columnas = 168;
    }
    else{
        columnas += 168;
    }
    this->update(-ancho/2,-alto/2,ancho,alto);/*La funcion update constantemente actualiza el boundingRect del jugador para que su
    origen siempre sea la mitad de la imagen actual.*/
}

/*LAS SIGUIENTES SON LAS FUNCIONES DE MOVIMIENTO DEL JUGADOR */

//Movimiento hacia la izquierda restringido hasta el borde de la ventana
void JugadorBatalla::   moveLeft()
{
    if (banLeft)
    {
        reset_golpe();
        ultimoEstado = 1;
        if(fila!=336 and fila!=0){
            t = 0.3;
            columnas = 0;
        }
        fila=336;
        if(x()>42){
            setPos(x()-xFinal,y());
            box.setPos(x()-25, y()-50);
        }
    }
}

//Movimiento hacia la derecha restringido hasta que llega dond eel Boss y no lo puede atravesar
//ya que el boss no es intangible
void JugadorBatalla::moveRight()
{
    if (banRight)
    {
        reset_golpe();
        ultimoEstado = 2;
        if(fila!=504 and fila!=168){
            t = 0.3;
            columnas = 0;
        }
        fila = 504;
        if(x()<lim){
            setPos(x()+xFinal,y());
            box.setPos(x()-25, y()-50);
        }
    }
}

/*Para el salto del jugador lo que se hizo fue un movimiento parabolico con un angulo de 90°
asi cuando el jugador esta quieto va a saltar y solo se va a move en el eje Y; asi el jugador va a tener
la libertad de moverse en el aire para facilitar la jugabilidad*/
void JugadorBatalla::Jump()
{
    if(banJump){
        tsalto+=0.3;
        double X, Y;
        //Salto con un angulo de 90°
        X = (15*0*tsalto); //0 => cos(90°)
        Y = (15*1*tsalto)-(0.5*g*pow(tsalto,2)); //1 => sen(90°)

        setPos(x()+X, y()-Y);

        box.setPos(x()-25, y()-50);
        if(y()>=y0){
            setPos(x(),y0);
            box.setPos(x()-25, y()-50);
            banJump=false;
            tsalto=0;
        }
    }
}

/*ependiendo de la posicion anterior el jugador va a hacer una animacion de ataque y tambien
va a poner en alto una bandera, en total son 2 banderas, una para cada lado del jugador;
para asi verificar cuando el jugador ataca por un lado especifico y el Boss le pueda golpear por la espalda
sin ser heridos dandole realidad al golpe*/
void JugadorBatalla::Attack()
{
    if (banAttack){
        if (fila != 672 and fila != 840)
            columnas = 0;
        switch (ultimoEstado) {
        case 1:
            golpe_izq = true;
            fila = 672;
            break;
        case 2:
            golpe_der = true;
            fila = 840;
            break;
        default:
            break;
        }
        if(!muerto)
            Ataque.play();
    }
    if(muerto)
        Ataque.stop();
}

//cuando el jugador presiona la tecla especial va a lanzar una bola de fuego con mov. circular que va hacia el Boss
void JugadorBatalla::Spell()
{   /*Funcion para poder lanzar un hechizo cada 5 segundos*/
    if(TiempoHechizo){//Si ya han pasado los 5 segundos del ultimo ataque
        if(banSpell and !banAttack){
            //Si el jugador no se encuentra atacando con la espada y si presiono la tecla del hechizo
            if (fila != 1008 and fila != 1176)
                columnas = 0;
            switch (ultimoEstado) {
            //Dependiendo de a donde se encontraba mirando el jugador se ejecutara la animacion al cambiar fila
            case 1:
                fila = 1008;
                break;
            case 2:
                fila = 1176;
                break;
            default:
                break;
            }
            if (columnas==672){
                /*Si se llega hasta el final de la animacion se creara una bola de fuego con movimiento circular
                uniforme que colisionara contra el boss y le infligira daño.*/
                Hechizo.play();

                //Se crea la bola de fuego y se le añaden sus posiciones iniciales
                bolaFuego *bola = new bolaFuego(this, ultimoEstado, 1);
                bola->Pixmap = QPixmap(":/Imagenes/BOLAFUEGO.png");
                bola->X = this->x();
                bola->Y = this->y();
                bola->setPos(bola->X,bola->Y);
                scene()->addItem(bola);

                TiempoHechizo=false;

                /*Pasados 5 segundos desde que se lanza la habilidad va a poder ser lanzada de nuevo;
                  dentro de ese intervalo de tiempo sera imposible lanzarla.*/
                QTimer::singleShot(5000,this,SLOT(tiempo()));
                if (ultimoEstado == 1)
                    fila = 0;
                else
                    fila = 168;
            }
        }
    }
}

/*Esta función determina la posicion anterior del personaje para saber si el jugador está quieto o no*/
void JugadorBatalla::pos()
{
    /*Funcion para reiniciar las animaciones del jugador cuando este sin moverse*/
    if (banAttack or banSpell) //Cuando este atacando no debe hacer la animacion de estar quieto
        return;
    if (posAnterior == QPoint(x(),y())){ //Si se cumple es porque el jugador está quieto
        switch (ultimoEstado) {//Se analiza a que lado se estaba moviendo anteriormente el jugador
        case 1:
            fila=0;
            break;
        case 2:
            fila=168;
            break;
        default:
            break;
        }
    }
    posAnterior = QPoint(x(),y());
}
