#pragma once

#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include<QTcpSocket>
#include<iostream>
#include<QPushButton>
#include "ui_client.h"
#include "handler.h"
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
    Ui::client *ui;

private slots:
    void crear_interfaz_inicial();
    void crear_interfaz_juego();
    void rellenar_matrizBotones();
    void set_botonEnHandler(int columna, int fila);
    void enable_botonEnMatriz(int caso);
    void obtenerIndices_cartaPresionada();
    void check_estadoJuego();
    void set_turnoActual(int caso);
    void on_enviarNombrePlayer_clicked();

    QTcpSocket* crear_socket();
    void leer_mensaje();
    void enviar_al_server(QString message);

private:

    QTcpSocket* _socket;
    QPushButton* matrizBotones[6][6];
    handler* responseFromServer_Handler;

    int counter = 0;
    int counter_cartasPresionadas = 0;
    int cartasGanadas = 0;
    int player1 = 1;

    Ui::client interfaz;



};

#endif // CLIENT_H
