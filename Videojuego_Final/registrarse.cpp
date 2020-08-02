#include "registrarse.h"
#include "ui_registrarse.h"

Registrarse::Registrarse(Widget *parent) :
    Widget(parent),
    ui(new Ui::Registrarse)
{
    ui->setupUi(this);

    //Sonido al presionar los botones
    boton.setMedia(QUrl("qrc:/Musica/ESPADA.mp3"));
    boton.setVolume(100);

    /*Sistema de reproducción de gif en el menú:
    Para reproducir un gif primeramente se creara un nuevo QLabel al cual le asignaremos las dimensiones de la ventana, posterior a eso
    crearemos una variable QMovie con el gif a reproducir y también le asignaremos el tamaño de la pantalla, luego con
    la función setMovie le asignaremos al Label que contenga el gif y se reproduzca.*/
    w = new QLabel(this);
    w->resize(1000,650);//Tamaño de la ventana.
    movie = new QMovie(this);
    movie->setFileName(":/Imagenes/GIF.gif");
    movie->setScaledSize(QSize(1000,650));//Tamaño de la ventana.
    w->setMovie(movie);//Asignamos el gif al Label.
    movie->start();//Iniciamos la reproducción del gif.

    /*Ya que estamos creando el gif por código, siempre aparecerá en la ventana por encima de todos los elementos agregados en el .ui,
    para arreglar esto haremos uso de la función .raise(); que traerá al frente los elementos de la interfaz grafica
    (Labels, botones, lineEdits, etc). Primero traeremos al frente el label Interfaz que contiene la imagen diseñada para el menú,
    y luego se traerá al frente los demás elementos.*/
    ui->Interfaz->raise();
    ui->clave->raise();
    ui->usuario->raise();
    ui->label->raise();
    ui->label_2->raise();
    ui->registro->raise();
    ui->mirar->raise();
    ui->volver->raise();

    /*Creacion del cursor del videojuego: con ayuda de QCursor podremos brindarle al cursor la imagen que deseamos.
    Primeramente crearemos un pixmap que contiene la imagen, luego se creara una variable QCursor que recibira la imagen
    y los puntos de eje del click, luego con setCursor establecemos ese cursor para toda la ventana.*/
    Pixmap_Cursor = QPixmap(":/Imagenes/CURSOR.png");
    cursor = QCursor(Pixmap_Cursor,0,0);
    setCursor(cursor);

    //Enfoca por defecto la casilla del usuario para hacer el login mas rapido
    ui->usuario->setFocus();

    /*Aqui se valida la informacion que el usuario entra en la casilla de nombre de usuario;
    El QRegExp o registro de expresion es una cadena de caracteres que contiene los caracteres
    que no debeen ser usados en un nombre de usuario o cualquier otra cosa commo una contraseña,
    e-mail, numero de telefono, etcétera. Luego de tener esta expresion el QRegExpValidator la toma
    y no deja que en esa casilla de nombre de usuario aparezcan esos caracteres aunque el usuario
    los presione*/
    rx = QRegExp("^[\\w'\\-,.][^_!¡' '?÷?¿/\\+=@#$%ˆ&*(){}|~<>;:[\\]]{2,}$");
    val = new QRegExpValidator(rx, this);
    ui->usuario->setValidator(val);
    ui->clave->setValidator(val);

    /*Se inicializa la casilla de contraseña en forma de Password para que sea imposible saber
     cual es; hay una opcion en la misma casilla de ver contraseña*/
    ui->clave->setEchoMode(QLineEdit::Password);

    /*Se cambia la letra porque la fuente usada es grande y los circulos de la contraseña
    se ven mucho mas grandes*/
    ui->clave->setFont(QFont("Arial Black", 10, 1));
}

Registrarse::~Registrarse()
{
    delete ui;
}

void Registrarse::on_registro_clicked()
{
    boton.play(); //Sonido del botón

    /*En esta función el usuario llena unas casillas con sus datos para registrarlo
     y paso seguido se toman esos datos y se llevan a la base de datos de los jugadores
     registrados*/
    QString user, pass;

    //Se toman los datos
    user = ui->usuario->text();
    pass = ui->clave->text();
    if(user==NULL || pass==NULL){
        QMessageBox msgBox;
        msgBox.setText("Registro Invalido.");
        msgBox.setWindowTitle("HellBurn");
        msgBox.setWindowIcon(QIcon(":/Imagenes/ICONO.png"));
        msgBox.setStyleSheet("background-color:#211b18;"
                             "color:white;");
        msgBox.exec();
        ui->usuario->clear();
        ui->clave->clear();
        return;
    } 
    ifstream Verificacion("../Videojuego_Final/Partidas/"+user.toUtf8()+".txt");
    if(Verificacion.is_open()){
        QMessageBox msgBox;
        msgBox.setText("Este usuario ya existe.");
        msgBox.setWindowTitle("HellBurn");
        msgBox.setWindowIcon(QIcon(":/Imagenes/ICONO.png"));
        msgBox.setStyleSheet("background-color:#211b18;"
                             "color:white;");
        msgBox.exec();

        ui->usuario->clear();
        ui->clave->clear();
        return;
    }

    //Se abre y se llevan los datos ingresados al archivo
    ofstream file("../Videojuego_Final/Partidas/"+user.toUtf8()+".txt");
    //Se guardan el nombre de usuario y la contraseña
    file<<user.toStdString()<<"\n"<<pass.toStdString();
    file.flush();
    file.close();
    //Ventana emergente que notifica que el registro fue exitoso
    QMessageBox msgBox;
    msgBox.setText("Registrado correctamente.");
    msgBox.setWindowTitle("HellBurn");
    msgBox.setWindowIcon(QIcon(":/Imagenes/ICONO.png"));
    msgBox.setStyleSheet("background-color:#211b18;"
                         "color:white;");
    msgBox.exec();
    //Se abre un nuevo widget para poder iniciar sesion.
    Widget *w = new Widget;
    w->show();
    close();
}

void Registrarse::on_volver_clicked()
{
    boton.play(); //Sonido del botón

    delete movie;
    delete w;
    delete val;
    /*Si deseamos registrarnos en el sistema, al presionar el botón se procederá a cerrar la ventana actual y se  creara una
    nueva ventana de registro y se abrirá.*/
    Widget *w = new Widget;
    w->show();
    close();
}

void Registrarse::on_mirar_stateChanged(int arg1)
{
    /*Cuando la casilla esta marcada la contraseña se va mostrar y cuando
     la casilla está desmarcada la contraseña va a estar oculta, la fuente se
     cabia por las mismas razones qe se explicó en el constructor de la clase*/
    if (arg1){
        ui->clave->setEchoMode(QLineEdit::Normal);
    }
    else{
        ui->clave->setEchoMode(QLineEdit::Password);
    }
}
