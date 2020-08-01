#include "mapa_gameplay.h"
#include "ui_mapa_gameplay.h"
#include "niveles.h"
#include "menu_partida.h"
#include "jugador.h"
#include "menupausa.h"

Jugador *jugador, *jugador2;

extern short int num_jugadores;
extern QString user, pass;

Mapa_GamePlay::Mapa_GamePlay(QWidget *parent, bool nuevaPartida) :
    QWidget(parent), ui(new Ui::Mapa_GamePlay), nueva_partida(nuevaPartida)
{
    ui->setupUi(this);

    srand(time(0));

    EnemigosCreados = 0;

    nivel1 = {QPoint(660,1800), QPoint(220,1680), QPoint(1130,1460), QPoint(485,1470), QPoint(1150,2100)};
    nivel2 = {QPoint(475,920), QPoint(505,460), QPoint(1140,495), QPoint(1105,975), QPoint(1735,1305),
              QPoint(1290,1620), QPoint(1140,1980), QPoint(1325,355)};
    nivel3 = {QPoint(1540,1575), QPoint(1425,1950), QPoint(2170,1880), QPoint(1710,1195), QPoint(2070,1410),
             QPoint(2230,975), QPoint(1860,595), QPoint(2230,435)};

    nombre = user;

    ObjetivosCumplidos = false;

    //Esconde el cursor
    Pixmap_Cursor = QPixmap(":/Imagenes/CURSOR.png");
    cursor = QCursor(Qt::BlankCursor);
    setCursor(cursor);

    //Musica de fondo
    lista.addMedia(QUrl("qrc:/Musica/Ambiente.mp3"));
    lista.setCurrentIndex(0);
    lista.setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    ambiente.setPlaylist(&lista);
    ambiente.setVolume(100);
    ambiente.play();

    //Sonido de los botones
    botonSound.setMedia(QUrl("qrc:/Musica/ESPADA.mp3"));
    botonSound.setVolume(100);
      
    //Timer para actualizar la escena y centrarla en el jugador
    connect(&timer,SIGNAL(timeout()),this,SLOT(ActualizarEscena()));
    timer.start();

    //Verificar la muerte del jugador
    connect(&dead,SIGNAL(timeout()),this,SLOT(verificar_muerte()));
    connect(&dead,SIGNAL(timeout()),this,SLOT(ingreso_batalla()));
    connect(&dead,SIGNAL(timeout()),this,SLOT(Contador_Enemigos()));
    dead.start(100);

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

    //Segunda capa del mapa
    mapa = new QGraphicsPixmapItem;
    mapa->setPixmap(QPixmap(":/Imagenes/MAPA.png"));
    mapa->setPos(0,0);
    escena->addItem(mapa);

    /*Con la variable global num_jugadores que viene de la clase menu_partida sabemos cuantos
    jugadores escogió el usuario y aqui se va a hacer uso de eso; si es 1 se crea un
    solo jugador como se hace normalmente, en caso de ser 2 se crean dos objetos jugadores
    y se hace true la variable pj2 que indica al programa la existencia de un jugador dos en
    la partida, pj2 en true el programa sabe que debe habilitar las teclas de movimiento para
    un segundo jugador*/

    //Spawn de los enemigos
    connect(&enemigos,SIGNAL(timeout()),this,SLOT(spawn()));
    enemigos.start(5000);

    string Usuario, password;
    ifstream file("../Videojuego_Final/Partidas/"+user.toUtf8()+".txt");
    if (!file.is_open()){
        return;}
    file>>Usuario;
    file>>password;
    file>>num_jugadores;
    file>>BossesMuertos;
    file>>ObjetivosCompletados;
    file.close();

    if (num_jugadores == 2){ //Dos jugadores

        //Se crean los dos objetos Jugador en el mapa que van
        //a ser los jugadores 1 y 2

        jugador = new Jugador(this);
        jugador->pixmap = new QPixmap(":/Imagenes/SPRITEPLAYER.png");//Asignamos el determinado sprite al jugador

        jugador2 = new Jugador(this);
        jugador2->pixmap = new QPixmap(":/Imagenes/SPRITEPLAYER2.png");//Asignamos el determinado sprite al jugador

        CargarPartida();

        jugador->setPos(PosX0,PosY0);
        escena->addItem(jugador);
        jugador->box.setPos(x()-15,y()+12);
        jugador->vida.setPos(jugador->x()-30,jugador->y()-50);
        jugador->vida.setZValue(2);

        jugador2->setPos(PosX02,PosY02);
        escena->addItem(jugador2);

        muros = new QGraphicsPixmapItem;
        muros->setPixmap(QPixmap(":/Imagenes/MUROS.png"));
        muros->setPos(0,0);
        jugador2->Muros.push_back(muros);

        jugador2->box.setPos(x()-15,y()+12);
        jugador2->vida.setPos(jugador2->x()-30,jugador2->y()-50);
        jugador2->vida.setZValue(2);
    }
    else{
        jugador = new Jugador(this);
        jugador->pixmap = new QPixmap(":/Imagenes/SPRITEPLAYER.png");//Asignamos el determinado sprite al jugador
        CargarPartida();
        jugador->setPos(PosX0,PosY0);
        escena->addItem(jugador);
        jugador->box.setPos(x()-15,y()+12);
        jugador->vida.setPos(jugador->x()-30,jugador->y()-50);
        jugador->vida.setZValue(2);
    }

    //Capa del mapa en la que se entran el png con las colisiones que seran analizadas por lo jugadores al moverse
    muros = new QGraphicsPixmapItem;
    muros->setPixmap(QPixmap(":/Imagenes/MUROS.png"));
    muros->setPos(0,0);
    jugador->Muros.push_back(muros);

    //Tercera capa del mapa
    objetos = new QGraphicsPixmapItem;
    objetos->setPos(0,0);
    objetos->setPixmap(QPixmap(":/Imagenes/OBJETOS.png"));
    objetos->setZValue(1);
    escena->addItem(objetos);

    //Aviso decorativo que se mostrara al momento de estar en la entrada de una batalla contra un Boss
    aviso = new QLabel(this);
    aviso->setPixmap(QPixmap(":/Imagenes/LETRERO.png"));
    aviso->setScaledContents(true); //Ajusta la imagen al label
    aviso->setGeometry(290,270,200,110);
    aviso->hide();//Por defecto se encontrara escondido para simplemente mostrarse cuando se este en una entrada

    //Boton que permitira al usuario decidir si desea entrar a la batalla contra el Boss o no
    boton.setParent(this);
    boton.setText("Entrar");
    boton.setGeometry(326,326,130,35);
    boton.hide();//Por defecto se encontrara escondido para simplemente mostrarse cuando se este en una entrada
    connect(&boton,SIGNAL(clicked()),this,SLOT(Nivel()));//Se ejecutara la funcion Nivel() si se presiona el boton

    if(BossesMuertos==0 and nueva_partida){
        nivelActual = 0;
        tutorial=true;
    }
    else{
        ui->Controles->hide();
        tutorial=false;
    }

    //Añadir barras de vida
    if (num_jugadores == 2){
        escena->addItem(&jugador->vida);
        escena->addItem(&jugador2->vida);
    }
    else{
        escena->addItem(&jugador->vida);
    }

    //sonidos
    jugadorMuerto.setMedia(QUrl("qrc:/Musica/MUERTO.mp3"));
    jugadorMuerto.setVolume(100);
}

