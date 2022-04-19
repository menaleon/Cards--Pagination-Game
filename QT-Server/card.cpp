#include "card.h"

Card::Card(int row, int col, int type, int ganada)
{
    this->carta.row = row;
    this->carta.column = col;
    this->carta.type = type;
    this->carta.ganada = ganada;

    //this->image = "SoyUnaImagen";
}

Card::Card(Carta structCarta)
{
    this->carta.row = structCarta.row;
    this->carta.column = structCarta.column;
    this->carta.type = structCarta.type;
    this->carta.ganada = structCarta.ganada;

    //this->image = "SoyUnaImagen";
}
void Card::show(){
    qDebug()<<"SHOW-CARD: "<<carta.row<<carta.column<<carta.type<<carta.ganada<<endl;

}

int Card::getRow(){
    return this->carta.row;
}
