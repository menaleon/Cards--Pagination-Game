#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include<QTcpServer>
#include<QTcpSocket>
#include "pagedmatrix.h"
#include "card.h"


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
    void handle_mensaje(QString mensaje, int mensajesRecibidos);
    void descomponer_indices(QString mensaje, int whichCard);
    void on_send_clicked();
    void enviar_al_cliente(QString message);
    void comparar_cartas(size_t index1);
    void enviar_imagen();
    void mostrar_cartasDisco();
    void fill_inMemory();
    size_t verificar_cartaInMemory(bool secondCard);


private:

    Ui::server *ui;
    QTcpServer* _server;
    QTcpSocket* _socket;

    int mensajes_recibidos = 0;
    int player1 = 1;
    bool llego_segundaCarta;

    int columna1;
    int fila1;

    int columna2;
    int fila2;

    size_t index1;

    PagedMatrix* pagedMatrix;
};

#endif // SERVER_H