Mapa_GamePlay::~Mapa_GamePlay()
{
    delete ui;
}

void Mapa_GamePlay::CargarPartida()
{
    switch (BossesMuertos) {
    case 0:{
        nivelActual = 0;

        //Se añadira el pixmap a los muros determinados con el que el jugador tendra que colisionar
        muros = new QGraphicsPixmapItem;
        muros->setPixmap(QPixmap(":/Imagenes/BLOQUEO1.png"));
        muros->setPos(0,0);
        escena->addItem(muros);

        jugador->Muros.push_back(muros);

        //Posiciones iniciales del jugador que tendra si se carga una nueva partida
        PosX0=770,PosY0=2155;

        if(num_jugadores==2){
            PosX02=820,PosY02=2155;
            jugador2->Muros.push_back(muros);
        }
        ui->label_2->hide();

        Enemigos_Asesinar=0;
        EnemigosTotales=Enemigos_Asesinar;
        break;
    }

    case 1:{
        nivelActual = 1;

        muros = new QGraphicsPixmapItem;
        muros->setPixmap(QPixmap(":/Imagenes/BLOQUEO2.png"));
        muros->setPos(0,0);
        escena->addItem(muros);

        jugador->Muros.push_back(muros);

        if(ObjetivosCompletados==0){
            PosX0=415,PosY0=2200;

            if(num_jugadores==2){
                PosX02=330,PosY02=2200;
                jugador2->Muros.push_back(muros);
            }

            Enemigos_Asesinar=5;
        }
        else{
            PosX0=755,PosY0=1485;

            if(num_jugadores==2){

                PosX02=815,PosY02=1480;
                jugador2->Muros.push_back(muros);
            }
                Enemigos_Asesinar=0;
        }
        EnemigosTotales=Enemigos_Asesinar;
        break;
    }

    case 2:{
        nivelActual = 2;

        muros = new QGraphicsPixmapItem;
        muros->setPixmap(QPixmap(":/Imagenes/BLOQUEO3.png"));
        muros->setPos(0,0);
        escena->addItem(muros);

        jugador->Muros.push_back(muros);

        if(ObjetivosCompletados==0){
            PosX0=755,PosY0=1485;
            if(num_jugadores==2){
                PosX02=815,PosY02=1480;
                jugador2->Muros.push_back(muros);
            }
            Enemigos_Asesinar=10;
        }
        else{
            PosX0=1625,PosY0=1765;
            if(num_jugadores==2){
                PosX02=1585,PosY02=1765;
                jugador2->Muros.push_back(muros);
            }
            Enemigos_Asesinar=0;
        }
        EnemigosTotales=Enemigos_Asesinar;
        break;
    }

    case 3:{
        nivelActual = 3;
        if(ObjetivosCompletados==0){
            PosX0=1625,PosY0=1765;
            if(num_jugadores==2)
                PosX02=1585,PosY02=1765;
            Enemigos_Asesinar=15;
        }
        else{
            PosX0=2015,PosY0=585;
            if(num_jugadores==2)
                PosX02=2175,PosY02=600;
            Enemigos_Asesinar=0;
        }
        EnemigosTotales=Enemigos_Asesinar;
        break;
    }

    case 4:{
        nivelActual = 4;
        PosX0=2125,PosY0=600;
        if(num_jugadores==2)
            PosX02=2175,PosY02=600;
        Enemigos_Asesinar=0;
        EnemigosTotales=Enemigos_Asesinar;
        break;
        }
    }
}

