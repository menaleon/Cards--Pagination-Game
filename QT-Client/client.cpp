#include "client.h"

const char filaIndex[] = "fila";
const char columnaIndex[] = "columna";

client::client(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::client)
{
    crear_interfaz_inicial();
    responseFromServer_Handler = new adapter();
}

void client::crear_interfaz_inicial(){
    ui->setupUi(this);
    ui->plainTextEdit->setReadOnly(true);
    ui->player2Label->hide();
    ui->nameP1->hide();
    ui->nameP2->hide();
    ui->playingLabel->hide();
    ui->turnoLabel->hide();

    QTcpSocket* _socket = crear_socket();

    connect(_socket, SIGNAL(readyRead()), this, SLOT(leer_mensaje()));

}

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

void client::crear_interfaz_juego(){
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
            matrizBotones[columnas][filas]->setGeometry(QRect(QPoint((columnas+1)*50 +
                                                                     (columnas+1)*15,
                                                                     (filas+1)*50 +
                                                                     (filas+1)*10),
                                                                      QSize(50,50)));

            matrizBotones[columnas][filas]->setProperty(filaIndex, filas);
            matrizBotones[columnas][filas]->setProperty(columnaIndex, columnas);

            connect(matrizBotones[columnas][filas], SIGNAL(clicked()), this,
                                    SLOT(obtenerIndices_cartaPresionada()));

            matrizBotones[columnas][filas]->setVisible(true);
        }
    }
}

void client::leer_mensaje(){

    QByteArray buffer;
    buffer.resize(_socket->bytesAvailable());
    _socket->read(buffer.data(), buffer.size());
    QString mensaje = QString(buffer);

    QMap<QString, int> responseType =
            responseFromServer_Handler->
            handle_message(mensaje, counter_cartasPresionadas);

    qDebug()<<responseType.first()<<endl;
}

void client::enviar_al_server(QString message){
    _socket->write(message.toLatin1().data(), message.size());
}

void client::obtenerIndices_cartaPresionada(){

    counter_cartasPresionadas +=1;

    QString fila = sender()->property(filaIndex).toString();
    QString columna = sender()->property(columnaIndex).toString();

    QString mensaje;
    mensaje.append(fila);
    mensaje.append(columna);

    enviar_al_server(mensaje);
}

bool client::set_TurnoActual(int player){

    if(!(player == 1) || !(player == 2)){
        return false;
    }

    turnoActual = player;
    ui->turnoLabel->setText((QString) turnoActual);

    return true;
}

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







client::~client()
{
    delete ui;
}




