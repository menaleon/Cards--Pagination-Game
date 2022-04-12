#include "client.h"
#include "ui_client.h"

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    _socket = new QTcpSocket(this);
    _socket->connectToHost("localhost", 4050);

    if(_socket->waitForConnected(4050)){
        ui->plainTextEdit->appendPlainText("Se conecto correctamente");
    }else{
        ui->plainTextEdit->appendPlainText("No se conecto");
    }

    connect(_socket, SIGNAL(readyRead()), this, SLOT(leer()));

}

void client::leer(){
    QByteArray buffer;
    buffer.resize(_socket->bytesAvailable());
    _socket->read(buffer.data(), buffer.size());
    ui->plainTextEdit->setReadOnly(true);
    ui->plainTextEdit->appendPlainText(QString(buffer));
}

client::~client()
{
    delete ui;
}

void client::on_enviar_clicked()
{
    _socket->write(ui->lineEdit->text().toLatin1().data(), ui->lineEdit->text().size());
    ui->lineEdit->clear();
}
