#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include<QTcpSocket>
#include<iostream>
#include<QPushButton>
#include "ui_client.h"
#include "adapter.h"
using namespace std;

namespace Ui {
class client;
}

class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = nullptr);
    ~client();

    bool set_TurnoActual(int player);

private slots:
    void crear_interfaz_inicial();
    void crear_interfaz_juego();
    void obtenerIndices_cartaPresionada();
    void on_enviarNombrePlayer_clicked();

    QTcpSocket* crear_socket();
    void leer_mensaje();
    void enviar_al_server(QString message);



private:
    Ui::client *ui;

    QTcpSocket* _socket;
    QPushButton* matrizBotones[6][6];
    adapter* responseFromServer_Handler;

    int counter = 0;
    int turnoActual; //puede ser 1 o 2
    int counter_cartasPresionadas = 0;



};

#endif // CLIENT_H