/*Las siguientes son las funciones del teclado; existe tanto las teclas para el jugador
1 como para el jugador 2, pero en el caso de jugador 2, no puede haber movimiento a menos
que pj2 = true y esto pasa solo si asi lo escoge el usuario en la pantalla anterior del menú,
si no se escoge asi las teclas no van a tener ningun efecto cuando se este jugando solo*/

void Mapa_GamePlay::keyPressEvent(QKeyEvent *event)
{
    //Segun la tecla que se presione se habilita su respectiva bandera de movimiento
    if(!tutorial){

        if(event->key() == Qt::Key_Escape){
            on_Opciones_clicked();//Si presionamos Escape se activara la funcion del boton al ser clickeado
        }
        //Tecla escape destinada para pausar el juego y ver las opciones
        else if (!jugador->muerto){
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
            else if (event->key() == Qt::Key_F){
                jugador->setBanAttack();
            }
        }
        //Estas son las teclas de movimiento para el jugador 2.
        //Solo estan habilitadas (o habilitadas) si asi lo quiere el usuario.
        if (num_jugadores == 2 and !jugador2->muerto){
            if(event->key()==Qt::Key_I){
                jugador2->setBanUp();
            }
            else if(event->key()==Qt::Key_K){
                jugador2->setBanDown();
            }
            else if(event->key()==Qt::Key_J){
                jugador2->setBanLeft();
            }
            else if(event->key()==Qt::Key_L){
                jugador2->setBanRight();
            }
            else if (event->key() == Qt::Key_H){
                jugador2->setBanAttack();
            }
        }
    }
    else{
        if(event->key() == Qt::Key_Space){
            emit Tutorial();
        }
    }

}

