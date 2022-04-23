#include "server.h"
#include "ui_server.h"


/**
 * Constructor de la clase server
 *
 * @brief server::server
 * @param parent un objeto propio de QT
 */
server::server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::server)
{


    ui->setupUi(this);
    this->setStyleSheet("background-color: lightblue;");
    ui->plainTextEdit->setReadOnly(true);
    server::create_inMemory(); // opcion 2

    _server = new QTcpServer(this);
    _server->listen(QHostAddress::Any, 4050);
    _socket = new QTcpSocket(this);
    connect(_server, SIGNAL(newConnection()), this, SLOT(conexion_nueva()));
}

/**
 * Realiza una conexión TCP con un cliente y se prepara para estar en constante lectura
 * de sus mensajes
 *
 * @brief server::conexion_nueva
 */
void server::conexion_nueva(){
    _socket = _server->nextPendingConnection();
    connect(_socket, SIGNAL(readyRead()), this, SLOT(leer_socket()));
}

/**
 * Lee los mensajes enviados por el cliente y los remite a un metodo handler
 *
 * @brief server::leer_socket
 */
void server::leer_socket(){

    mensajes_recibidos++;

    QByteArray buffer;
    buffer.resize(_socket->bytesAvailable());
    _socket->read(buffer.data(), buffer.size());

    QString message = QString(buffer).toLatin1().data();
    ui->plainTextEdit->appendPlainText(message);

    handle_mensaje(message, mensajes_recibidos);

}

/**
 * Llama a la matriz paginada para comparar los tipos de dos cartas distintas
 * En caso de ser iguales, envia al cliente un "1" y si no, un "0"
 *
 * @brief server::comparar_cartas
 * @param index1 el indice donde se encuentra la primer carta clickeada
 */
void server::comparar_cartas(size_t index1){

    //verifica si está cargada la carta 2
    size_t ind2 = pagedMatrix->buscar_CartasCargadas(fila2, columna2);

    bool sameType = pagedMatrix->inMemoryCards.at(index1).carta.type
            == pagedMatrix->inMemoryCards.at(ind2).carta.type;

    if(sameType){
        pagedMatrix->onDiscCards.clear();
        pagedMatrix->inMemoryCards.clear();
        pagedMatrix->update_array(fila1, columna1, fila2, columna2);
        manejar_puntajes(true, curr_player, 1); // llevar un contador para saber si pegó dos seguidas el curr player
        enviar_al_cliente("1");

    }else{
        enviar_al_cliente("0");
        manejar_puntajes(true, curr_player, 1);
    }
}


void server::enviar_imagen(){

}

/**
 * Recibe un flag para saber si es la primer o segunda carta clickeada y
 * solicita a matriz paginada buscar si una carta esta cargada en memoria o no.
 * Si esta cargada, devuelve el indice en donde esta almacenada dicha carta
 *
 * @brief server::verificar_cartaInMemory
 * @param secondCard para identificar cual carta necesita
 * @return
 */
size_t server::verificar_cartaInMemory(bool secondCard){

    size_t index;

    if(secondCard){
        index = pagedMatrix->buscar_CartasCargadas(fila2, columna2);
        return index;

    }else{
        index = pagedMatrix->buscar_CartasCargadas(fila1, columna1);
        return index;
    }
}

/**
 * Manda a inicializar la matriz paginada para que en su interior esta genere
 * el disco y la memoria.
 *
 * @brief server::create_inMemory
 */
void server::create_inMemory(){
    pagedMatrix = new PagedMatrix();
    pagedMatrix->llenar_array(); // esto internamente llama a la funcion para escribir en el .bin
    pagedMatrix->leer_arrayArchivo();
    pagedMatrix->llenar_inMemory();
    mostrar_cartasDisco();
}

/**
 * Despliega en consola las cartas contenidas en disco, en memoria, los page
 * hits, page faults y los puntos de los jugadores.
 *
 * @brief server::mostrar_cartasDisco
 */
