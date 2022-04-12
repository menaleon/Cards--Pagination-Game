#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include<QTcpSocket>

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

private:
    Ui::client *ui;
    QTcpSocket* _socket;
};

#endif // CLIENT_H
