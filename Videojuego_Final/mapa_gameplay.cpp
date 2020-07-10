#include "mapa_gameplay.h"
#include "ui_mapa_gameplay.h"

extern int num_jugadores;
extern QString user, pass;

Muro *muro;
Jugador *jugador, *jugador2;

Mapa_GamePlay::Mapa_GamePlay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mapa_GamePlay)
{
    ui->setupUi(this);

    //Esconde el cursor
    QCursor cursor = QCursor(Qt::BlankCursor);
    setCursor(cursor);

    //Musica de fondo
    ambiente = new QMediaPlayer(this);
    ambiente->setMedia(QUrl("qrc:/Musica/ambiente.ogg"));
    ambiente->setVolume(50);
    ambiente->play();

    //Este timer hacer que la cancion de fondo de repita una vez ha terminado
    QTimer * loop = new QTimer(this);
    connect(loop, SIGNAL(timeout()), this, SLOT(iniciar()));
    loop->start(100000);

    nombre = user;
    pj2 = false; //Inicializacion de la variable del segundo jugador por defecto apagado

    //Aqui se añade la escena; la escena es bastante grande ya que el mapa del juego no es una pantalla fija
    //sino que el jugador se mueve por todo el mapa y la escena se actualiza con un timer y se centra en
    //nuestro jugador.
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0,2239,2235);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setScene(escena);

    /*El diseño del mapa esta divido en tres capas; la primera es la de los muros que es MUROS.png
    esta imagen es la que representa las colisiones del jugador con el castillo, las rocas, los arboles y demas objetos.
    La segunda capa es del mapa completo, se incluye la imagen MAPA.png
    sobre la imagen anterior y esta va mostrar lo que el mapa donde nos vamos a mover; La tarcera capa
    es de las estructuras del mapa como castillos, rocas y demas cosas, se implementa con el fin de
    crear una sensasion de pasar justo por debajo de puertas o estar detras de estructuras  y no
    siempre por encima como se veria si solo usamos una imagen completa del mapa.*/

    //NOTA: Para generar dicha sensasion es necesario crear al jugador justo despues de la capa dos, despues
    //del mapa completo y antes del mapa de las estructuras.

    //Primera capa del mapa
    muro = new Muro;
    muro->setPos(0,0);
    escena->addItem(muro);

    //Segunda capa del mapa
    mapa = new QGraphicsPixmapItem;
    mapa->setPos(0,0);
    mapa->setPixmap(QPixmap(":/Imagenes/MAPA.png"));
    escena->addItem(mapa);

    /*Con la variable global num_jugadores que viene de la clase menu_partida sabemos cuantos
    jugadores escogió el usuario y aqui se va a hacer uso de eso; si es 1 se crea un
    solo jugador como se hace normalmente, en caso de ser 2 se crean dos objetos jugadores
    y se hace true la variable pj2 que indica al programa la existencia de un jugador dos en
    la partida, pj2 en true el programa sabe que debe habilitar las teclas de movimiento para
    un segundo jugador*/

    if (num_jugadores == 1){ //Solo un jugador
        jugador = new Jugador(this);
        jugador->pixmap = new QPixmap(":/Imagenes/SPRITEPLAYER.png");//Asignamos el determinado sprite al jugador
        jugador->setPos(770,2155);
        escena->addItem(jugador);
        jugador->crear_hitBox();
    }
    else if (num_jugadores == 2){ //Dos jugadores
        pj2 = true; //Se activa la presencia de un jugador dos en mapa

        //Se crean los dos objetos Jugador en el mapa que van
        //a ser los jugadores 1 y 2

        jugador = new Jugador(this);
        jugador->pixmap = new QPixmap(":/Imagenes/SPRITEPLAYER.png");//Asignamos el determinado sprite al jugador
        jugador->setPos(770,2155);
        escena->addItem(jugador);
        jugador->crear_hitBox();//Se crea el hitbox del primer jugador

        jugador2 = new Jugador(jugador);
        jugador2->pixmap = new QPixmap(":/Imagenes/SPRITEPLAYER.png");//Asignamos el determinado sprite al jugador
        jugador2->setPos(820,2155);
        escena->addItem(jugador2);
        jugador2->crear_hitBox();//Se crea el hitbox del segundo jugador
    }

    //Timer para actualizar la escena y centrarla en el jugador
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(ActualizarEscena()));
    timer->start();

    //Tercera capa del mapa
    objetos = new QGraphicsPixmapItem;
    objetos->setPos(0,0);
    objetos->setPixmap(QPixmap(":/Imagenes/OBJETOS.png"));
    escena->addItem(objetos);
}