/*Esta funcion tambien es para el movimiento pero en este caso detecta las teclas
que se han dejado de presionar; Aqui lo que se hace es darle reset a las banderas
de movimiento para detenerlo*/

void Mapa_GamePlay::keyReleaseEvent(QKeyEvent *event)
{
    //Segun la tecla que se deja de presionar una bandera se baja y el movimiento ahi se detiene

    if (!jugador->muerto){
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
        else if (event->key() == Qt::Key_F){
            jugador->resetBanAttack();
        }
    }

    //Estas son las teclas de movimiento para el jugador 2.
    //Solo estan habilitadas (o habilitadas) si asi lo quiere el usuario.
    if (num_jugadores == 2 and !jugador2->muerto){
        if(event->key()==Qt::Key_I){
            jugador2->resetBanUp();
        }
        else if(event->key()==Qt::Key_K){
            jugador2->resetBanDown();
        }
        else if(event->key()==Qt::Key_J){
            jugador2->resetBanLeft();
        }
        else if(event->key()==Qt::Key_L){
            jugador2->resetBanRight();
        }
        else if (event->key() == Qt::Key_H){
            jugador2->resetBanAttack();
        }
    }
}

void Mapa_GamePlay::EliminarEnemigos(Enemigo *obj, bool v)
{
    if (v){
        Enemigos_Asesinar--;
        if (Enemigos_Asesinar == 0)
            ObjetivosCumplidos = true;
    }
    else{
        EnemigosCreados--;
    }
    listaEnemigos.removeOne(obj);
}

void Mapa_GamePlay::aumentarNivel()
{
    nivelActual++;
}

void Mapa_GamePlay::reanudarTimers()
{
    jugador->ReiniciarTimers();
    if (num_jugadores == 2)
        jugador2->ReiniciarTimers();
    enemigos.start(5000);
    QListIterator<Enemigo *>Iterador(listaEnemigos);
    while(Iterador.hasNext()){
        Iterador.next()->ReiniciarTimers();
    }
}

void Mapa_GamePlay::Tutorial()
{
    cont++;
    if(cont==1)
        ui->Controles->setPixmap(QPixmap(":/Imagenes/TUTORIALMAPA.png"));
    else if(cont==2)
        ui->Controles->setPixmap(QPixmap(":/Imagenes/TECLASMAPA.png"));
    else if(cont==3){
        tutorial=false;
        ui->Controles->hide();
    }
}

void Mapa_GamePlay::Nivel()
{
    botonSound.play();
    ambiente.stop();

    if(Xpos>=325 && Xpos<=405 && YPos>=2193 && YPos<=2215) nivel=0;//Verifica si se esta en la entrada del tutorial
    else if(Xpos>=755 && Xpos<=815 && YPos<=1465 && YPos>=1405) nivel=1;//Verifica si se esta en la entrada del nivel 1
    else if(Xpos>=1565 && Xpos<=1690 && YPos<=1825 && YPos>=1760) nivel=2;//Verifica si se esta en la entrada del nivel 2
    else if(Xpos>=2075 && Xpos<=2200 && YPos<=645 && YPos>=585) nivel=3;//Verifica si se esta en la entrada del nivel 3

    EnemigosCreados=0;

    //Se abre la ventana determinada para las batallas contra Bosses
    Niveles * batalla = new Niveles(nullptr, nivel);
    connect(batalla, &Niveles::aumentar, this, &Mapa_GamePlay::aumentarNivel);
    batalla->show();
    delete jugador;
    if (num_jugadores == 2)
        delete jugador2;
    close();
    delete this;
}

