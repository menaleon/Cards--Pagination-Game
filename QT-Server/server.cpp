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
    }else{                              // mensaje recibido = primera carta
        descomponer_indices(mensaje, 1);
    }
}

server::~server()
{
    delete ui;
}

void server::on_send_clicked()
{

    enviar_al_cliente(ui->lineEdit->text());

    ui->plainTextEdit->appendPlainText(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void server::enviar_al_cliente(QString message){
    _socket->write(message.toLatin1().data(), message.size());

}
