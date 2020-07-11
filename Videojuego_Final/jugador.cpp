#include "jugador.h"
#include "mapa_gameplay.h"

extern Muro *muro;

Jugador::Jugador(QObject *parent) : QObject(parent)
{
    //Inicialzacion de las banderas de movimiento
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
    timer = new QTimer(this);
    columnas = 0;
    fila = 0;

    //Ancho y alto del sprite del jugador
    ancho = 84;
    alto  = 84;
    connect(timer,SIGNAL(timeout()),this,SLOT(Actualizacion()));
    /*Este timer nos permitira la constante actualizacion de la imagen de nuestro jugador*/
    timer->start(150);
}

//Aqui es donde se crea la caa que colisiona con el mapa y se hace invisble en la escena

void Jugador::crear_hitBox()
{
    /*Hitbox es una clase auxiliar que constara de un rect que constantemente actualizara su posicion para estar siempre situado en los
    pies del jugador, el objetivo de esto es que el sprite diseñado es muy grande para nuestro mapa (83x84) entonces hacer las colisiones
    correctamente seria un problema, para esto agregamos un hitbox, esta es una tactica altamente conocida y usada en el mundo de los
    videojuego; Con este rectangulo estamos revisando las colisiones del jugador.*/
    box = new HitBox(this);
    box->setPos(755,2167);
    scene()->addItem(box);
    /*Hacemos uso de la funcion hide() que nos permite ocultar el objeto hitbox pero aun asi tenerlo presente para revisar las colisiones.*/
    box->hide();
}

QRectF Jugador::boundingRect() const
{
    /*La funcion propia de qt bodingRect, crea y retorna el rectangulo que conforma la figura del jugador y en el cual pintaremos nuestra
    imagen. -ancho/2,-alto/2 permite que el punto de origen del rectangulo siempre sea le centro de la imagen y no la esquina superior.*/
    return QRectF(-ancho/2,-alto/2,ancho,alto);
}

void Jugador::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void Jugador::Actualizacion()
{
    /*La imagen sprite del jugador es una imagen que estaba dividida por filas y por columnas, cada fila determina un movimiento o
    accion diferente hecha por el jugador, y las columnas son frames que permiten que esa accion se vea con movimiento, entonces mediante
    un timer estaremos constantemente interactuando en las columnas de determinada fila para asi ir generando una animacion fluida y
    continua.*/
    columnas +=84;
    if(columnas >=336)//El archivo consta de 6 columnas de 84x84, cuando se llegue a la sexta columna se iniciara de nuevo
    {
        columnas =0;
    }
    this->update(-ancho/2,-alto/2,ancho,alto);/*La funcion update constantemente actualiza el boundingRect del jugador para que su
    origen siempre sea la mitad de la imagen actual.*/
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
        fila = 420; //Actualiza el sprite
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
        fila = 504;//Actualiza el sprite
        if(x()<2209){//Condiciones del borde de las escena
            setPos(x()+5,y());//Movimiento del jugador
            box->setPos(x()-10,y()+12);//Movimiento del hiteBox que colisiona
            if (box->collidesWithItem(muro)){//Verifica la colision
                //En este punto con el fin de no intersectar al jugador con los objetos del mapa
                //lo que se hace es retroceder al jugador y a su vez a las caja que lo sigue una
                //vez esta colisiona con alguna part del mapa.
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
        fila = 588;//Actualiza el sprite
        if(y()>0){//Condiciones del borde de las escena
            setPos(x(),y()-5);//Movimiento del jugador
            box->setPos(x()-15,y()+7);//Movimiento del hiteBox que colisiona
            if (box->collidesWithItem(muro)){//Verifica la colision
                //En este punto con el fin de no intersectar al jugador con los objetos del mapa
                //lo que se hace es retroceder al jugador y a su vez a las caja que lo sigue una
                //vez esta colisiona con alguna part del mapa.
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
        fila = 336;//Actualiza el sprite
        if(y()<2205){//Condiciones del borde de las escena
            setPos(x(),y()+5);//Movimiento del hiteBox que colisiona
            box->setPos(x()-15,y()+17);
            if (box->collidesWithItem(muro)){//Verifica la colision
                //En este punto con el fin de no intersectar al jugador con los objetos del mapa
                //lo que se hace es retroceder al jugador y a su vez a las caja que lo sigue una
                //vez esta colisiona con alguna part del mapa.
                setPos(x(),y()-5);
                box->setPos(x(),y()-5);
            }
        }
    }
}