void Mapa_GamePlay::spawn()
{
    if (listaEnemigos.count() == 5 or nivelActual<=0 or EnemigosCreados>=EnemigosTotales or Enemigos_Asesinar==0){ //Maximo 5 enemigos para no colapsar el programa
        return;
    }

    //Estas son las posiciones donde va a aparecer los enemigos en el mapa
    Enemigo * enemigo = new Enemigo(this);
    int num;
    switch (nivelActual) {
    case 1:
        num = 0 + (rand() % nivel1.length());
        enemigo->setPos(nivel1[num]);
        break;
    case 2:
        num = 0 + (rand() % nivel2.length());
        enemigo->setPos(nivel2[num]);
        break;
    case 3:
        num = 0 + (rand() % nivel3.length());
        enemigo->setPos(nivel3[num]);
        break;
    default:
        break;
    }

    //El enemigo se añade a la escena con su barra de vida
    escena->addItem(enemigo);
    enemigo->vida.setPos(enemigo->x(),enemigo->y());
    escena->addItem(&enemigo->vida);
    enemigo->vida.setZValue(2);
    listaEnemigos.append(enemigo); //Se añade a una lista el enemigo para controlar cuando enemigos hay
    EnemigosCreados++;
    connect(enemigo, SIGNAL(EliminarDeLista(Enemigo *, bool)), this, SLOT(EliminarEnemigos(Enemigo *, bool)));
}

void Mapa_GamePlay::ingreso_batalla()
{
    if (jugador->muerto){
        Xpos=jugador2->x();
        YPos=jugador2->y();
    }
    else{
        Xpos=jugador->x();
        YPos=jugador->y();
    }
    /*Si el jugador se encuentra en las posiciones determinadas de las entradas de los niveles se procedera a
    mostrarsele en pantalla un Label con una imagen, el boton de entrada al nivel, y el cursor para que le sea facil
    seleccionar y clickear el boton.*/
    if((Xpos>=325 && Xpos<=405 && YPos>=2193 && YPos<=2215 and nivelActual == 0)or
            (Xpos>=755 && Xpos<=815 && YPos<=1465 && YPos>=1405 and nivelActual == 1)or
            (Xpos>=1565 && Xpos<=1690 && YPos<=1825 && YPos>=1760 and nivelActual == 2)or
            (Xpos>=2075 && Xpos<=2200 && YPos<=645 && YPos>=585 and nivelActual == 3)){
        if(Enemigos_Asesinar==0){
            //Se le muestra al usuario el aviso y el boton para asi seleccionarlo.
            aviso->setPixmap(QPixmap(":/Imagenes/LETRERO.png"));
            aviso->show();
            boton.show();
            //Se le muestra al usuario el cursor personalizado.
            QPixmap Pixmap_Cursor = QPixmap(":/Imagenes/CURSOR.png");
            QCursor cursor = QCursor(Pixmap_Cursor,0,0);
            setCursor(cursor);
        }
        else{
            aviso->setPixmap(QPixmap(":/Imagenes/ADVERTENCIA.png"));
            aviso->show();
        }
    }
    else{
        /*Si el jugador no se encuentra en esas posiciones simplemente se procedera a no mostrarle el aviso y el boton; y
        tambien el cursor seguira siendo invisible*/
        aviso->hide();
        boton.hide();
        cursor = QCursor(Qt::BlankCursor);
        setCursor(cursor);
    }
}

