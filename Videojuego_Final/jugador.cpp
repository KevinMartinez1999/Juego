#include "jugador.h"
#include "mapa_gameplay.h"

#define X 5

Jugador::Jugador(QObject *parent) : QObject(parent)
{
    //Inicialzacion de las banderas de movimiento
    banLeft = false;
    banRight = false;
    banUp = false;
    banDown = false;
    banAttack = false;
    ultimoEstado = 1;
    posAnterior = QPoint(0,0);
    health = 50;
    muerto = false;

    //Ancho y alto del sprite del jugador (inicialización de variables para el sprite)
    ancho = 84;
    alto  = 84;
    columnas = 0;
    fila = 0;

    // Se crea el timer que va a estar asociado al movimiento del jugador
    connect(&timer1, SIGNAL(timeout()), this, SLOT(moveLeft()));
    connect(&timer1, SIGNAL(timeout()), this, SLOT(moveRight()));
    connect(&timer1, SIGNAL(timeout()), this, SLOT(moveUp()));
    connect(&timer1, SIGNAL(timeout()), this, SLOT(moveDown()));
    connect(&timer1, SIGNAL(timeout()), this, SLOT(Attack()));
    connect(&timer1, SIGNAL(timeout()), this, SLOT(pos()));
    timer1.start(30);

    //Timer para las actualización y dibujo del sprite.
    /*Este timer nos permitira la constante actualizacion de la imagen de nuestro jugador*/
    connect(&timer,SIGNAL(timeout()),this,SLOT(Actualizacion()));
    timer.start(200);

    //Se crea el HitBox
    box.setRect(0,0,25,25);

    //Barra de vida del jugador
    vida.setRect(0,0,health,5);
    vida.setBrush(Qt::red);

    //Sonido de ataque
    Ataque = new QMediaPlayer(this);
    Ataque->setMedia(QUrl("qrc:/Musica/ESPADA.mp3"));
    Ataque->setVolume(80);
}

QRectF Jugador::boundingRect() const
{
    /*La funcion propia de qt bodingRect, crea y retorna el rectangulo
     que conforma la figura del jugador y en el cual pintaremos nuestra
     imagen. -ancho/2,-alto/2 permite que el punto de origen del rectangulo
     siempre sea le centro de la imagen y no la esquina superior.*/
    return QRectF(-ancho/2,-alto/2,ancho,alto);
}

void Jugador::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /*Funcion propia de Qt que nos permite dibujar dentro del boundingRect la imagen
     que queramos, para hacer esto debemos introducirle al drawPixmap algunos datos como el
     punto en el que queremos que empiece a dibujar, el pixmap a dibujar, el ancho y alto de
     lo que se dibujara y tambien, se le pasara constantemente en la funcion actualizar la columna
     que representara el cuadro 84x84 que se dibujara en el momento. La variable fila representa
     el grupo de frames que se quiere realizar dependiendo a las acciones del
     jugador, esta variable cambia cuando el usuario activa un KeyEvent.*/
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-ancho/2,-alto/2,*pixmap,columnas,fila,ancho,alto);
}

void Jugador::Actualizacion()
{
    /*La imagen sprite del jugador es una imagen que estaba dividida por filas y por columnas, cada fila
     determina un movimiento o accion diferente hecha por el jugador, y las columnas son frames que
     permiten que esa accion se vea con movimiento, entonces mediante un timer estaremos constantemente
     interactuando en las columnas de determinada fila para asi ir generando una animacion fluida y continua.*/
    if (fila >= 672 and columnas >= 168)
        columnas = 0;
    else if(columnas >= 336)
        //El archivo consta de 6 columnas de 84x84, cuando se llegue a la sexta columna se iniciara de nuevo
        columnas = 84;
    else
        columnas += 84;
    /*La funcion update constantemente actualiza el boundingRect del jugador para que su
     origen siempre sea la mitad de la imagen actual.*/
    this->update(-ancho/2,-alto/2,ancho,alto);
}

//Cuando el jugador se mueve se resetean todas las banderas de ataque en todas
//las posiciones.
void Jugador::reset_golpe()
{
    Ataque->stop();
    golpe_izq = false;
    golpe_der = false;
    golpe_arr = false;
    golpe_aba = false;
}

void Jugador::PararTimers()
{
    reset_golpe();
    resetBanUp();
    resetBanDown();
    resetBanLeft();
    resetBanRight();
    resetBanAttack();
    timer.stop();
    timer1.stop();
}

void Jugador::ReiniciarTimers()
{
    timer.start(200);
    timer1.start(30);
}

/*Las siguientes son las señales de movimiento que funcionan con un timer;
El jugador únicamente se va a mover cuando se respectiva bandera esté activada, por ejemplo,
para moverse a la izquierda, banLeft debe ser true.

una vez se verifica que la respectiva bandera esté activa se le da un valor a Fila que es donde
debe ubicarse en la imagen SPRITEPLAYER.png para hacer las animaciones correctamente.

Las definiciones de cada función son similares en el resto de funciones de movimiento;
cambian en los limites de la escena donde el jugador puede moverse en la funcion setPos()
que se le suma o se le resta dependiendo de hacia donde es el movimiento.*/

