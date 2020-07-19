#include "boss.h"
#include "niveles.h"

Boss::Boss(QObject *parent,int tipo) : QObject(parent)
{

    fila = 0;
    columnas = 0;
    if(tipo==0){
        ancho = 128;
        alto  = 110;
        limiteSprite=788;
        pixmap = new QPixmap(":/Imagenes/BOSS4.png");
        setPos(890,455);
    }
    else if(tipo==1){
        ancho = 192;
        alto  = 224;
        limiteSprite=1344;
        pixmap = new QPixmap(":/Imagenes/BOSS2.png");
        setPos(900,490);
    }
    else if(tipo==2){
        ancho = 165;
        alto  = 201;
        limiteSprite=825;
        pixmap = new QPixmap(":/Imagenes/BOSS3.png");
        setPos(875,363);
    }
    else if(tipo==3){
        ancho = 320;
        alto  = 288;
        limiteSprite=1600;
        pixmap = new QPixmap(":/Imagenes/BOSS1.png");
        setPos(875,450);
    }

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(Actualizacion()));
    timer->start(100);
}

QRectF Boss::boundingRect() const
{
    /*La funcion propia de qt bodingRect, crea y retorna el rectangulo que conforma la figura del jugador y en el cual pintaremos nuestra
    imagen. -ancho/2,-alto/2 permite que el punto de origen del rectangulo siempre sea le centro de la imagen y no la esquina superior.*/
    return QRectF(-ancho/2,-alto/2,ancho,alto);
}

void Boss::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void Boss::Actualizacion()
{
    /*La imagen sprite del jugador es una imagen que estaba dividida por filas y por columnas, cada fila determina un movimiento o
    accion diferente hecha por el jugador, y las columnas son frames que permiten que esa accion se vea con movimiento, entonces mediante
    un timer estaremos constantemente interactuando en las columnas de determinada fila para asi ir generando una animacion fluida y
    continua.*/
    if(columnas >= limiteSprite)//El archivo consta de 6 columnas de 84x84, cuando se llegue a la sexta columna se iniciara de nuevo
    {
        columnas = 0;
    }
    else{
        columnas += ancho;
    }
    this->update(-ancho/2,-alto/2,ancho,alto);/*La funcion update constantemente actualiza el boundingRect del jugador para que su
    origen siempre sea la mitad de la imagen actual.*/
}
