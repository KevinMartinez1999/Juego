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

    //Rutas que se manejaran dependiendo de si trabajamos el juego por qt o por el ejecutable
    ruta = "../Videojuego_Final/Partidas/"; //Ruta de los archivos en modo Debug
    //ruta = "Partidas/"; //Ruta de los archivos para modo release o ejecutable

    srand(time(0));//Inicializamos el srand para poder hacer uso de el luego.

    //Se inicializa la variable enemigos creados, esta variables no servira para limitar cuantos enemigos se crearan.
    EnemigosCreados = 0;

    /*Estas listas que seran rellenadas con QPoints nos serviran para que en cada estado del mapa o nivel en el que nos
    encontremos los enemigos sean generados en distintas posiciones en el mapa que seran elegidas aleatoriamente de
    estas listas, se tienen una lista por cada nivel.*/
    nivel1 = {QPoint(660,1800), QPoint(220,1680), QPoint(1130,1460), QPoint(485,1470), QPoint(1150,2100)};
    nivel2 = {QPoint(475,920), QPoint(505,460), QPoint(1140,495), QPoint(1105,975), QPoint(1735,1305),
              QPoint(1290,1620), QPoint(1140,1980), QPoint(1325,355)};
    nivel3 = {QPoint(1540,1575), QPoint(1425,1950), QPoint(2170,1880), QPoint(1710,1195), QPoint(2070,1410),
             QPoint(2230,975), QPoint(1860,595), QPoint(2230,435)};

    /*Se inicializa por defecto la variable de objetivos cumplidos en falso, esta variable nos servira para determinar
    si el jugador ya asesino a todos los enemigos necesarios en el mapa.*/
    ObjetivosCumplidos = false;

    //Esconde el cursor ya que en esta ventana no se le dara mucho uso
    Pixmap_Cursor = QPixmap(":/Imagenes/CURSOR.png");
    cursor = QCursor(Qt::BlankCursor);
    setCursor(cursor);

    //Musica de fondo, se agregara a una lista y puesto en modo loop para asi se puede escuchar siempre mientras la ventana
    //este activa
    lista.addMedia(QUrl("qrc:/Musica/Ambiente.mp3"));
    lista.setCurrentIndex(0);
    lista.setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    ambiente.setPlaylist(&lista);
    ambiente.setVolume(100);
    ambiente.play();

    //Sonido de los botones
    botonSound.setMedia(QUrl("qrc:/Musica/ESPADA.mp3"));
    botonSound.setVolume(100);

    //Aqui se añade la escena; la escena es bastante grande ya que el mapa del juego no es una pantalla fija
    //sino que el jugador se mueve por todo el mapa y la escena se actualiza con un timer y se centra en
    //nuestro jugador.
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0,2239,2235);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setScene(escena);

    //Timer para actualizar la escena y centrarla en el jugador
    connect(&timer,SIGNAL(timeout()),this,SLOT(ActualizarEscena()));

    //Verificar la muerte del jugador
    connect(&dead,SIGNAL(timeout()),this,SLOT(verificar_muerte()));
    //Verifica si el jugador esta en alguna de las entradas a los niveles
    connect(&dead,SIGNAL(timeout()),this,SLOT(ingreso_batalla()));
    //Verifica si el jugador asesino a todos los enemigos para asi guardar la partida
    connect(&dead,SIGNAL(timeout()),this,SLOT(Contador_Enemigos()));
    dead.start(100);

    //Timer para el spawn de los enemigos del mapa
    connect(&enemigos,SIGNAL(timeout()),this,SLOT(spawn()));
    enemigos.start(6000);


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

    /*Se abre el archivo .txt del jugador para asi recoger todos los datos guardados
    El archivo de cada jugador estara organizado de la siguiente forma, en las primeras dos lineas estaran guardados
    el nombre y luego la clave del usuario, esto para el control de acceso.
    Luego en la siguiente linea se guardara el numero de jugadores que tiene la partida actual, para asi cargar la
    partida correctamente, luego en la siguiente linea se guarda un numero del 0 al 4, cada uno representando cuantos bosses
    el jugador ha matado, y la variable de objetivosCompletados, con esta variable se verifica si al cargar partida
    el jugador aun tiene que matar todos los enemigos necesarios.*/
    string Usuario, password;
    //Se abre el archivo
    ifstream file(ruta+user.toUtf8()+".txt");
    if (!file.is_open()){
        return;}
    //Se pasan los datos y se cierra el archivo
    file>>Usuario;
    file>>password;
    file>>num_jugadores;
    file>>BossesMuertos;
    file>>ObjetivosCompletados;
    file.close();

    if (num_jugadores == 2){ //Dos jugadores

        //Se crean los dos objetos Jugador en el mapa que van
        //a ser los jugadores 1 y 2

        jugador = new Jugador;
        jugador->pixmap = new QPixmap(":/Imagenes/SPRITEPLAYER.png");//Asignamos el determinado sprite al jugador

        jugador2 = new Jugador;
        jugador2->pixmap = new QPixmap(":/Imagenes/SPRITEPLAYER2.png");//Asignamos el determinado sprite al jugador

        /*La funcion cargar partida se encargara de trabajar con los datos leidos previamente del archivo .txt y
        escogera las posiciones iniciales de los jugadores dependiendo del estado de partida en el que se encuentre*/
        CargarPartida();

        jugador->setPos(PosX0,PosY0);
        escena->addItem(jugador);//Se añade el jugador a la escena

        jugador->box.setPos(x()-15,y()+12);
        /*Se añade la hitbox del jugador a la escena, con esta hitbox verificaremos las colisiones del jugador*/
        jugador->vida.setPos(jugador->x()-30,jugador->y()-50);//Se añade la vida del jugador a la escena
        /*La funcion setZValue nos permite escoger que elementos de la escena queremos que se vean por delante de otros
        entre mayor sea el valor en Z, se mostrara primero que los objetos de menores valores.*/
        jugador->vida.setZValue(4);

        jugador2->setPos(PosX02,PosY02);
        escena->addItem(jugador2);//Se añade al jugador 2 a la escena)

        muros = new QGraphicsPixmapItem;
        muros->setPixmap(QPixmap(":/Imagenes/MUROS.png"));
        muros->setPos(0,0);
        /*Cada jugador tiene una lista propia con la cual estara verificando cada que se mueve si hay una colision
        Entonces a cada jugador se le añadira a la lista la imagen muros que es un png con todos los muros y colisiones
        principales en el mapa, no se añade esta imagen a la escena ya que es solo de utilidad y no estetica.*/
        jugador2->Muros.push_back(muros);

        jugador2->box.setPos(x()-15,y()+12);
        jugador2->vida.setPos(jugador2->x()-30,jugador2->y()-50);//Se añade la vida del jugador a la escena
        /*La funcion setZValue nos permite escoger que elementos de la escena queremos que se vean por delante de otros
        entre mayor sea el valor en Z, se mostrara primero que los objetos de menores valores.*/
        jugador2->vida.setZValue(4);

    }
    else{
        //En el caso de que la partida sea de solo un jugador
        jugador = new Jugador;
        jugador->pixmap = new QPixmap(":/Imagenes/SPRITEPLAYER.png");//Asignamos el determinado sprite al jugador

        /*La funcion cargar partida se encargara de trabajar con los datos leidos previamente del archivo .txt y
        escogera las posiciones iniciales de los jugadores dependiendo del estado de partida en el que se encuentre*/
        CargarPartida();

        jugador->setPos(PosX0,PosY0);
        escena->addItem(jugador);//Se añade el jugador a la escena

        /*Se añade la hitbox del jugador a la escena, con esta hitbox verificaremos las colisiones del jugador*/
        jugador->box.setPos(x()-15,y()+12);
        //Se añade la barra de vida del jugador
        jugador->vida.setPos(jugador->x()-30,jugador->y()-50);
        /*La funcion setZValue nos permite escoger que elementos de la escena queremos que se vean por delante de otros
        entre mayor sea el valor en Z, se mostrara primero que los objetos de menores valores.*/
        jugador->vida.setZValue(4);

    }

    //Capa del mapa en la que se entran el png con las colisiones que seran analizadas por lo jugadores al moverse
    muros = new QGraphicsPixmapItem;
    muros->setPixmap(QPixmap(":/Imagenes/MUROS.png"));
    muros->setPos(0,0);
    /*Cada jugador tiene una lista propia con la cual estara verificando cada que se mueve si hay una colision
    Entonces a cada jugador se le añadira a la lista la imagen muros que es un png con todos los muros y colisiones
    principales en el mapa, no se añade esta imagen a la escena ya que es solo de utilidad y no estetica.*/
    jugador->Muros.push_back(muros);

    /*Tercera capa del mapa, esta imagen es un png que se diseñó para que cumpla la funcion de dar la sensacion de
    perspectiva y vista aerea en el mapa, esta imagen hace que el jugador se sienta y vea mas real en el mapa, brinda
    una gran ambientación.*/
    objetos = new QGraphicsPixmapItem;
    objetos->setPos(0,0);
    objetos->setPixmap(QPixmap(":/Imagenes/OBJETOS.png"));
    objetos->setZValue(3);
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

    /*Este condicional se encarga de verificar si esta es la primera partida del jugador, si es asi entonces se
    procedera a mostrarle al jugador las imagenes del tutorial y a inmovilizarlo para que pueda ver las imagenes primero
    y no distraerse.*/
    if(BossesMuertos==0 and nueva_partida){
        nivelActual = 0;
        tutorial=true;
    }
    else if(BossesMuertos==4){
        /*Este condicional es para analizar si el juego ha sido ganado, si es el caso se procedera a mostrar en un label
        una imagen decorativa para avisarle al jugador que es el fin del juego.*/
        ui->Controles->setPixmap(QPixmap(":/Imagenes/FIN.png"));
    }
    else{
        /*En los casos contrarios se escondera el label que usaremos para poner las imagenes de tutorial y fin del juego*/
        ui->Controles->hide();
        //Si no esta en el tutorial el jugador podra moverse al entrar a la partida
        tutorial=false;
    }

    //Con estos condicionales añadiremos las vidas de los jugadores
    if (num_jugadores == 2){//En el caso de 2 jugadores
        escena->addItem(&jugador->vida);
        escena->addItem(&jugador2->vida);
    }
    else{//En el caso de un jugador
        escena->addItem(&jugador->vida);
    }

    //Sonido de derrota.
    jugadorMuerto.setMedia(QUrl("qrc:/Musica/MUERTO.mp3"));
    jugadorMuerto.setVolume(100);

    //Se inicia el timer que se encarga de centrar la escena en los jugadores
    timer.start(30);
}

