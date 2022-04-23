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

    //PASAR A PRIVATE
    Carta carta;
    //QByteArray imagen;
    char image[50];

    int getRow();
    int getColumn();
    int getType();
    int getGanada();
    Carta getStructCarta();
    char getImage();

    void setRow();
    void setColumn();
    void setType();
    void setGanada();
    void setStructCarta();
    void setImage();


private:



};

#endif // CARD_H
