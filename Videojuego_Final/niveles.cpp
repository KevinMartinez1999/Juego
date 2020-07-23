#include "niveles.h"
#include "ui_niveles.h"
#include <mapa_gameplay.h>

extern int nivel;
extern int num_jugadores;
JugadorBatalla *jugadorBatalla, *jugadorBatalla2;
Boss *boss;

Niveles::Niveles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Niveles)
{
    ui->setupUi(this);
    pj2 = false;//Inicializacion de la variable del segundo jugador por defecto apagado

    //Esconde el cursor
    QCursor cursor = QCursor(Qt::BlankCursor);
    setCursor(cursor); 

    //Musica de los niveles
    if(nivel==0)Lista.addMedia(QUrl("qrc:/Musica/MUSICA0.mp3"));
    else if(nivel==1)Lista.addMedia(QUrl("qrc:/Musica/MUSICA1.mp3"));
    else if(nivel==2)Lista.addMedia(QUrl("qrc:/Musica/MUSICA3.mp3"));
    else if(nivel==3)Lista.addMedia(QUrl("qrc:/Musica/MUSICA2.mp3"));
    Lista.setPlaybackMode(QMediaPlaylist::Loop);
    musicaNivel.setPlaylist(&Lista);
    musicaNivel.setVolume(60);
    musicaNivel.play();

    //Añadido de la escena y grafica del nivel
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0,982,632);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setScene(escena);

    //Pixmap fondo que sera modificado dependiendo a que nivel se esta entrando
    fondo = new QGraphicsPixmapItem;
    fondo->setPos(0,0);

    //Añadir a los jugadores
    if (num_jugadores == 1){ //Solo un jugador
        jugadorBatalla = new JugadorBatalla(this);
        jugadorBatalla->pixmap = new QPixmap(":/Imagenes/SPRITEBATALLA.png");//Asignamos el determinado sprite al jugador
    }
    else if (num_jugadores == 2){ //Dos jugadores
        pj2 = true;
        jugadorBatalla = new JugadorBatalla(this);
        jugadorBatalla->pixmap = new QPixmap(":/Imagenes/SPRITEBATALLA.png");//Asignamos el determinado sprite al jugador

        jugadorBatalla2 = new JugadorBatalla(jugadorBatalla);
        jugadorBatalla2->pixmap = new QPixmap(":/Imagenes/SPRITEBATALLA2.png");//Asignamos el determinado sprite al jugador
    }

    //Por defecto la ayuda esta desativada
    ui->Controles->hide();

    /*Funcion nivel setup que contendrá los distintos condicionales que crearan y modificaran los elementos que
    seran añadidos a la escena dependiendo del nivel en que se encuentre.*/
    NivelSetup();

    /*El slot Level_Events se encargará de periodicamente revisar los distintos eventos que puedan ocurrir durante
    la batalla, siendo uno de estos si el Boss ha sido derrotado.*/
    connect(&timer, SIGNAL(timeout()), this, SLOT(Level_Events()));
    timer.start(100);
}

Niveles::~Niveles()
{
    delete ui;
}

void Niveles::NivelSetup()
{
    if(nivel==0){
        //Si el nivel==0 se prepararan en la escena los elementos del tutorial
        fondo->setPixmap(QPixmap(":/Imagenes/TUTORIAL.png").scaled(1132,650));//Fondo del nivel
        //Posiciones iniciales del jugador 1 en el tutorial
        jugadorBatalla->setX0(170);
        jugadorBatalla->setY0(455);
        if(num_jugadores==2){
            //Posiciones iniciales del jugador 2 en el tutorial en caso de que este activado
            jugadorBatalla2->setX0(65);
            jugadorBatalla2->setY0(455);
        }
        ui->Controles->show(); //Se muestra la ayuda
        QTimer::singleShot(500,this,SLOT(Controles()));
    }
    else if(nivel==1){
        //Si el nivel==1 se prepararan en la escena los elementos del primer nivel
        fondo->setPixmap(QPixmap(":/Imagenes/NIVEL1.png").scaled(1000,650));//Fondo del nivel
        //Posiciones iniciales del jugador 1 en el primer nivel
        jugadorBatalla->setX0(170);
        jugadorBatalla->setY0(515); 
        if(num_jugadores==2){
            //Posiciones iniciales del jugador 2 en caso de que este activado
            jugadorBatalla2->setX0(70);
            jugadorBatalla2->setY0(515);    
        }
    }
    else if(nivel==2){
        fondo->setPixmap(QPixmap(":/Imagenes/NIVEL2.png").scaled(1000,650));//Fondo del nivel
        //Posiciones iniciales del jugador 1 en el segundo nivel
        jugadorBatalla->setX0(175);
        jugadorBatalla->setY0(378);   
        if(num_jugadores==2){
            //Posiciones iniciales del jugador 2 en el segundo nivel en caso de que este activado
            jugadorBatalla2->setX0(60);
            jugadorBatalla2->setY0(378);
        }
    }
    else if(nivel==3){
        fondo->setPixmap(QPixmap(":/Imagenes/NIVEL3.png").scaled(1000,650));//Fondo del nivel
        //Posiciones iniciales del jugador 1 en el tercer nivel
        jugadorBatalla->setX0(160);
        jugadorBatalla->setY0(480);
        if(num_jugadores==2){
            //Posiciones iniciales del jugador 2 en el tercer nivel en caso de que este activado
            jugadorBatalla2->setX0(45);
            jugadorBatalla2->setY0(480);  
        }
    }
    escena->addItem(fondo);//Se añade el fondo a la escena luego de ya estar seleccionado cual imagen poner

    boss = new Boss(this,nivel);//Se le pasa al boss el numero del nivel para determinar el tipo de boss que toca
    escena->addItem(boss);//Se añade el boss a la escena
    boss->vida.setPos(742,30);
    escena->addItem(&boss->vida);//Se añade a la escena la barra de vida del Boss

    //Se añade a la escena al jugador con las posiciones previamente seleccionadas
    jugadorBatalla->setPos(jugadorBatalla->GetX0(),jugadorBatalla->GetY0());
    escena->addItem(jugadorBatalla);
    jugadorBatalla->vida.setPos(90,28);
    escena->addItem(&jugadorBatalla->vida);//Se añade la barra de vida del jugador a la escena
    if(num_jugadores==2){
        //En el caso de que haya dos jugadores se añade al jugador2 con las posiciones previamente seleccionadas
        jugadorBatalla2->setPos(jugadorBatalla2->GetX0(),jugadorBatalla2->GetY0());
        escena->addItem(jugadorBatalla2);
        jugadorBatalla2->vida.setPos(146,96);
        escena->addItem(&jugadorBatalla2->vida);//Se añade la barra de vida del jugador a la escena
        /*En el caso de que hayan dos jugadores al label interfaz se le añadira una imagen de interfaz que tiene
        un diseño para las barras de vida de los dos jugadores*/
        ui->Interfaz->setPixmap(QPixmap(":/Imagenes/INTERFAZBOSS2.png"));
    }
    //Si es solo un jugador se añadira el diseño con solo una barra de vida
    else ui->Interfaz->setPixmap(QPixmap(":/Imagenes/INTERFAZBOSS1.png"));
}
/*Las siguientes son las funciones del teclado; existe tanto las teclas para el jugador
1 como para el jugador 2, pero en el caso de jugador 2, no puede haber movimiento a menos
que pj2 = true y esto pasa solo si asi lo escoge el usuario en la pantalla anterior del menú,
si no se escoge asi las teclas no van a tener ningun efecto cuando se este jugando solo*/