void server::mostrar_cartasDisco(){
    vector<Card> discCopy = pagedMatrix->onDiscCards;
    vector<Card> mem = pagedMatrix->inMemoryCards;
    int hits = pagedMatrix->pageHits;
    int faults = pagedMatrix->pageFaults;

    qDebug()<<"DISC CARDS----------MEMORY CARDS----------HITS----------FAULTS"<<endl;
    for(size_t i = 0; i<discCopy.size(); i++){

        if(i>=mem.size()){
            qDebug()<<discCopy.at(i).carta.row<<discCopy.at(i).carta.column<<
                      discCopy.at(i).carta.type<<discCopy.at(i).carta.ganada
                       <<"   -   ";

        }else{
            if(i == 0){
                qDebug()<<discCopy.at(i).carta.row<<discCopy.at(i).carta.column<<
                          discCopy.at(i).carta.type<<discCopy.at(i).carta.ganada<<
                          "       "<<mem.at(i).carta.row<<mem.at(i).carta.column<<
                          mem.at(i).carta.type<<mem.at(i).carta.ganada<<"         "<<
                          hits<<"         "<<faults;
            }else{
                qDebug()<<discCopy.at(i).carta.row<<discCopy.at(i).carta.column<<
                          discCopy.at(i).carta.type<<discCopy.at(i).carta.ganada<<
                          "       "<<mem.at(i).carta.row<<mem.at(i).carta.column<<
                          mem.at(i).carta.type<<mem.at(i).carta.ganada;
            }
        }
    }
}


/**
 * Interpreta los indices de las cartas (enviadas por el cliente mediante
 * el socket) y asi saber si le corresponde a la primera o segunda carta clickeada.
 *
 * @brief server::descomponer_indices
 * @param mensaje enviado por el cliente
 * @param whichCard para saber si es la primera o segunda carta
 */
void server::descomponer_indices(QString mensaje, int whichCard){

    int fila = mensaje[0].digitValue();
    int columna = mensaje[1].digitValue();

    if(whichCard == 1){ // Primera carta
        fila1 = fila;
        columna1 = columna;

    }else{ // Segunda carta
        fila2 = fila;
        columna2 = columna;
    }
}

/**
 * Se encarga de llamar metodos necesarios segun el el punto de juego
 *
 * @brief server::handle_mensaje
 * @param mensaje enviado por el cliente
 * @param mensajesRecibidos contador conveniente para definir ruta de accion
 */
void server::handle_mensaje(QString mensaje, int mensajesRecibidos){

    llego_segundaCarta = mensajesRecibidos % 2 == 0 && mensajesRecibidos > 2;

    if(mensajesRecibidos == 1){         // mensaje recibido = nombre player 1
        ui->nameP1->setText(mensaje);

    }else if(mensajesRecibidos == 2){   // mensaje recibido = nombre player 2
        ui->nameP2->setText(mensaje);
        cambiar_turnoJugador(true);

    }else if(llego_segundaCarta){       // mensaje recibido = segunda carta
        descomponer_indices(mensaje, 2);
        enviar_imagen();
        comparar_cartas(this->index1);
        mostrar_cartasDisco();

    }else{                              // mensaje recibido = primera carta
        descomponer_indices(mensaje, 1);
        index1 = verificar_cartaInMemory(false);
        enviar_imagen();
        mostrar_cartasDisco();
        cambiar_turnoJugador(false);
    }

}

/**
 * Suma o resta puntajes dependiendo de si el jugador hizo el match de las cartas
 * y de si esta usando un power Up.
 *
 * @brief server::manejar_puntajes
 * @param matchedCards bool para saber si gano los puntos
 * @param who cual jugador
 * @param whichPowerUp cual powerUp aplicar
 */
void server::manejar_puntajes(bool matchedCards, int who, int whichPowerUp){
    if(matchedCards){
        if(who == 1){
            p1_points++;
        }else{
            p2_points++;
        }
    }else{
        if(whichPowerUp == 1){
            if(who == 1){
                p1_points += 4;
            }else{
                p2_points += 4;
            }
        }else if(whichPowerUp == 2){
            curr_player;
        }
    }
}


void server::on_send_clicked(){
    enviar_al_cliente(ui->lineEdit->text());
    ui->plainTextEdit->appendPlainText(ui->lineEdit->text());
    ui->lineEdit->clear();
}

/**
 * Escribe mensajes y los envia al cliente por el socket creado anteriormente
 *
 * @brief server::enviar_al_cliente
 * @param message este se le enviara al cliente
 */
void server::enviar_al_cliente(QString message){
    _socket->write(message.toLatin1().data(), message.size());

}

/**
 * Actualiza una variable para saber cual jugador debe jugar en este momento
 *
 * @brief server::cambiar_turnoJugador
 * @param firstTime si es true, genera aleatoriamente el turno
 */
void server::cambiar_turnoJugador(bool firstTime){    

    if(firstTime){
        curr_player = 1 + rand()%(2 + 1 - 1);

        if(curr_player == 1){
            enviar_al_cliente("1");
        }else{
            enviar_al_cliente("2");
        }

    }else{

        if(curr_player == 1){
            curr_player = 2;
        }else{
            curr_player = 1;
        }
    }

    qDebug()<<"Current player"<<curr_player<<endl;


}

/**
 * Destructor de la clase server
 * @brief server::~server
 */
server::~server()
{
    delete ui;
}
