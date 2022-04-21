#include "server.h"
#include "ui_server.h"
#include <string>
#include<string.h>
#include<iostream>
#include <QDebug>

server::server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: lightblue;");
    ui->plainTextEdit->setReadOnly(true);


    //pagedMatrix->llamar_matrizDisco(); opcion 1
    server::fill_inMemory(); // opcion 2

    _server = new QTcpServer(this);
    _server->listen(QHostAddress::Any, 4050);
    _socket = new QTcpSocket(this);
    connect(_server, SIGNAL(newConnection()), this, SLOT(conexion_nueva()));
}

void server::conexion_nueva(){
    _socket = _server->nextPendingConnection();
    connect(_socket, SIGNAL(readyRead()), this, SLOT(leer_socket()));
}

void server::leer_socket(){

    mensajes_recibidos++;

    QByteArray buffer;
    buffer.resize(_socket->bytesAvailable());
    _socket->read(buffer.data(), buffer.size());

    QString message = QString(buffer).toLatin1().data();
    ui->plainTextEdit->appendPlainText(message);

    handle_mensaje(message, mensajes_recibidos);


}

void server::comparar_cartas(size_t index1){

    //verifica si está cargada la carta 2
    size_t ind2 = pagedMatrix->buscar_CartasCargadas(fila2, columna2);

    bool sameType = pagedMatrix->inMemoryCards.at(index1).carta.type
            == pagedMatrix->inMemoryCards.at(ind2).carta.type;

    if(sameType){
        pagedMatrix->onDiscCards.clear();
        pagedMatrix->inMemoryCards.clear();
        pagedMatrix->update_array(fila1, columna1, fila2, columna2);
        enviar_al_cliente("1");
    }else{
        enviar_al_cliente("0");
    }

    // necesito los indices para luego comparar los tipos y así ya esta lista la mecanica del juego
}

void server::enviar_imagen(){

}

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

void server::fill_inMemory(){
    pagedMatrix = new PagedMatrix();
    pagedMatrix->llenar_array(); // esto internamente llama a la funcion para escribir en el .bin
    pagedMatrix->leer_arrayArchivo();
    pagedMatrix->llenar_inMemory();
    mostrar_cartasDisco();
}

void server::mostrar_cartasDisco(){

    vector<Card> discCopy = pagedMatrix->onDiscCards;
    vector<Card> mem = pagedMatrix->inMemoryCards;
    int hits = pagedMatrix->pageHits;
    int faults = pagedMatrix->pageFaults;

    qDebug()<<"Mem size "<<mem.size()<<" Disc size"<<discCopy.size();

    qDebug()<<"DISC CARDS----------MEMORY CARDS----------HITS----------FAULTS"<<endl;
    for(size_t i = 0; i<discCopy.size(); i++){

        if(i>=mem.size()){
            qDebug()<<discCopy.at(i).carta.row<<discCopy.at(i).carta.column<<
                      discCopy.at(i).carta.type<<discCopy.at(i).carta.ganada
                       <<"       ";

        }else{
            if(i == 0){
                qDebug()<<discCopy.at(i).carta.row<<discCopy.at(i).carta.column<<
                          discCopy.at(i).carta.type<<discCopy.at(i).carta.ganada<<
                          "       "<<mem.at(i).carta.row<<mem.at(i).carta.column<<
                          mem.at(i).carta.type<<mem.at(i).carta.ganada<<"        "<<
                          hits<<"        "<<faults;
            }else{
                qDebug()<<discCopy.at(i).carta.row<<discCopy.at(i).carta.column<<
                          discCopy.at(i).carta.type<<discCopy.at(i).carta.ganada<<
                          "       "<<mem.at(i).carta.row<<mem.at(i).carta.column<<
                          mem.at(i).carta.type<<mem.at(i).carta.ganada;
            }
        }
    }
}

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

void server::handle_mensaje(QString mensaje, int mensajesRecibidos){

    llego_segundaCarta = mensajesRecibidos % 2 == 0 && mensajesRecibidos > 2;

    if(mensajesRecibidos == 1){         // mensaje recibido = nombre player 1
        ui->nameP1->setText(mensaje);

    }else if(mensajesRecibidos == 2){   // mensaje recibido = nombre player 2
        ui->nameP2->setText(mensaje);

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
    }

}

void server::on_send_clicked()
{

    enviar_al_cliente(ui->lineEdit->text());

    ui->plainTextEdit->appendPlainText(ui->lineEdit->text());
    ui->lineEdit->clear();

    //pagedMatrix->llamar_matrizDisco(); OPCION 1

}

void server::enviar_al_cliente(QString message){
    _socket->write(message.toLatin1().data(), message.size());

}

server::~server()
{
    delete ui;
}
