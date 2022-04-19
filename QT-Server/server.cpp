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

    ui->onDisc_cards->setPlainText("HOla");
    pagedMatrix = new PagedMatrix();
    server::fill_inMemory();

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

    handle_mensaje(message, mensajes_recibidos);

    ui->plainTextEdit->appendPlainText(message);  
}

void server::comparar_cartas(){
    verificar_cartaInMemory();
    //vector<Card> temp = pagedMatrix->leer_arrayArchivo(fila1, columna1, fila2, columna2, false);
    //mostrar_cartasDisco(temp);

}

void server::enviar_imagen(){

}

void server::verificar_cartaInMemory(){

    if(mensajes_recibidos % 2 == 0 && mensajes_recibidos > 2){

        //pagedMatrix->leer_arrayArchivo(fila1, columna1, fila2, columna2, false);
        size_t w = pagedMatrix->buscar_CartasCargadas(fila1, columna1);
        qDebug()<<"Buscar cartas result "<<w<<endl;
        size_t q = pagedMatrix->buscar_CartasCargadas(fila2, columna2);
        qDebug()<<"Buscar cartas result "<<q<<endl;

    }else{
        size_t e = pagedMatrix->buscar_CartasCargadas(fila1, columna1);
        qDebug()<<"Buscar cartas result "<<e<<endl;
    }
}

void server::fill_inMemory(){
    pagedMatrix = new PagedMatrix();
    pagedMatrix->llenar_array();
    pagedMatrix->leer_arrayArchivo(0,0,0,0, true);
    pagedMatrix->llenar_inMemory();
}

void server::mostrar_cartasDisco(vector<Card> cargadas){

    vector<Card> copy = cargadas;

    for(size_t i = 0; i<cargadas.size(); i++){
        cargadas.at(i).show();
        qDebug()<<"Aqui irian las cartas cargadas tambien";
    }
    qDebug()<<"ONdisc and inMemory------------------------";

    cargadas.clear();
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
        //enviar_imagen();
        //comparar_cartas();
        qDebug()<<"Fila2 columna2"<<fila2<<columna2;
        pagedMatrix->buscar_CartasCargadas(fila2, columna2);

    }else{                              // mensaje recibido = primera carta
        descomponer_indices(mensaje, 1);
        qDebug()<<"Fila1 columna1"<<fila1<<columna1;

        pagedMatrix->buscar_CartasCargadas(fila1, columna1);
        enviar_imagen();
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
