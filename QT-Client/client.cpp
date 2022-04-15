#include "client.h"

const char filaIndex[] = "fila";
const char columnaIndex[] = "columna";
const char ganada[] = "CartaGanada";

client::client(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::client)
{
    crear_interfaz_inicial();

    auto client = this;
    auto interfaz = this->ui;

    responseFromServer_Handler = new handler(interfaz);


}

void client::crear_interfaz_inicial(){
    ui->setupUi(this);
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

    rellenar_matrizBotones();
}

void client::rellenar_matrizBotones(){

    for(int columnas = 0; columnas<6; columnas++){
        for(int filas = 0; filas<6; filas++){

            QString indices = QString{"(%1,%2)"}.arg(columnas).arg(filas);

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

void client::leer_mensaje(){

    QByteArray buffer;
    buffer.resize(_socket->bytesAvailable());
    _socket->read(buffer.data(), buffer.size());
    QString mensaje = QString(buffer);

    int caso = responseFromServer_Handler->
            handle_message(mensaje, counter_cartasPresionadas);

    qDebug()<<"CASO "<<caso<<endl;
    enable_botonEnMatriz(caso);

}

void client::enviar_al_server(QString message){
    _socket->write(message.toLatin1().data(), message.size());
}

void client::obtenerIndices_cartaPresionada(){


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

void client::set_botonEnHandler(int columna, int fila){

    if(counter_cartasPresionadas %2 == 0){

        responseFromServer_Handler->columna2 = columna;
        responseFromServer_Handler->fila2 = fila;

    }else{

        responseFromServer_Handler->columna1 = columna;
        responseFromServer_Handler->fila1 = fila;
    }
}

void client::enable_botonEnMatriz(int caso){

    int f1 = responseFromServer_Handler->fila1;
    int c1 = responseFromServer_Handler->columna1;
    int f2 = responseFromServer_Handler->fila2;
    int c2 = responseFromServer_Handler->columna2;

    switch (caso) {

    case 3: // CARTAS IGUALES: establece botones como "ganados"

        matrizBotones[c1][f1]->property(ganada) = true;
        matrizBotones[c2][f2]->property(ganada) = true;
        cartasGanadas += 2;

        check_estadoJuego();
        set_turnoActual(0);

        break;

    case 4: // CARTAS DISTINTAS: habilita los botones nuevamente
        matrizBotones[c1][f1]->setEnabled(true);
        matrizBotones[c2][f2]->setEnabled(true);
        set_turnoActual(0);
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

void client::check_estadoJuego(){

    if(cartasGanadas == 36){
        ui->juegoFInalizado->show();
    }
}

void client::set_turnoActual(int caso){

    int player = ui->turnoLabel->text().toInt();

    if(caso == 0){
        if(player == 1){
            ui->turnoLabel->setText("2");
        }else{
            ui->turnoLabel->setText("1");
        }
    }else{
        if(caso == 2){
            ui->turnoLabel->setText("2");
        }else{
            ui->turnoLabel->setText("1");
        }
    }
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




