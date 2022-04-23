#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include<QTcpServer>
#include<QTcpSocket>
#include "pagedmatrix.h"
#include "card.h"
#include <string>
#include<string.h>
#include<iostream>
#include <QDebug>


namespace Ui {
class server;
}

class server : public QMainWindow
{
    Q_OBJECT

public:
    explicit server(QWidget *parent = nullptr);
    ~server();

private slots:
    void conexion_nueva();
    void leer_socket();
    void enviar_al_cliente(QString message);

    void on_send_clicked();

    void handle_mensaje(QString mensaje, int mensajesRecibidos); 
    void descomponer_indices(QString mensaje, int whichCard);
    void comparar_cartas(size_t index1);
    void enviar_imagen();
    void mostrar_cartasDisco();
    void create_inMemory();
    size_t verificar_cartaInMemory(bool secondCard);

    void cambiar_turnoJugador(bool firstTime);
    void manejar_puntajes(bool matchedCards, int who, int whichPowerUp);

private:

    Ui::server *ui;
    QTcpServer* _server;
    QTcpSocket* _socket;

    PagedMatrix* pagedMatrix;

    int mensajes_recibidos = 0;
    bool llego_segundaCarta;

    int player1 = 1;
    int curr_player = 0;
    int p1_points;
    int p2_points;

    int columna1;
    int fila1;
    int columna2;
    int fila2;
    size_t index1;


};

#endif // SERVER_H
