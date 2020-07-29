#include "widget.h"
#include "ui_widget.h"
#include "registrarse.h"
#include "menu_partida.h"

QString user, pass;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //Sistema de autocompletado de usuarios
    QString letras = "";
    QStringList lista;
    QCompleter *cmpt;
    QDir directorio;
    directorio.setPath("../Videojuego_Final/Partidas");
    int len = directorio.count();
    for (int i = 2; i < len; i++){
        for (int j = 0; j < directorio[i].length(); j++){
            if (directorio[i][j] == '.'){
                lista.append(letras);
                letras = "";
                break;
            }
            letras += directorio[i][j];
        }
    }
    cmpt = new QCompleter(lista, this);
    ui->usuario->setCompleter(cmpt);


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
    movie->setFileName(":/Imagenes/GIF1.gif");
    movie->setScaledSize(QSize(1000,650));//Tamaño de la ventana.
    w->setMovie(movie);//Asignamos el gif al Label.
    movie->start();//Iniciamos la reproducción del gif.

    /*Ya que estamos creando el gif por código, siempre aparecerá en la ventana por encima de todos los elementos agregados en el .ui,
    para arreglar esto haremos uso de la función .raise(); que traerá al frente los elementos de la interfaz grafica
    (Labels, botones, lineEdits, etc). Primero traeremos al frente el label Interfaz que contiene la imagen diseñada para el menú,
    y luego se traerá al frente los demás elementos.*/
    ui->Interfaz->raise();
    ui->usuario->raise();
    ui->clave->raise();
    ui->mostrar->raise();
    ui->label->raise();
    ui->label_2->raise();
    ui->login->raise();
    ui->salir->raise();
    ui->registrarse->raise();

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
    rx = QRegExp("^[\\w'\\-,.][^_!¡' '?÷?¿/\\+=@#$%ˆ&*(){}|~<>;:[\\]]{2,}$"); //Expresion
    val = new QRegExpValidator(rx, this); //Validador
    ui->usuario->setValidator(val);

    /*Se inicializa la casilla de contraseña en forma de Password para que sea imposible saber
     cual es; hay una opcion en la misma casilla de ver contraseña*/
    ui->clave->setEchoMode(QLineEdit::Password);

    /*Se cambia la letra porque la fuente usada es grande y los circulos de la contraseña
    se ven mucho mas grandes*/
    ui->clave->setFont(QFont("Arial", 10, 1));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_login_clicked()
{
    boton.play();

    //Obtenemos el usuario y contraseña que digitó el usuario
    user = ui->usuario->text();
    pass = ui->clave->text();

    /*Aqui se busca en la base de datos el archivo que contiene el usuario y
     cntraseña que el usuario digtó; Si dichi usuario no existe se va a saltar un
     error*/
    ifstream file("../Videojuego_Final/Partidas/"+user.toUtf8()+".txt");
    if (!file.is_open())
    {
        QMessageBox msgBox;
        msgBox.setText("Contraseña o usuario incorrecto.");
        msgBox.setWindowTitle("HellBurn");
        msgBox.setWindowIcon(QIcon(":/Imagenes/ICONO.png"));
        msgBox.setStyleSheet("background-color:#211b18;"
                             "color:white;");
        msgBox.exec();

        //Se borran las casillas de usuario y contraseña para una nueva
        ui->usuario->clear();
        ui->clave->clear();
        return;
    }

    /*Aqui en caso de que el archivo existe en la base de datos se pasa a verificar el contenido
     de este con lo que el usuario entró*/
    string usuario, clave;
    file>>usuario;
    file>>clave;
    file.close();
    if (pass.toStdString() == clave) //Verificación
    {
        delete movie;
        delete w;
        delete val;
        //Se abre la nueva mentana del menú de partida una vez el inicio fue exitóso
        Menu_partida *menu = new Menu_partida;
        menu->show();
        close();
    }
    else
    {
        //Muestra mensaje de error en caso de un login fallido
        QMessageBox msgBox;
        msgBox.setText("Contraseña o usuario incorrecto.");
        msgBox.setWindowTitle("HellBurn");
        msgBox.setWindowIcon(QIcon(":/Imagenes/ICONO.png"));
        msgBox.setStyleSheet("background-color:#211b18;"
                             "color:white;");
        msgBox.exec();
        ui->usuario->clear();
        ui->clave->clear();
    }
}

void Widget::on_registrarse_clicked()
{
    boton.play();

    /*Si deseamos registrarnos en el sistema, al presionar el botón se procederá a cerrar la ventana actual y se  creara una
    nueva ventana de registro y se abrirá.*/
    delete movie;
    delete w;
    delete val;

    Registrarse *registro = new Registrarse;
    registro->show();
    close();
}

void Widget::on_mostrar_stateChanged(int arg1)
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
