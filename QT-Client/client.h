#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include<QTcpSocket>
#include<iostream>
#include<QPushButton>

namespace Ui {
class client;
}

class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = nullptr);
    ~client();

private slots:
    void leer();
    void on_enviar_clicked();
    void generar_interfaz();
    void carta_presionada();

private:
    Ui::client *ui;
    QTcpSocket* _socket;
    QPushButton* matrizBotones[6][6];
    int counter = 0;


signals:
    void onClicked(int fila, int columna);
};

#endif // CLIENT_H
