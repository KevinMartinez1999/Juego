#include "niveles.h"
#include "ui_niveles.h"
#include "mapa_gameplay.h"
#include "menupausa.h"
#include <QGraphicsRectItem>

JugadorBatalla *jugadorBatalla, *jugadorBatalla2;
Boss *boss;

extern short int num_jugadores;
extern QString user,pass;

Niveles::Niveles(QWidget *parent, short int lvl) :
    QWidget(parent), ui(new Ui::Niveles), nivel(lvl)
{
    ui->setupUi(this);
    //Se inicializa por defecto la variable de tutorial en falso
    tutorial=false;

    ruta = "../Videojuego_Final/Partidas/"; //Modo Debug
    //ruta = "Partidas/"; //Ejecutabl

    //Esconde el cursor
    QCursor cursor = QCursor(Qt::BlankCursor);
    setCursor(cursor); 

    //Musica de los niveles
    //Dependiendo del nivel ingresado se añadira al loop una cancion diferente
    if(nivel==0)Lista.addMedia(QUrl("qrc:/Musica/MUSICA0.mp3"));
    else if(nivel==1)Lista.addMedia(QUrl("qrc:/Musica/MUSICA1.mp3"));
    else if(nivel==2)Lista.addMedia(QUrl("qrc:/Musica/MUSICA3.mp3"));
    else if(nivel==3)Lista.addMedia(QUrl("qrc:/Musica/MUSICA2.mp3"));
    Lista.setPlaybackMode(QMediaPlaylist::Loop);
    musicaNivel.setPlaylist(&Lista);
    musicaNivel.setVolume(80);
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
    connect(&timer, SIGNAL(timeout()), this, SLOT(verificar_muerte()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(muerte()));
    timer.start(100);

    //Sonidos
    JugadorMuerto.setMedia(QUrl("qrc:/Musica/MUERTO.mp3"));
    JugadorMuerto.setVolume(100);

    //Sonido de victoria
    victoria.setMedia(QUrl("qrc:/Musica/GANAR.wav"));
    victoria.setVolume(100);

    if(tutorial)
        //En el caso de que estemos en el tutorial los bosses no atacaran
        boss->PararTimers();
}

Niveles::~Niveles()
{
    delete ui;
}

void Niveles::NivelSetup()
{
    /*Dependiendo del nivel con el que se inicie el constructor de la clase, se crearan 4 niveles totalmente
    diferentes.
    Se escogera la imagen de fondo del nivel, las posiciones iniciales de los dos jugadores y la imagen de
    tutorial en caso de que el jugador se encuentre en el tutorial del juego.
    */
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
        tutorial=true;
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
    boss->vida.setZValue(1);
    escena->addItem(&boss->vida);//Se añade a la escena la barra de vida del Boss

    //Se añade a la escena al jugador con las posiciones previamente seleccionadas
    jugadorBatalla->setPos(jugadorBatalla->GetX0(),jugadorBatalla->GetY0());
    escena->addItem(jugadorBatalla);
    jugadorBatalla->box.setPos(jugadorBatalla->x()-25, jugadorBatalla->y()-50);
    jugadorBatalla->vida.setPos(90,28);
    jugadorBatalla->vida.setZValue(1);
    escena->addItem(&jugadorBatalla->vida);//Se añade la barra de vida del jugador a la escena
    if(num_jugadores==2){
        //En el caso de que haya dos jugadores se añade al jugador2 con las posiciones previamente seleccionadas
        jugadorBatalla2->setPos(jugadorBatalla2->GetX0(),jugadorBatalla2->GetY0());
        escena->addItem(jugadorBatalla2);
        jugadorBatalla2->box.setPos(jugadorBatalla2->x()-25, jugadorBatalla2->y()-50);
        jugadorBatalla2->vida.setPos(146,96);
        jugadorBatalla2->vida.setZValue(1);
        escena->addItem(&jugadorBatalla2->vida);//Se añade la barra de vida del jugador a la escena
        /*En el caso de que hayan dos jugadores al label interfaz se le añadira una imagen de interfaz que tiene
        un diseño para las barras de vida de los dos jugadores*/
        ui->Interfaz->setPixmap(QPixmap(":/Imagenes/INTERFAZBOSS2.png"));
    }
    //Si es solo un jugador se añadira el diseño con solo una barra de vida
    else ui->Interfaz->setPixmap(QPixmap(":/Imagenes/INTERFAZBOSS1.png"));
}
void Niveles::muerte()
{
    /*Esta funcion se encarga de verificar si los jugadores se encuentran muertos en la partida, esto con el
    objetivo de ocultarlos y tambien sus atributos y acabar la partida.*/
    if (num_jugadores == 2){
        /*En el caso de dos jugadores se analizara a los dos jugadores por separado*/
        if (jugadorBatalla->health <= 1){
            //jugador1
            jugadorBatalla->muerto=true;
            jugadorBatalla->hide();//Se esconde al jugador junto con su barra de vida
            jugadorBatalla->vida.hide();//Se esconde al jugador junto con su barra de vida
        }
        if (jugadorBatalla2->health <= 1){
            //jugador2
            jugadorBatalla2->muerto=true;
            JugadorMuerto.play();
            jugadorBatalla2->hide();//Se esconde al jugador junto con su barra de vida
            jugadorBatalla2->vida.hide();//Se esconde al jugador junto con su barra de vida
        }
       }
    else{
        if (jugadorBatalla->health <= 1){
            //En el caso de que solo haya un solo jugador en la partida solamente se analizara al jugador 1
            jugadorBatalla->muerto=true;
            jugadorBatalla->hide();//Se esconde al jugador junto con su barra de vida
            jugadorBatalla->vida.hide();//Se esconde al jugador junto con su barra de vida
        }
    }
}

void Niveles::verificar_muerte()
{
    /*Esta funcion nos permite generar el mensaje de fin de partida junto con los sonidos y la apertura de
    un nuevo mapa para que el jugador decida si quiere o no repetir el nivel.*/
    if (num_jugadores == 2){
        if (jugadorBatalla->muerto and jugadorBatalla2->muerto){
            //En el caso de dos jugadore si ya los dos se encuentran muertos se acabara la partida.
            JugadorMuerto.play();
            boss->PararTimers();
            //Se muestra el QMessageBox
            QMessageBox msgBox;
            msgBox.setText("Tu alma ha sido destruida.");
            msgBox.setWindowTitle("HellBurn");
            msgBox.setWindowIcon(QIcon(":/Imagenes/ICONO.png"));
            msgBox.setStyleSheet("background-color:#211b18;"
                                 "color:white;");
            msgBox.exec();

            //Se crea un nuevo mapagameplay para devolver al jugador
            Mapa_GamePlay *mapa=new Mapa_GamePlay(nullptr, 0);
            mapa->show();
            close();
            delete this;
        }
    }
    else{
        if (jugadorBatalla->muerto){
            //En el caso de un solo jugador cuando el jugador muere se hara lo mismo
            boss->PararTimers();
            JugadorMuerto.play();
            //Se muestra el QMessageBox
            QMessageBox msgBox;
            msgBox.setText("Tu alma ha sido destruida.");
            msgBox.setWindowTitle("HellBurn");
            msgBox.setWindowIcon(QIcon(":/Imagenes/ICONO.png"));
            msgBox.setStyleSheet("background-color:#211b18;"
                                 "color:white;");
            msgBox.exec();

            //Se crea un nuevo mapagameplay para devolver al jugador
            Mapa_GamePlay *mapa=new Mapa_GamePlay(nullptr, 0);
            mapa->show();
            close();
            delete this;
    }
    }
}

void Niveles::Tutorial()
{
    /*Esta funcion basicamente lo que permite es que al momento del tutorial cada que presionemos la tecla espacio
    se cambie la imagen de tutorial que se este mostrando actualmente. Cuando finalmente se haya presionado el boton
    3 veces se ocultara el label de las imagenes y los jugadores ya se podran mover.*/
    cont++;
    if(cont==1)
        ui->Controles->setPixmap(QPixmap(":/Imagenes/TUTORIALBOSS1.png"));
    else if(cont==2)
        ui->Controles->setPixmap(QPixmap(":/Imagenes/TECLAS.png"));
    else if(cont==3){
        //En la tercera vez se ocultara el label y la batalla comenzara.
        ui->Controles->hide();
        tutorial=false;
        boss->ReiniciarTimers();
    }
}
/*Las siguientes son las funciones del teclado; existe tanto las teclas para el jugador
1 como para el jugador 2, pero en el caso de jugador 2, no puede haber movimiento a menos
que pj2 = true y esto pasa solo si asi lo escoge el usuario en la pantalla anterior del menú,
si no se escoge asi las teclas no van a tener ningun efecto cuando se este jugando solo*/

void Niveles::keyPressEvent(QKeyEvent *event)
{
    if(!tutorial){
        if (event->key() == Qt::Key_A){
            if (!jugadorBatalla->muerto)//Verificacion que el jugador no esta muerto
                jugadorBatalla->setBanLeft();
        }
        else if (event->key() == Qt::Key_D){
            if (!jugadorBatalla->muerto)//Verificacion que el jugador no esta muerto
                jugadorBatalla->setBanRight();
        }
        else if (event->key() == Qt::Key_W){
            if (!jugadorBatalla->muerto)//Verificacion que el jugador no esta muerto
                jugadorBatalla->setBanJump();
        }
        else if (event->key() == Qt::Key_F){
            if(!jugadorBatalla->muerto)//Verificacion que el jugador no esta muerto
                jugadorBatalla->setBanAttack();
        }
        else if (event->key() == Qt::Key_C){
            if (!jugadorBatalla->muerto)//Verificacion que el jugador no esta muerto
                jugadorBatalla->setBanSpell();
        }
        /*Estas son las teclas de movimiento para el jugador 2. Solo estan habilitadas si asi
          lo quiere el usuario.*/
        else if (event->key() == Qt::Key_J){
            if (num_jugadores == 2)
                if (!jugadorBatalla2->muerto)//Verificacion que el jugador no esta muerto
                    jugadorBatalla2->setBanLeft();
        }
        else if (event->key() == Qt::Key_L){
            if (num_jugadores == 2)
                if (!jugadorBatalla2->muerto)//Verificacion que el jugador no esta muerto
                    jugadorBatalla2->setBanRight();
        }
        else if (event->key() == Qt::Key_I){
            if (num_jugadores == 2)
                if (!jugadorBatalla2->muerto)//Verificacion que el jugador no esta muerto
                    jugadorBatalla2->setBanJump();
        }
        else if (event->key() == Qt::Key_H){
            if(!jugadorBatalla2->muerto)//Verificacion que el jugador no esta muerto
                jugadorBatalla2->setBanAttack();
        }
        else if (event->key() == Qt::Key_N){
            if (num_jugadores == 2)
                if (!jugadorBatalla2->muerto)//Verificacion que el jugador no esta muerto
                    jugadorBatalla2->setBanSpell();
        }
        //Tecla escape destinada para pausar el juego y ver las opciones
        else if(event->key() == Qt::Key_Escape){
            emit on_Opciones_clicked();//Si presionamos Escape se activara la funcion del boton al ser clickeado
        }
    }
    else if(tutorial){
        //Si estamos en el tutorial al momento de presionar espacio cambiaremos las imagenes
        if(event->key() == Qt::Key_Space){
            Tutorial();
        }
    }
}

void Niveles::keyReleaseEvent(QKeyEvent *event)
{
    //Segun la tecla que se deja de presionar una bandera se baja y el movimiento ahi se detiene
    if(!tutorial){
        /*Estas son las teclas de movimiento para el jugador 2. Solo estan habilitadas si asi
          lo quiere el usuario.*/
        if (event->key() == Qt::Key_A){
            if (!jugadorBatalla->muerto)
                jugadorBatalla->resetBanLeft();
        }
        else if (event->key() == Qt::Key_D){
            if (!jugadorBatalla->muerto)
                jugadorBatalla->resetBanRight();
        }
        else if (event->key() == Qt::Key_F){
            if(!jugadorBatalla->muerto)
                jugadorBatalla->resetBanAttack();
        }
        else if (event->key() == Qt::Key_C){
            if (!jugadorBatalla->muerto)
                jugadorBatalla->resetBanSpell();
        }
        /*Estas son las teclas de movimiento para el jugador 2. Solo estan habilitadas si asi
          lo quiere el usuario.*/
        else if (event->key() == Qt::Key_J){
            if (num_jugadores == 2)
                if (!jugadorBatalla2->muerto)
                    jugadorBatalla2->resetBanLeft();
        }
        else if (event->key() == Qt::Key_L){
            if (num_jugadores == 2)
                if (!jugadorBatalla2->muerto)
                    jugadorBatalla2->resetBanRight();
        }
        else if (event->key() == Qt::Key_N){
            if (num_jugadores == 2)
                if (!jugadorBatalla2->muerto)
                    jugadorBatalla2->resetBanSpell();
        }
        else if (event->key() == Qt::Key_H){
            if(!jugadorBatalla2->muerto)
                jugadorBatalla2->resetBanAttack();
        }
    }
}

void Niveles::changeEvent(QEvent *event)
{
    /*Esta funcion nos permite tener control de si el jugador por alguna razon minimiza la ventana, al minimizarla se
        activara el menu de pausa para evitar algun error.*/
    if(event->type() == QEvent::WindowStateChange){
        if(windowState() == Qt::WindowMinimized){
            //Si la ventana se encuentra minimizada se pausara el juego.
            emit on_Opciones_clicked();
        }
    }
}

void Niveles::Level_Events()
{
    /*Esta funcion se encarga de verificar si el boss ya ha sido derrotado.
    Al momento de que el boss tenga su vida menor que 0 se activara un bool que indicara a la clase nivel que el
    usuario ha ganado el nivel y ahora puede volver al mapa principal, se abrira una nueva ventana mapa_gameplay
    y se eliminara la ventana del nivel.*/
    if(boss->Boss_Derrotado){
        victoria.play();
        musicaNivel.stop();

        //Esta señal aumenta el nivelactual con el objetivo de cargar la partida
        emit aumentar();

        //Se para el movimiento de los jugadores
        jugadorBatalla->PararTimers();
        if(num_jugadores==2)
            jugadorBatalla2->PararTimers();

        /*Despues de que el boss ha sido derrotado se procede a guardar en el archivo el progreso conseguido
        se escribira en el archivo los datos y se escribira que el nivelactual ha sido derrotado al escribir
        nivel+1 eso signifca que hemos derrotado a un nuevo Boss, y posteriormente se añadira un 0 para
        que al entrar al mapa ahora el jugador tenga que derrotar a los enemigos.*/
        fstream file(ruta+user.toUtf8()+".txt");
        if (!file.is_open())
            return;
        //Se guardan los datos.
        file<<user.toStdString()<<"\n"<<pass.toStdString();
        file<<'\n'<<num_jugadores<<'\n'<<nivel+1<<'\n'<<0;
        file.flush();
        file.close();

        boss->Boss_Derrotado=false;
        if(nivel!=0){
            /*En el caso de que los bosses hayan sido derrotados se paran sus timers, el boss del tutorial
            tiene una animacion de muerte especial por eso no esta incluido en este if por la linea nivel!=0.*/
            boss->PararTimers();
        }

        //Se muestra el QMessageBox que nos dice que ganamos el nivel
        QMessageBox msgBox;
        msgBox.setText("Has sobrevivido a la batalla!");
        msgBox.setWindowTitle("HellBurn");
        msgBox.setWindowIcon(QIcon(":/Imagenes/ICONO.png"));
        msgBox.setStyleSheet("background-color:#211b18;"
                             "color:white;");
        msgBox.exec();

        //Se creara un nuevo mapa_gameplay para continuar con el desarrollo del juego
        Mapa_GamePlay *mapa=new Mapa_GamePlay;
        mapa->show();
        close();
        delete this;
    }
    //Si no hemos derrotado al boss simplemente se devolvera.
    else return;
}

void Niveles::on_Opciones_clicked()
{
    /*En caso de oprimir la tecla Escape o presionar le boton de opciones en la esquina de la pantalla
    se abrira una nueva ventada opciones por encima de la ventana actual.
    Esta ventana de opciones tiene una cualidad llamada modal que permite que no se pueda interactuar de cualquier
    forma con la ventana de atras si el ui de pausa aun esta abierto, para poder volver a interactuar con el inter
    faz de nivel primero se debe cerrar la ventana de pausa.*/
    //Se detienen todos los timers de los jugadores y de los bosses
    jugadorBatalla->PararTimers();
    if (num_jugadores == 2)
        jugadorBatalla2->PararTimers();
    boss->PararTimers();

    //Se abre el menu de pausado
    MenuPausa *opciones = new MenuPausa(nullptr);
    opciones->show();
    /*Conectamos el slot Cerrar_Ventana de esta clase con la signal cerrar_Sesion de menupausa, en caso de que la signal se
    active desde menu pausa entonces el slot de esta clase sera activado*/
    connect(opciones,&MenuPausa::Cerrar_Sesion,this,&Niveles::Cerrar_Ventana);
    /*Conectamos el slot reanudarTimers de esta clase con la signal reanudad de menupausa, en caso de que la signal se
    active desde menu pausa entonces el slot de esta clase sera activado*/
    connect(opciones,&MenuPausa::reanudar,this,&Niveles::reanudarTimers);
}

void Niveles::Cerrar_Ventana()
{
    /*Si desde menupausa el jugador decidio que quiere cerrar sesion se debe emitir una signal conectada a este
    slot */
    close();
    delete this;
}

void Niveles::reanudarTimers()
{
    /*Este slot estara conectado al menu de pausa, al momento de emitir la señal en el boton de reanudar
    se activara este slot que se encargara de reiniciar todos los timers del jugador y el boss para poder
    continuar con la partida.*/
    jugadorBatalla->ReiniciarTimers();
    if (num_jugadores == 2)
        jugadorBatalla2->ReiniciarTimers();
    boss->ReiniciarTimers();
}
