
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


    //GETTERS Y SETTERS
    int getFila1();
    int getFila2();
    int getColumna1();
    int getColumna2();

    void setFila1(int newFila1);
    void setFila2(int newFila2);
    void setColumna1(int newCol1);
    void setColumna2(int newCol2);


private:

    Ui::client* interfaz;
    bool dosCartas_clicked;
    bool ceroCartas_clicked;
    int fila1;
    int columna1;
    int fila2;
    int columna2;

};

#endif // HANDLER_H
