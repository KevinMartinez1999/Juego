#include "jugador.h"
#include "mapa_gameplay.h"

extern int Fila;
extern Muro *muro;

Jugador::Jugador(QObject *parent) : QObject(parent)
{
    banLeft = false;
    banRight = false;
    banUp = false;
    banDown = false;

    // Se crea el timer que va a estar asociado al movimiento del jugador

    QTimer *timer1 = new QTimer;
    connect(timer1, SIGNAL(timeout()), this, SLOT(moveLeft()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(moveRight()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(moveUp()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(moveDown()));
    timer1->start(25);

    //Timer para las actualización y dibujo del sprite.

    timer = new QTimer();
    columnas = 0;
    pixmap = new QPixmap(":/Imagenes/SPRITEPLAYER.png");
    ancho = 84;
    alto  = 84;
    connect(timer,SIGNAL(timeout()),this,SLOT(Actualizacion()));
    timer->start(150);
}

//AQui es donde se crea la caa que colisiona con el mapa y se hace invisble en la escena

void Jugador::crear_hiteBox()
{
    box = new HiteBox(this);
    box->setRect(0,0,30,30);
    box->setBrush(QBrush(Qt::transparent));
    box->setPos(755,2167);
    scene()->addItem(box);
    box->hide();
}

QRectF Jugador::boundingRect() const
{
    return QRectF(-ancho/2,-alto/2,ancho,alto);
}

void Jugador::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(-ancho/2,-alto/2,*pixmap,columnas,Fila,ancho,alto);
}

void Jugador::Actualizacion()
{
    columnas +=84;
    if(columnas >=336)
    {
        columnas =0;
    }
    this->update(-ancho/2,-alto/2,ancho,alto);
}

//Las siguientes son las señales de movimiento que funcionan con un timer;
//El jugador únicamente se va a mover cuando se respectiva bandera esté activada, por ejemplo,
//para moverse a la izquierda, banLeft debe ser true.

//una vez se verifica que la respectiva bandera esté activa se le da un valor a Fila que es donde
//debe ubicarse en la imagen SPRITEPLAYER.png para hacer las animaciones correctamente.

// Las definiciones de cada función son similares en el resto de funciones de movimiento;
// cambian en los limites de la escena donde el jugador puede moverse en la funcion setPos()
// que se le suma o se le resta dependiendo de hacia donde es el movimiento.

void Jugador::moveLeft()
{

    if (banLeft)
    {
        Fila = 420; //Actualiza el sprite
        if(x()>0){ //Condiciones del borde de las escena
            setPos(x()-5,y()); //Movimiento del jugador
            box->setPos(x()-20,y()+12); //Movimiento del hiteBox que colisiona
            if (box->collidesWithItem(muro)){ //Verifica la colision

                //En este punto con el fin de no intersectar al jugador con los objetos del mapa
                //lo que se hace es retroceder al jugador y a su vez a las caja que lo sigue una
                //vez esta colisiona con alguna part del mapa.

                setPos(x()+5,y());
                box->setPos(x()+5,y());
            }
        }
    }
}

//Las definiciones son similares para el resto de las funciones

void Jugador::moveRight()
{
    if (banRight)
    {
        Fila = 504;
        if(x()<2209){
            setPos(x()+5,y());
            box->setPos(x()-10,y()+12);
            if (box->collidesWithItem(muro)){
                setPos(x()-5,y());
                box->setPos(x()-5,y());
            }
        }
    }
}

void Jugador::moveUp()
{
    if (banUp)
    {
        Fila = 588;
        if(y()>0){
            setPos(x(),y()-5);
            box->setPos(x()-15,y()+7);
            if (box->collidesWithItem(muro)){
                setPos(x(),y()+5);
                box->setPos(x(),y()+5);
            }
        }
    }
}

void Jugador::moveDown()
{
    if (banDown)
    {
        Fila = 336;
        if(y()<2205){
            setPos(x(),y()+5);
            box->setPos(x()-15,y()+17);
            if (box->collidesWithItem(muro)){
                setPos(x(),y()-5);
                box->setPos(x(),y()-5);
            }
        }
    }
}
