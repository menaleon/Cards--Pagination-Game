#include "client.h"

const char filaIndex[] = "fila";
const char columnaIndex[] = "columna";
const char ganada[] = "CartaGanada";

/**
 * Constructor de la clase cliente
 * @brief client::client
 * @param parent objeto de QT
 */
client::client(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::client)
{
    crear_interfaz_inicial();
    auto client = this;
    Ui::client* interfaz = this->ui;
    responseFromServer_Handler = new handler(interfaz);


}

/**
 * Muestra una ventana con campos de texto para ingresar nombres de jugadores
 *
 * @brief client::crear_interfaz_inicial
 */
void client::crear_interfaz_inicial(){
    ui->setupUi(this);
    this->setStyleSheet("background-color: lightgreen;");
    ui->plainTextEdit->setReadOnly(true);
    ui->player2Label->hide();
    ui->nameP1->hide();
    ui->nameP2->hide();
    ui->playingLabel->hide();
    ui->turnoLabel->hide();
    ui->comparacionLabel->hide();
    ui->juegoFInalizado->hide();

    QTcpSocket* _socket = crear_socket();
    connect(_socket, SIGNAL(readyRead()), this, SLOT(leer_mensaje()));

}

/**
 * Crea el canal de comunicacion para enviar y recibir mensajes del server
 * Si no logra realizar la conexion avisa con un mensaje en la GUI
 *
 * @brief client::crear_socket
 * @return
 */
QTcpSocket* client::crear_socket(){
    _socket = new QTcpSocket(this);
    _socket->connectToHost("localhost", 4050);

    if(_socket->waitForConnected(4050)){
        ui->plainTextEdit->appendPlainText("You and server are now connected :D");
    }else{
        ui->plainTextEdit->appendPlainText("Could not connect to server :( "
                                           "Exit and try again");
        ui->enviarNombrePlayer->hide();
        ui->enviarNombrePlayer->deleteLater();
        ui->lineEdit->hide();
        ui->lineEdit->deleteLater();
        ui->playerLabel->hide();
        ui->playerLabel->deleteLater();
    }
    return _socket;
}

/**
 * Arregla los widgets correspondientes para ver la matriz de botones
 * y el turno actual del jugador
 *
 * @brief client::crear_interfaz_juego
 */
void client::crear_interfaz_juego(){
    ui->plainTextEdit->hide();
    ui->plainTextEdit->deleteLater();
    ui->playerLabel->show();
    ui->player2Label->show();
    ui->nameP1->show();
    ui->nameP2->show();
    ui->playingLabel->show();
    ui->turnoLabel->show();

    rellenar_matrizBotones();
}

/**
 * Define en cada posicion de la matriz un boton con ciertas caracteristicas iniciales
 * para poder visualizarlo en la interfaz del juego
 *
 * @brief client::rellenar_matrizBotones
 */
void client::rellenar_matrizBotones(){

    for(int columnas = 0; columnas<6; columnas++){
        for(int filas = 0; filas<6; filas++){

            QString indices = QString{"(%1,%2)"}.arg(filas).arg(columnas);

            matrizBotones[columnas][filas] = new QPushButton("ButtonText", this);
            matrizBotones[columnas][filas]->setGeometry(QRect(QPoint((columnas+1)*50 +
                                                                     (columnas+1)*15,
                                                                     (filas+1)*50 +
                                                                     (filas+1)*10),
                                                                      QSize(50,50)));

            matrizBotones[columnas][filas]->setProperty(filaIndex, filas);
            matrizBotones[columnas][filas]->setProperty(columnaIndex, columnas);
            matrizBotones[columnas][filas]->setProperty(ganada, false);

            matrizBotones[columnas][filas]->setText(indices);
            matrizBotones[columnas][filas]->setVisible(true);

            connect(matrizBotones[columnas][filas], SIGNAL(clicked()), this,
                                    SLOT(obtenerIndices_cartaPresionada()));
        }
    }


}

/**
 * Si se detecta que llego un mensaje entrante, lo envia a un objeto de tipo Handler
 * para que este interprete lo que significa el mensaje recibido y asi desarrollar los
 * distintos puntos del juego
 *
 * @brief client::leer_mensaje
 */
void client::leer_mensaje(){

    QByteArray buffer;
    buffer.resize(_socket->bytesAvailable());
    _socket->read(buffer.data(), buffer.size());
    QString mensaje = QString(buffer);

    int caso = responseFromServer_Handler->
            handle_message(mensaje, counter_cartasPresionadas);

    qDebug()<<"Caso"<<caso<<endl;

    enable_botonEnMatriz(caso);

}

