
#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QPushButton>
#include "ui_client.h"


using namespace std;


class handler
{
public:
    handler(Ui::client* interface);

    int handle_message(QString mensaje, int counter_cartasPresionadas);

    void set_direccionImagen(QString mensaje, int whichButton);
    int set_comparacionCartas(QString mensaje);
    int set_turnoPlayer(QString mensaje);

    int fila1;
    int columna1;

    int fila2;
    int columna2;

private:
    Ui::client* interfaz;
    bool dosCartas_clicked;
    bool ceroCartas_clicked;

};

#endif // HANDLER_H