void Mapa_GamePlay::Contador_Enemigos()
{
    if(ObjetivosCumplidos==true){
        fstream file("../Videojuego_Final/Partidas/"+user.toUtf8()+".txt");
        if (!file.is_open())
            return;
        file<<user.toStdString()<<"\n"<<pass.toStdString();
        file<<'\n'<<num_jugadores<<'\n'<<nivelActual<<'\n'<<1;
        file.flush();
        file.close();
        ObjetivosCumplidos=false;
    }
    if(nivelActual!=0)
        ui->Contador->setText(QString::number(Enemigos_Asesinar));
    if(Enemigos_Asesinar==0){
        ui->Contador->hide();
        ui->label_2->hide();
    }

}

/*Esta funcion actualiza el mapa constantemente con un timer para centrarlo en el jugador y dar la
  sensasion de que la camara sigue al personaje.
  Ademas de esto, la funcion verifica si dentro de la escena el personaje se encuentra parado en las determinadas
  entradas a los niveles de batallas contra Bosses, para asi, poder preguntarle si esta listo para entrar.*/

void Mapa_GamePlay::ActualizarEscena()
{
    if (num_jugadores == 2){
        if (jugador->muerto){
            ui->graphicsView->centerOn(jugador2);
        }
        else{
            ui->graphicsView->centerOn(jugador);
        }
    }
    else
        ui->graphicsView->centerOn(jugador);
}

void Mapa_GamePlay::verificar_muerte()
{
    if (num_jugadores == 2){
        if (jugador->muerto and jugador2->muerto){
            jugadorMuerto.play();
            QMessageBox msgBox;
            msgBox.setText("Tu alma ha sido destruida.");
            msgBox.setWindowTitle("HellBurn");
            msgBox.setWindowIcon(QIcon(":/Imagenes/ICONO.png"));
            msgBox.setStyleSheet("background-color:#211b18;"
                                 "color:white;");
            msgBox.exec();

            Mapa_GamePlay *Mapa = new Mapa_GamePlay;
            Mapa->show();

            close();
            delete this;
        }
    }
    else{
        if (jugador->muerto){
            jugadorMuerto.play();
            QMessageBox msgBox;
            msgBox.setText("Tu alma ha sido destruida.");
            msgBox.setWindowTitle("HellBurn");
            msgBox.setWindowIcon(QIcon(":/Imagenes/ICONO.png"));
            msgBox.setStyleSheet("background-color:#211b18;"
                                 "color:white;");
            msgBox.exec();

            Mapa_GamePlay *Mapa = new Mapa_GamePlay;
            Mapa->show();

            close();
            delete this;
        }
    }
}

void Mapa_GamePlay::on_Opciones_clicked()
{
    /*En caso de oprimir la tecla Escape o presionar le boton de opciones en la esquina de la pantalla
    se abrira una nueva ventada opciones por encima de la ventana actual.
    Esta ventana de opciones tiene una cualidad llamada modal que permite que no se pueda interactuar de cualquier
    forma con la ventana de atras si el ui de pausa aun esta abierto, para poder volver a interactuar con el inter
    faz de nivel primero se debe cerrar la ventana de pausa.*/
    jugador->PararTimers();
    if (num_jugadores == 2)
        jugador2->PararTimers();
    QListIterator<Enemigo *>Iterador(listaEnemigos);
    while(Iterador.hasNext()){
        Iterador.next()->PararTimers();
    }
    enemigos.stop();
    MenuPausa *opciones = new MenuPausa(nullptr);
    opciones->show();
    connect(opciones, &MenuPausa::reanudar, this, &Mapa_GamePlay::reanudarTimers);
    connect(opciones,&MenuPausa::Cerrar_Sesion,this,&Mapa_GamePlay::Cerrar_Ventana);
}

void Mapa_GamePlay::Cerrar_Ventana()
{
    close();
    delete jugador;
    if (num_jugadores == 2)
        delete jugador2;
    delete this;
}