/**
 * Mediante el canal de comunicacion TCP creado anteriormente, envia al server
 * algun mensaje definido en el parametro.
 *
 * @brief client::enviar_al_server
 * @param message este se enviara al server
 */
void client::enviar_al_server(QString message){
    _socket->write(message.toLatin1().data(), message.size());
}

/**
 * Obtiene la posicion del boton en la matriz de botones definida anteriormente
 * y esta informacion se la envia al server
 *
 * @brief client::obtenerIndices_cartaPresionada
 */
void client::obtenerIndices_cartaPresionada(){
    ui->comparacionLabel->hide();
    counter_cartasPresionadas +=1;

    QString fila = sender()->property(filaIndex).toString();
    QString columna = sender()->property(columnaIndex).toString();
    int filaInt = fila.toInt();
    int columnaInt = columna.toInt();

    matrizBotones[columnaInt][filaInt]->setEnabled(false);
    set_botonEnHandler(columnaInt, filaInt);

    QString mensaje;
    mensaje.append(fila);
    mensaje.append(columna);
    enviar_al_server(mensaje);
}

/**
 * Establece las variables fila y columna en el handler para que
 * este sepa con cual carta esta trabajando
 *
 * @brief client::set_botonEnHandler
 * @param columna del boton en la matriz
 * @param fila del boton en la matriz
 */
void client::set_botonEnHandler(int columna, int fila){

    if(counter_cartasPresionadas %2 == 0){
        responseFromServer_Handler->setColumna2(columna);
        responseFromServer_Handler->setFila2(fila);
    }else{
        responseFromServer_Handler->setColumna1(columna);
        responseFromServer_Handler->setFila1(fila);
    }
}

/**
 * Habilita los botones clickeados en interfaz dependiendo de si el jugador
 * no realizo el match (perdio en ese turno)
 *
 * @brief client::enable_botonEnMatriz
 * @param caso para saber que hacer dependiendo de la situacion
 */
void client::enable_botonEnMatriz(int caso){
    int f1 = responseFromServer_Handler->getFila1();
    int c1 = responseFromServer_Handler->getColumna1();
    int f2 = responseFromServer_Handler->getFila2();
    int c2 = responseFromServer_Handler->getColumna2();

    switch (caso) {
    case 3: // CARTAS IGUALES: establece botones como "ganados"
        matrizBotones[c1][f1]->property(ganada) = true;
        matrizBotones[c2][f2]->property(ganada) = true;
        cartasGanadas += 2;
        check_estadoJuego();
        set_turnoActual(2);
        break;

    case 4: // CARTAS DISTINTAS: habilita los botones nuevamente
        matrizBotones[c1][f1]->setEnabled(true);
        matrizBotones[c2][f2]->setEnabled(true);
        set_turnoActual(2);
        break;

    case 1:
        set_turnoActual(1);
        break;
    case 2:
        set_turnoActual(2);
        break;

    default:
        break;
    }
}

/**
 * Verifica mediante un contador si ya todas las cartas tienen match
 * y si todas ya tienen match entonces muestra un mensaje para indicar
 * que el juego ha concluido
 *
 * @brief client::check_estadoJuego
 */
void client::check_estadoJuego(){
    if(cartasGanadas == 36){
        ui->juegoFInalizado->show();
    }
}

/**
 * Cambia en la interfaz el turno del jugador
 *
 * @brief client::set_turnoActual
 * @param caso variable para determinar la ruta de accion
 */
void client::set_turnoActual(int caso){

    if(counter_cartasPresionadas == 0){
        if(caso == 1){
            player = 1;
            ui->turnoLabel->setText("1");
        }else{
            player = 2;
            ui->turnoLabel->setText("2");
        }

    }else{

        if(ui->turnoLabel->text().toInt() == 1){
            player = 2;
            ui->turnoLabel->setText("2");
        }else{
            player = 1;
            ui->turnoLabel->setText("1");

        }
    }

}

/**
 * Toma el texto del lineEdit en la interfaz y lo envia
 * al server mediante el socket creado anteriormente
 *
 * @brief client::on_enviarNombrePlayer_clicked
 */
void client::on_enviarNombrePlayer_clicked()
{
    if(counter == 1){
        ui->enviarNombrePlayer->hide();
        ui->enviarNombrePlayer->deleteLater();
        ui->playerLabel->hide();
        ui->playerLabel->move(50, 430);
        ui->lineEdit->hide();
        ui->lineEdit->deleteLater();
        crear_interfaz_juego();
    }

    ui->playerLabel->setText("Player 2: ");
    QString nombre_player = ui->lineEdit->text();
    enviar_al_server(nombre_player);
    ui->lineEdit->clear();
    counter +=1;
}

/**
 * Destructor de la clase cliente
 * @brief client::~client
 */
client::~client()
{
    delete ui;
}
