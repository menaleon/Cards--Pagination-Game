#ifndef CARD_H
#define CARD_H

#include<QDebug>
#include<QByteArray>


struct Carta{
    int row;
    int column;
    int type;
    bool ganada;
};

class Card
{
public:
    Card(int row, int col, int type, int ganada);
    Card(Carta structCarta);


    Carta carta;

    //QByteArray imagen;
    char image[50];


    void show();
    int getRow();
    int getColumn();
    int getType();
    int getGanada();
};

#endif // CARD_H