void Jugador::moveLeft()
{
    if (banLeft)
    {
        reset_golpe();
        ultimoEstado = 2;
        fila = 420; //Actualiza el sprite
        if(box.x() > 0){ //Condiciones del borde de las escena
            setPos(x()-X,y()); //Movimiento del jugador
            box.setPos(x()-15,y()+12);//Movimiento del hiteBox que colisiona
            vida.setPos(x()-30,y()-50);
            for(auto i=0;i<Muros.size();i++){
                if (box.collidesWithItem(Muros.at(i))){ //Verifica la colision
                    //En este punto con el fin de no intersectar al jugador con los objetos del mapa
                    //lo que se hace es retroceder al jugador y a su vez a las caja que lo sigue una
                    //vez esta colisiona con alguna part del mapa.
                    setPos(x()+X,y());
                    box.setPos(x()-15,y()+12);//Movimiento del hiteBox que colisiona
                    vida.setPos(x()-30,y()-50);
                }
            }
        }
    }
}

//Las definiciones son similares para el resto de las funciones

void Jugador::moveRight()
{
    if (banRight)
    {
        reset_golpe();
        ultimoEstado = 4;
        fila = 504;//Actualiza el sprite
        if(box.x() < 2239-25){//Condiciones del borde de las escena
            setPos(x()+X,y());//Movimiento del jugador
            box.setPos(x()-15,y()+12);//Movimiento del hiteBox que colisiona
            vida.setPos(x()-30,y()-50);
            for(auto i=0;i<Muros.size();i++){
                if (box.collidesWithItem(Muros.at(i))){//Verifica la colision
                    //En este punto con el fin de no intersectar al jugador con los objetos del mapa
                    //lo que se hace es retroceder al jugador y a su vez a las caja que lo sigue una
                    //vez esta colisiona con alguna part del mapa.
                    setPos(x()-X,y());
                    box.setPos(x()-15,y()+12);//Movimiento del hiteBox que colisiona
                    vida.setPos(x()-30,y()-50);
                }
            }
        }
    }
}

void Jugador::moveUp()
{
    if (banUp)
    {
        reset_golpe();
        ultimoEstado = 3;
        fila = 588;//Actualiza el sprite
        if(box.y() > 0){//Condiciones del borde de las escena
            setPos(x(),y()-X);//Movimiento del jugador
            box.setPos(x()-15,y()+12);//Movimiento del hiteBox que colisiona
            vida.setPos(x()-30,y()-50);
            for(auto i=0;i<Muros.size();i++){
                if (box.collidesWithItem(Muros.at(i))){//Verifica la colision
                    //En este punto con el fin de no intersectar al jugador con los objetos del mapa
                    //lo que se hace es retroceder al jugador y a su vez a las caja que lo sigue una
                    //vez esta colisiona con alguna part del mapa.
                    setPos(x(),y()+X);
                    box.setPos(x()-15,y()+12);//Movimiento del hiteBox que colisiona
                    vida.setPos(x()-30,y()-50);
                }
            }
        }
    }
}

void Jugador::moveDown()
{
    if (banDown)
    {
        reset_golpe();
        ultimoEstado = 1;
        fila = 336;//Actualiza el sprite
        if(box.y() < 2235-25){//Condiciones del borde de las escena
            setPos(x(),y()+X);//Movimiento del hiteBox que colisiona
            box.setPos(x()-15,y()+12);//Movimiento del hiteBox que colisiona
            vida.setPos(x()-30,y()-50);
            for(auto i=0;i<Muros.size();i++){
                if (box.collidesWithItem(Muros.at(i))){//Verifica la colision
                    //En este punto con el fin de no intersectar al jugador con los objetos del mapa
                    //lo que se hace es retroceder al jugador y a su vez a las caja que lo sigue una
                    //vez esta colisiona con alguna part del mapa.
                    setPos(x(),y()-X);
                    box.setPos(x()-15,y()+12);//Movimiento del hiteBox que colisiona
                    vida.setPos(x()-30,y()-50);
                }
            }
        }
    }
}

/*ependiendo de la posicion anterior el jugador va a hacer una animacion de ataque y tambien
va a poner en alto una bandera, en total son 4 banderas, una para cada lado del jugador;
para asi verificar cuando el jugador ataca por un lado especifico y los enemigos le puedan golpear por la espalda
sin ser heridos dandole realidad al golpe*/
void Jugador::Attack()
{
    if (banAttack){
        if (fila != 672 and fila != 840 and fila != 756 and fila != 924)
            //Si el jugador no se encuentra atacando se reiniciaran las columnas para que se realize la animacion completa
            columnas = 0;
        switch (ultimoEstado) {
        case 1: //abajo
            golpe_aba = true;
            fila = 672;
            break;
        case 2: //izquierda
            golpe_izq = true;
            fila = 840;
            break;
        case 3: //arriba
            golpe_arr = true;
            fila = 756;
            break;
        case 4: //derecha
            golpe_der = true;
            fila = 924;
            break;
        default:
            break;
        }
        if(!muerto)
            Ataque->play();
    }
    if(muerto)
        Ataque->stop();
}

/*Esta función determina la posicion anterior del personaje para saber si el jugador está quieto*/
void Jugador::pos()
{
    if (banAttack) //Cuando este atacando no debe hacer la animacion de estar quieto
        return;
    if (posAnterior == QPoint(x(),y())){ //Si se cumple es porque el jugador está quieto
        switch (ultimoEstado) {
        case 1:
            fila=0;
            break;
        case 2:
            fila=168;
            break;
        case 3:
            fila=84;
            break;
        case 4:
            fila=252;
            break;
        default:
            break;
        }
    }
    posAnterior = QPoint(x(),y());
}