Mapa_GamePlay::~Mapa_GamePlay()
{
    delete ui;
}

/*Las siguientes son las funciones del teclado; existe tanto las teclas para el jugador
1 como para el jugador 2, pero en el caso de jugador 2, no puede haber movimiento a menos
que pj2 = true y esto pasa solo si asi lo escoge el usuario en la pantalla anterior del menú,
si no se escoge asi las teclas no van a tener ningun efecto cuando se este jugando solo*/

void Mapa_GamePlay::keyPressEvent(QKeyEvent *event)
{
    //Segun la tecla que se presione se habilita su respectiva bandera de movimiento

    if (event->key() == Qt::Key_W){
        jugador->setBanUp();
    }
    else if (event->key() == Qt::Key_S){
        jugador->setBanDown();
    }
    else if (event->key() == Qt::Key_A){
        jugador->setBanLeft();
    }
    else if (event->key() == Qt::Key_D){
        jugador->setBanRight();
    }

    //Estas son las teclas de movimiento para el jugador 2.
    //Solo estan habilitadas (o habilitadas) si asi lo quiere el usuario.
    else if(event->key()==Qt::Key_J){
        if(pj2)
            jugador2->setBanLeft();
    }
    else if(event->key()==Qt::Key_L){
        if(pj2)
            jugador2->setBanRight();
    }
    else if(event->key()==Qt::Key_I){
        if(pj2)
            jugador2->setBanUp();
    }
    else if(event->key()==Qt::Key_K){
        if(pj2)
            jugador2->setBanDown();
    }
}

/*Esta funcion tambien es para el movimiento pero en este caso detecta las teclas
que se han dejado de presionar; Aqui lo que se hace es darle reset a las banderas
de movimiento para detenerlo*/

void Mapa_GamePlay::keyReleaseEvent(QKeyEvent *event)
{
    //Segun la tecla que se deja de presionar una bandera se baja y el movimiento ahi se detiene

    if (event->key() == Qt::Key_W){
        jugador->resetBanUp();
    }
    else if (event->key() == Qt::Key_S){
        jugador->resetBanDown();
    }
    else if (event->key() == Qt::Key_A){
        jugador->resetBanLeft();
    }
    else if (event->key() == Qt::Key_D){
        jugador->resetBanRight();
    }

    //Estas son las teclas de movimiento para el jugador 2.
    //Solo estan habilitadas (o habilitadas) si asi lo quiere el usuario.
    else if(event->key()==Qt::Key_J){
        if(pj2)
            jugador2->resetBanLeft();
    }
    else if(event->key()==Qt::Key_L){
        if(pj2)
            jugador2->resetBanRight();
    }
    else if(event->key()==Qt::Key_I){
        if(pj2)
            jugador2->resetBanUp();
    }
    else if(event->key()==Qt::Key_K){
        if(pj2)
            jugador2->resetBanDown();
    }
}

//Esta funcion actualiza el mapa constantemente con un timer para
//centrarlo en el jugador y dar la sensasion de que la camara sigue al personaje.

void Mapa_GamePlay::ActualizarEscena()
{
    ui->graphicsView->centerOn(jugador);
}