Mapa_GamePlay::~Mapa_GamePlay()
{
    delete ui;
}

void Mapa_GamePlay::CargarPartida()
{
    /*Al momento de leer el archivo del jugador, habran unos datos importantes que determinaran como se cargara la partida
    Hay en total 8 puntos de guardado en total, estos puntos se determinaran con un switch con la variable Bosses Muertos.
    */
    switch (BossesMuertos) {
    case 0:{
        //1.BossesMuertos = 0  :Se cargara el tutorial del juego
        nivelActual = 0;

        /*Dependiendo del nivel actual, el jugador tendra unas imagenes que estaran puestas en distintas partes del
        mapa que impediran que le jugador no siga el curso o la ruta de los niveles, es necesario agregar estas imagenes
        a la lista de QGraphicsPixmapItems del jugador con las cuales verificara las colisiones.*/
        muros = new QGraphicsPixmapItem;
        muros->setPixmap(QPixmap(":/Imagenes/BLOQUEO1.png"));
        muros->setPos(0,0);
        escena->addItem(muros);

        jugador->Muros.push_back(muros);//Se añadira las colisiones a la lista del jugador

        //Posiciones iniciales del jugador que tendra si se carga una nueva partida
        PosX0=770,PosY0=2155;

        if(num_jugadores==2){
            PosX02=820,PosY02=2155;
            jugador2->Muros.push_back(muros);//Se añadira las colisiones a la lista del jugador
        }

        Enemigos_Asesinar=0;//En este caso no tiene que eliminar ningun enemigo
        /*La variable enemigosTotales nos permite tener una copia del numero de los enemigos que hay que matar antes de
        que la variable Enemigos_Asesinar sea descontada.*/
        EnemigosTotales=Enemigos_Asesinar;
        break;
    }

    case 1:{
        nivelActual = 1;

        /*Dependiendo del nivel actual, el jugador tendra unas imagenes que estaran puestas en distintas partes del
        mapa que impediran que le jugador no siga el curso o la ruta de los niveles, es necesario agregar estas imagenes
        a la lista de QGraphicsPixmapItems del jugador con las cuales verificara las colisiones.*/
        muros = new QGraphicsPixmapItem;
        muros->setPixmap(QPixmap(":/Imagenes/BLOQUEO2.png"));
        muros->setPos(0,0);
        escena->addItem(muros);

        jugador->Muros.push_back(muros);//Se añadira las colisiones a la lista del jugador

        if(ObjetivosCompletados==0){
            /*2.BossesMuertos = 1 Y ObjetivosCumplidos = 0
            El boss del tutorial fue derrotado pero no se han matado a los enemigos del mapa*/
            PosX0=415,PosY0=2200;

            if(num_jugadores==2){
                PosX02=330,PosY02=2200;
                Enemigos_Asesinar=10;
                jugador2->Muros.push_back(muros);//Se añadira las colisiones a la lista del jugador
            }
            else
                Enemigos_Asesinar=5;
        }
        else{
            /*3.BossesMuertos = 1 Y ObjetivosCumplidos = 1
            El boss del tutorial fue derrotado y ya se han matado a los enemigos del mapa por lo que podremos ingresar al sig nivel
            */
            PosX0=755,PosY0=1485;

            if(num_jugadores==2){
                PosX02=815,PosY02=1480;
                jugador2->Muros.push_back(muros);//Se añadira las colisiones a la lista del jugador
            }
            Enemigos_Asesinar=0;//En este caso no tiene que eliminar ningun enemigo
        }
        /*La variable enemigosTotales nos permite tener una copia del numero de los enemigos que hay que matar antes de
        que la variable Enemigos_Asesinar sea descontada.*/
        EnemigosTotales=Enemigos_Asesinar;
        break;
    }

    case 2:{
        nivelActual = 2;

        /*Dependiendo del nivel actual, el jugador tendra unas imagenes que estaran puestas en distintas partes del
        mapa que impediran que le jugador no siga el curso o la ruta de los niveles, es necesario agregar estas imagenes
        a la lista de QGraphicsPixmapItems del jugador con las cuales verificara las colisiones.*/
        muros = new QGraphicsPixmapItem;
        muros->setPixmap(QPixmap(":/Imagenes/BLOQUEO3.png"));
        muros->setPos(0,0);
        escena->addItem(muros);

        jugador->Muros.push_back(muros);//Se añadira las colisiones a la lista del jugador

        if(ObjetivosCompletados==0){
            /*4.BossesMuertos = 2 Y ObjetivosCumplidos = 0
            El boss del nivel 1 fue derrotado pero no se han matado a los enemigos del mapa*/
            PosX0=755,PosY0=1485;
            if(num_jugadores==2){
                PosX02=815,PosY02=1480;
                Enemigos_Asesinar=15;
                jugador2->Muros.push_back(muros);//Se añadira las colisiones a la lista del jugador
            }
            else
                Enemigos_Asesinar=10;
        }
        else{
            /* 5.BossesMuertos = 2 Y ObjetivosCumplidos = 1
            El boss del nivel 1 fue derrotado y ya se han matado a los enemigos del mapa por lo que podremos ingresar al sig nivel
            */
            PosX0=1625,PosY0=1765;
            if(num_jugadores==2){
                PosX02=1585,PosY02=1765;
                jugador2->Muros.push_back(muros);//Se añadira las colisiones a la lista del jugador
            }
            Enemigos_Asesinar=0;//En este caso no tiene que eliminar ningun enemigo
        }
        /*La variable enemigosTotales nos permite tener una copia del numero de los enemigos que hay que matar antes de
        que la variable Enemigos_Asesinar sea descontada.*/
        EnemigosTotales=Enemigos_Asesinar;
        break;
    }

    case 3:{
        nivelActual = 3;
        if(ObjetivosCompletados==0){
            /*6.BossesMuertos = 3 Y ObjetivosCumplidos = 0
            El boss del nivel 2 fue derrotado pero no se han matado a los enemigos del mapa
            */
            PosX0=1625,PosY0=1765;
            if(num_jugadores==2){
                PosX02=1585,PosY02=1765;
                Enemigos_Asesinar=20;
            }
            else
                Enemigos_Asesinar=15;
        }
        else{
            /*7..BossesMuertos = 3 Y ObjetivosCumplidos = 1
    El boss del nivel 2 fue derrotado y ya se han matado a los enemigos del mapa por lo que podremos ingresar al sig nivel*/
            PosX0=2135,PosY0=600;
            if(num_jugadores==2)
                PosX02=2175,PosY02=600;
            Enemigos_Asesinar=0;//En este caso no tiene que eliminar ningun enemigo
        }
        /*La variable enemigosTotales nos permite tener una copia del numero de los enemigos que hay que matar antes de
        que la variable Enemigos_Asesinar sea descontada.*/
        EnemigosTotales=Enemigos_Asesinar;
        break;
    }

    case 4:{
        /*8.BossesMuertos = 4
        El boss final fue derrotado por lo cual el juego se ha acabado*/
        nivelActual = 4;
        PosX0=2135,PosY0=600;
        if(num_jugadores==2)
            PosX02=2175,PosY02=600;
        Enemigos_Asesinar=0;//En este caso no tiene que eliminar ningun enemigo 
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
    //En el caso de que estemos en el tutorial ninguno de los jugadores podra moverse
    if(!tutorial){
        //Segun la tecla que se presione se habilita su respectiva bandera de movimiento
        if (event->key() == Qt::Key_A){
            if (!jugador->muerto)
                jugador->setBanLeft();
        }
        else if (event->key() == Qt::Key_D){
            if (!jugador->muerto)
                jugador->setBanRight();
        }
        else if (event->key() == Qt::Key_W){
            if (!jugador->muerto)
                jugador->setBanUp();
        }
        else if (event->key() == Qt::Key_S){
            if (!jugador->muerto)
                jugador->setBanDown();
        }
        else if (event->key() == Qt::Key_F){
            if (!jugador->muerto)
                jugador->setBanAttack();
        }
        //Estas son las teclas de movimiento para el jugador 2.
        //Solo estan habilitadas si asi lo quiere el usuario.
        else if(event->key()==Qt::Key_J){
            if (num_jugadores == 2)
                if (!jugador2->muerto)
                    jugador2->setBanLeft();
        }
        else if(event->key()==Qt::Key_L){
            if (num_jugadores == 2)
                if (!jugador2->muerto)
                    jugador2->setBanRight();
        }
        else if(event->key()==Qt::Key_I){
            if (num_jugadores == 2)
                if (!jugador2->muerto)
                    jugador2->setBanUp();
        }
        else if(event->key()==Qt::Key_K){
            if (num_jugadores == 2)
                if (!jugador2->muerto)
                    jugador2->setBanDown();
        }
        else if(event->key()==Qt::Key_H){
            if (num_jugadores == 2)
                if (!jugador2->muerto)
                    jugador2->setBanAttack();
        }
        else if (event->key() == Qt::Key_Escape){
            //Tecla destinada para activar el boton de pausa
            emit on_Opciones_clicked();
        }
    }
    else if(tutorial){
        //Si estamos en el tutorial con espacio podremos cambiar las imagenes
        if(event->key() == Qt::Key_Space){
            Tutorial();
        }
    }

}

/*Esta funcion tambien es para el movimiento pero en este caso detecta las teclas
que se han dejado de presionar; Aqui lo que se hace es darle reset a las banderas
de movimiento para detenerlo*/

void Mapa_GamePlay::keyReleaseEvent(QKeyEvent *event)
{
    //Segun la tecla que se deja de presionar una bandera se baja y el movimiento ahi se detiene
    if(!tutorial){
        //Tecla escape destinada para pausar el juego y ver las opciones
        if (event->key() == Qt::Key_A){
            if (!jugador->muerto)
                jugador->resetBanLeft();
        }
        else if (event->key() == Qt::Key_D){
            if (!jugador->muerto)
                jugador->resetBanRight();
        }
        else if (event->key() == Qt::Key_W){
            if (!jugador->muerto)
                jugador->resetBanUp();
        }
        else if (event->key() == Qt::Key_S){
            if (!jugador->muerto)
                jugador->resetBanDown();
        }
        else if (event->key() == Qt::Key_F){
            if (!jugador->muerto)
                jugador->resetBanAttack();
        }
        //Estas son las teclas de movimiento para el jugador 2.
        //Solo estan habilitadas si asi lo quiere el usuario.
        else if(event->key()==Qt::Key_J){
            if (num_jugadores == 2)
                if (!jugador2->muerto)
                    jugador2->resetBanLeft();
        }
        else if(event->key()==Qt::Key_L){
            if (num_jugadores == 2)
                if (!jugador2->muerto)
                    jugador2->resetBanRight();
        }
        else if(event->key()==Qt::Key_I){
            if (num_jugadores == 2)
                if (!jugador2->muerto)
                    jugador2->resetBanUp();
        }
        else if(event->key()==Qt::Key_K){
            if (num_jugadores == 2)
                if (!jugador2->muerto)
                    jugador2->resetBanDown();
        }
        else if(event->key()==Qt::Key_H){
            if (num_jugadores == 2)
                if (!jugador2->muerto)
                    jugador2->resetBanAttack();
        }
    }
}

void Mapa_GamePlay::changeEvent(QEvent *event)
{
    /*Esta funcion nos permite tener control de si el jugador por alguna razon minimiza la ventana, al minimizarla se
    activara el menu de pausa para evitar algun error.*/
    if(event->type() == QEvent::WindowStateChange){
        if(windowState() == Qt::WindowMinimized){
            emit on_Opciones_clicked();
        }
    }
}

void Mapa_GamePlay::EliminarEnemigos(Enemigo *obj, bool estado)
{
    if (estado){
        /*En el caso de que el enemigo haya sido asesinado se descontara la variable Enemigos_Asesinar y se verificara
        si ese enemigo fue el ultimo enemigo asesinado para decirle al juego que el jugador ya cumplio los objetivos
        y puede ingresar al nivel.*/
        Enemigos_Asesinar--;
        if (Enemigos_Asesinar == 0)
            //Si es el ultimo enemigo
            ObjetivosCumplidos = true;
    }
    else{
        /*El otro caso es en caso de que el enemigo se haya creado muy lejos del jugador, si es el caso se eliminara
        al enemigo y se descontara la variable para asi crear un nuevo enemigo mas cerca.*/
        EnemigosCreados--;
    }
    //Se elimina al enemigo de la lista
    listaEnemigos.removeOne(obj);
}

void Mapa_GamePlay::aumentarNivel()
{
    /*En el caso de que ganemos la batalla contra el boss se aumentara el nivel en el que estamos*/
    nivelActual++;
}

void Mapa_GamePlay::reanudarTimers()
{
    /*Si pausamos la partida al momento de reanudar deberemos activar todos los timers de nuevo,
    este slot al momento de presionar el boton de pausado sera conectado a una señal de menu pausa. por lo que al emitir
    la  señal desde menupausa se activara este slot que activara primero los timers de los dos jugadores, y posteriormente
    activara el timer de spawneo de enemigos y los timers de los enemigos que haya en el mapa actualmente.*/
    //Se activan los timers del jugador 1
    jugador->ReiniciarTimers();
    if (num_jugadores == 2)
        //Si hay dos jugadores se activa los timers del jugador 2
        jugador2->ReiniciarTimers();
    //Se activa el timer de spawneo de enemigos
    enemigos.start(6000);
    //ITeraremos sobre toda la lista para asegurarnos que a cada enemigo existente se le activen sus timers
    QListIterator<Enemigo *>Iterador(listaEnemigos);
    while(Iterador.hasNext()){
        Iterador.next()->ReiniciarTimers();
    }
    timer.start(25);
}

void Mapa_GamePlay::Tutorial()
{
    /*Esta funcion basicamente lo que permite es que al momento del tutorial cada que presionemos la tecla espacio
    se cambie la imagen de tutorial que se este mostrando actualmente. Cuando finalmente se haya presionado el boton
    4 veces se ocultara el label de las imagenes y los jugadores ya se podran mover.*/
    cont++;
    if(cont==1)//Cambio de imagen en el label
        ui->Controles->setPixmap(QPixmap(":/Imagenes/TUTORIALMAPA.png"));
    else if(cont==2)//Cambio de imagen en el label
        ui->Controles->setPixmap(QPixmap(":/Imagenes/TUTORIALMAPA1.png"));
    else if(cont==3)//Cambio de imagen en el label
        ui->Controles->setPixmap(QPixmap(":/Imagenes/TECLASMAPA.png"));
    else if(cont==4){
        //Se quita el label de las imagenes y el jugador puede moverse otra vez
        tutorial=false;
        ui->Controles->hide();
    }
}

void Mapa_GamePlay::Nivel()
{
    /*Al presionar el boton de entrar, se analizara en cual de todas las entradas a los niveles estamos y se
    abrira una nueva ventana niveles que se creara diferente dependiendo del nivel.*/

    //Sonidos del boton
    botonSound.play();
    ambiente.stop();

    if(Xpos>=325 && Xpos<=405 && YPos>=2193 && YPos<=2215) nivel=0;//Verifica si se esta en la entrada del tutorial
    else if(Xpos>=755 && Xpos<=815 && YPos<=1465 && YPos>=1405) nivel=1;//Verifica si se esta en la entrada del nivel 1
    else if(Xpos>=1565 && Xpos<=1690 && YPos<=1825 && YPos>=1760) nivel=2;//Verifica si se esta en la entrada del nivel 2
    else if(Xpos>=2075 && Xpos<=2200 && YPos<=645 && YPos>=585) nivel=3;//Verifica si se esta en la entrada del nivel 3

    EnemigosCreados=0;

    //Se abre la ventana determinada para las batallas contra Bosses
    Niveles * batalla = new Niveles(nullptr, nivel);
    /*El slot aumentar niveles permitira al programa mantener datos al momento de si se gana el nivel o no, en caso
    de que se aumente el nivel el mapa ya sabra que cosas debera cargar y cuales no.*/
    connect(batalla, &Niveles::aumentar, this, &Mapa_GamePlay::aumentarNivel);
    batalla->show();
    //Se borran a los jugadores para evitar problemas de memoria.
    delete jugador;
    if (num_jugadores == 2)
        delete jugador2;
    close();
    delete this;
}

void Mapa_GamePlay::spawn()
{
    /*Funcion que permite la generacion continua de enemigos en el mapa.*/
    if (listaEnemigos.count() == 5 or nivelActual<=0 or EnemigosCreados>=EnemigosTotales or Enemigos_Asesinar==0){ //Maximo 5 enemigos para no colapsar el programa
        /*En el caso en el que ya hayan 5 enemigos a la vez en el mapa no se crearan mas para evitar sobrecargar el mapa
        tambien si se han creado ya los enemigos exactos del nivel no se crearan mas, tambien si el nivel es el tutorial
        no se crearan mas enemigos, o si el jugador ya asesino a todos los enemigos no se crearan mas.*/
        return;
    }
    Enemigo * enemigo = new Enemigo(this);
    int num;
    switch (nivelActual) {
    /*Dependiendo del nivel en el que nos encontremos se escogera de alguna de las listas de QPoints una posicion aleatoria
    en la que se creara el enemigo*/
    case 1:
        //Se escoge una posicion aleatoria
        num = 0 + (rand() % nivel1.length());
        enemigo->setPos(nivel1[num]);
        break;
    case 2:
        //Se escoge una posicion aleatoria
        num = 0 + (rand() % nivel2.length());
        enemigo->setPos(nivel2[num]);
        break;
    case 3:
        //Se escoge una posicion aleatoria
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
    enemigo->vida.setZValue(4);
    /*Se añade a una lista el enemigo para controlar cuando enemigos hay en el mapa al mismo tiempo*/
    listaEnemigos.append(enemigo);
    /*Se aumentara la variable enemigos creados que nos ayuda para controlar que se creen solo los enemigos necesarios
    por nivel*/
    EnemigosCreados++;
    /*Se conecta al nuevo enemigo creado con el slot eliminar enemigo, este slot lo que hace es que al emitir un señal
    desde enemigo se verificara algunas cosas como si el enemigo fue asesinado o fue eliminado por estar muy lejos para
    evitar problemas de memoria, sea cual sea el caso se eliminara al enemigo de la lista de enemigos para asi crear
    mas si es el caso.*/
    connect(enemigo, SIGNAL(EliminarDeLista(Enemigo *, bool)), this, SLOT(EliminarEnemigos(Enemigo *, bool)));
}

void Mapa_GamePlay::ingreso_batalla()
{
    if (jugador->muerto){
        //Si el jugador 1 se encuentra muerto al que se le analizaran las posiciones es al jugador 2
        if(num_jugadores==2){
            Xpos=jugador2->x();
            YPos=jugador2->y();
        }
    }
    else{
        //Si el jugador 1 esta vivo
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
            //Si el jugador aun no ha asesinado a todos los enemigos se le mostrara un aviso recordandole
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
    /*Esta funcion se encarga de verificar si el jugador ya derroto a todos los enemigos del mapa y ya pueden entrar al
    nivel actual;*/
    if(ObjetivosCumplidos==true){
        /*En el caso en el que ya se haya cumplido, se guardara la partida, para que en el caso de que el jugador se
        salga su proceso quede guardado y pueda ingresar directamente al nivel al entrar.*/
        fstream file(ruta+user.toUtf8()+".txt");
        if (!file.is_open())
            return;
        file<<user.toStdString()<<"\n"<<pass.toStdString();
        file<<'\n'<<num_jugadores<<'\n'<<nivelActual<<'\n'<<1;
        file.flush();
        file.close();
        ObjetivosCumplidos=false;
    }

    if(Enemigos_Asesinar==0){
        /*Si el jugador ya asesino a todos los enemigos se le mostrara un aviso diciendole que debe ingresar a la batalla*/
        ui->Contador->hide();
        ui->label_2->setText("   Ingresa a la batalla");
    }
    else if(Enemigos_Asesinar!=0 or nivelActual!=0){
            /*En el caso en el que no se hayan matado a todos los enemigos, en la barra superior se mostrara un contador
            de los enemigos que el jugador tendra que asesinar.*/
            ui->label_2->setText("Enemigos restantes:");
            ui->Contador->setText(QString::number(Enemigos_Asesinar));
    }
}

/*Esta funcion actualiza el mapa constantemente con un timer para centrarlo en el jugador y dar la
  sensasion de que la camara sigue al personaje.
  Ademas de esto, la funcion verifica si dentro de la escena el personaje se encuentra parado en las determinadas
  entradas a los niveles de batallas contra Bosses, para asi, poder preguntarle si esta listo para entrar.*/

void Mapa_GamePlay::ActualizarEscena()
{
    /*Esta funcion se encarga de actualizar la escena y centrarla en el jugador para asi dar esa sensacion de movimiento
    en el mapa*/
    if (num_jugadores == 2){
        if (jugador->muerto){
            /*En el caso que la partida sea de dos jugadores y el jugador 1 esta muerto la camara se centrara en }
            el jugador 2*/
            ui->graphicsView->centerOn(jugador2);
        }
        else{
            /*Si el jugador 1 se encuentra vivo la pantalla se centrara en el jugador 1*/
            ui->graphicsView->centerOn(jugador);
        }
    }
    else
        //Si solo es un jugador en la partida se centrara en el jugador 1
        ui->graphicsView->centerOn(jugador);
}

void Mapa_GamePlay::verificar_muerte()
{
    /*Esta funcion se encarga de verificar si el jugador se encuentra muerto para asi reiniciar la partida*/
    if (num_jugadores == 2){
        /*En el caso de dos jugadores si ya los dos se encuentran muertos se procedera a mostrarle un QMessageBox
        y se creara un nuevo mapa_gameplay que cargara a los jugadores al principio de la mision.*/
        if (jugador->muerto and jugador2->muerto){
            jugadorMuerto.play();
            QMessageBox msgBox;
            msgBox.setText("Tu alma ha sido destruida.");
            msgBox.setWindowTitle("HellBurn");
            msgBox.setWindowIcon(QIcon(":/Imagenes/ICONO.png"));
            msgBox.setStyleSheet("background-color:#211b18;"
                                 "color:white;");
            msgBox.exec();

            //Se crea un nuevo mapa_gameplay y se cerrara el actual
            Mapa_GamePlay *Mapa = new Mapa_GamePlay;
            Mapa->show();

            close();
            delete this;
        }
    }
    else{
        if (jugador->muerto){
            /*En el caso de un jugador si el jugador muere se mostrara el QMessageBox y se creara un nuevo Mapa_gameplay*/
            jugadorMuerto.play();
            QMessageBox msgBox;
            msgBox.setText("Tu alma ha sido destruida.");
            msgBox.setWindowTitle("HellBurn");
            msgBox.setWindowIcon(QIcon(":/Imagenes/ICONO.png"));
            msgBox.setStyleSheet("background-color:#211b18;"
                                 "color:white;");
            msgBox.exec();

            //Se crea un nuevo mapa_gameplay y se cerrara el actual
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
    //Pararemos todos los timers activos para evitar que se sigan ejecutando procesos en el fondo
    timer.stop();

    jugador->PararTimers();
    if (num_jugadores == 2)
        jugador2->PararTimers();
    QListIterator<Enemigo *>Iterador(listaEnemigos);
    while(Iterador.hasNext()){
        Iterador.next()->PararTimers();
    }
    enemigos.stop();
    //Abriremos un nuevo menu de pausa
    MenuPausa *opciones = new MenuPausa(nullptr);
    opciones->show();
    /*Conectamos el slot reanudarTimers de esta clase con la signal reanudad de menupausa, en caso de que la signal se
    active desde menu pausa entonces el slot de esta clase sera activado*/
    connect(opciones, &MenuPausa::reanudar, this, &Mapa_GamePlay::reanudarTimers);
    /*Conectamos el slot Cerrar_Ventana de esta clase con la signal cerrar_Sesion de menupausa, en caso de que la signal se
    active desde menu pausa entonces el slot de esta clase sera activado*/
    connect(opciones,&MenuPausa::Cerrar_Sesion,this,&Mapa_GamePlay::Cerrar_Ventana);
}

void Mapa_GamePlay::Cerrar_Ventana()
{
    /*Este slot sera activado en caso que el jugador desde menupausa decida que quiere cerrar sesion, entonces la signal
    sera emitida y se activara este slot que cerrara la ventana actual, y eliminara a los jugadores para evitar problemas
    de memoria al abrir un nuevo mapa_gameplay*/
    close();
    delete jugador;
    if (num_jugadores == 2)
        delete jugador2;
    delete this;
}