void Niveles::keyPressEvent(QKeyEvent *event)
{
    //Segun la tecla que se presione se habilita su respectiva bandera de movimiento
    if (event->key() == Qt::Key_A){
        jugadorBatalla->setBanLeft();
    }
    else if (event->key() == Qt::Key_D){
        jugadorBatalla->setBanRight();
    }
    else if (event->key() == Qt::Key_F){
        jugadorBatalla->setBanAttack();
    }
    else if (event->key() == Qt::Key_C){
        jugadorBatalla->setBanSpell();
    }
    /*Estas son las teclas de movimiento para el jugador 2. Solo estan habilitadas si asi
      lo quiere el usuario.*/
    else if(event->key()==Qt::Key_J){
        if(pj2)
            jugadorBatalla2->setBanLeft();
    }
    else if(event->key()==Qt::Key_L){
        if(pj2)
            jugadorBatalla2->setBanRight();
    }
    else if (event->key() == Qt::Key_H){
        if(pj2)
        jugadorBatalla2->setBanAttack();
    }
    //Tecla escape destinada para pausar el juego y ver las opciones
    else if(event->key() == Qt::Key_Escape){
        emit on_Opciones_clicked();//Si presionamos Escape se activara la funcion del boton al ser clickeado
    }

}

void Niveles::keyReleaseEvent(QKeyEvent *event)
{
    //Segun la tecla que se deja de presionar una bandera se baja y el movimiento ahi se detiene
    if (event->key() == Qt::Key_A){
            jugadorBatalla->resetBanLeft();
        }
    else if (event->key() == Qt::Key_D){
        jugadorBatalla->resetBanRight();
    }
    else if (event->key() == Qt::Key_F){
        jugadorBatalla->resetBanAttack();
    }
    else if (event->key() == Qt::Key_C){
        jugadorBatalla->resetBanSpell();
    }

    else if(event->key()==Qt::Key_J){
        if(pj2)
            jugadorBatalla2->resetBanLeft();
    }
    else if(event->key()==Qt::Key_L){
        if(pj2)
            jugadorBatalla2->resetBanRight();
    }
    else if (event->key() == Qt::Key_H){
        jugadorBatalla2->resetBanAttack();
    }
}

void Niveles::Controles()
{
    ui->Controles->hide();
}

void Niveles::Level_Events()
{
    /*Esta funcion se encarga de verificar si el boss ya ha sido derrotado.
    Al momento de que el boss tenga su vida menor que 0 se activara un bool que indicara a la clase nivel que el
    usuario ha ganado el nivel y ahora puede volver al mapa principal, se abrira una nueva ventana mapa_gameplay
    y se eliminara la ventana del nivel.*/
    if(boss->Boss_Derrotado){
        boss->Boss_Derrotado=false;
        musicaNivel.stop();
        Mapa_GamePlay *mapa=new Mapa_GamePlay;
        mapa->show();
        close();
        delete this;
    }
    else return;
}

void Niveles::on_Opciones_clicked()
{
    /*En caso de oprimir la tecla Escape o presionar le boton de opciones en la esquina de la pantalla
    se abrira una nueva ventada opciones por encima de la ventana actual.
    Esta ventana de opciones tiene una cualidad llamada modal que permite que no se pueda interactuar de cualquier
    forma con la ventana de atras si el ui de pausa aun esta abierto, para poder volver a interactuar con el inter
    faz de nivel primero se debe cerrar la ventana de pausa.*/
    opciones = new MenuPausa;
    opciones->show();
}
