#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include<QTcpServer>
#include<QTcpSocket>

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

    void on_send_clicked();

private:
    Ui::server *ui;
    QTcpServer* _server;
    QTcpSocket* _socket;
};

#endif // SERVER_H
