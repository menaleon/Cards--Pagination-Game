#include "client.h"
#include "ui_client.h"

using namespace std;

//int counter = 0;
const char filaIndex[] = "fila";
const char columnaIndex[] = "columna";
//QPushButton* matrizBotones[6][6];

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    ui->plainTextEdit->setReadOnly(true);

    ui->player2Label->hide();
    ui->nameP1->hide();
    ui->nameP2->hide();
    ui->playingLabel->hide();
    ui->turnoLabel->hide();

    _socket = new QTcpSocket(this);
    _socket->connectToHost("localhost", 4050);

    if(_socket->waitForConnected(4050)){

        ui->plainTextEdit->appendPlainText("You and server are now connected :D");
    }else{

        ui->plainTextEdit->appendPlainText("Could not connect to server :( Exit and try again");

        ui->enviar->hide();
        ui->enviar->deleteLater();
        ui->lineEdit->hide();
        ui->lineEdit->deleteLater();
        ui->playerLabel->hide();
        ui->playerLabel->deleteLater();

    }

    QObject::connect(_socket, SIGNAL(readyRead()), this, SLOT(leer()));

}

void client::leer(){
    QByteArray buffer;
    buffer.resize(_socket->bytesAvailable());
    _socket->read(buffer.data(), buffer.size());
    QString mensaje = QString(buffer);
    qDebug()<<mensaje<<endl;

    //ui->plainTextEdit->appendPlainText(QString(buffer));
}

void client::generar_interfaz(){
    ui->plainTextEdit->hide();
    ui->plainTextEdit->deleteLater();

    ui->playerLabel->show();

    ui->player2Label->show();
    ui->nameP1->show();
    ui->nameP2->show();
    ui->playingLabel->show();
    ui->turnoLabel->show();

    for(int columnas = 0; columnas<6; columnas++){
        for(int filas = 0; filas<6; filas++){

            QString indices = QString{"(%1,%2)"}.arg(columnas).arg(filas);
            QString filaButon = (QString) filas;

            matrizBotones[columnas][filas] = new QPushButton("Button", this);
            matrizBotones[columnas][filas]->setGeometry(QRect(QPoint((columnas+1)*50 + (columnas+1)*15, (filas+1)*50
                                                                     + (filas+1)*10), QSize(50,50)));

            matrizBotones[columnas][filas]->setProperty(filaIndex, filas);
            matrizBotones[columnas][filas]->setProperty(columnaIndex, columnas);

            connect(matrizBotones[columnas][filas], SIGNAL(clicked()), this, SLOT(carta_presionada()));
            matrizBotones[columnas][filas]->setVisible(true);
        }
    }
}

client::~client()
{
    delete ui;
}

void client::on_enviar_clicked(){

    if(counter == 1){
        ui->enviar->hide();
        ui->enviar->deleteLater();

        ui->playerLabel->hide();
        ui->playerLabel->move(50, 430);

        ui->lineEdit->hide();
        ui->lineEdit->deleteLater();
        //ui->lineEdit->move(100,300);

        generar_interfaz();
    }

    ui->playerLabel->setText("Player 2: ");

    const char* message = ui->lineEdit->text().toLatin1().data();
    qint64 msgSize = ui->lineEdit->text().size();
    _socket->write(message, msgSize);
    ui->lineEdit->clear();

    counter +=1;

}

void client::carta_presionada(){

    QString fila = sender()->property(filaIndex).toString();
    QString columna = sender()->property(columnaIndex).toString();

    QString mensaje;
    mensaje.append(fila);
    mensaje.append(columna);

    _socket->write(mensaje.toLatin1().data(), mensaje.size());
}
