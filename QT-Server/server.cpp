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

    QByteArray buffer;
    buffer.resize(_socket->bytesAvailable());
    _socket->read(buffer.data(), buffer.size());

    QString message = QString(buffer);

    ui->plainTextEdit->appendPlainText(message);
}

server::~server()
{
    delete ui;
}

void server::on_send_clicked()
{
    const char* message = ui->lineEdit->text().toLatin1().data();

    _socket->write(message, ui->lineEdit->text().size());

    ui->plainTextEdit->appendPlainText(ui->lineEdit->text());
    ui->lineEdit->clear();
}
