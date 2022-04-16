#include "card.h"

Card::Card(int row, int col, int type, int ganada)
{
    this->carta.row = row;
    this->carta.column = col;
    this->carta.type = type;
    this->carta.ganada = ganada;

    //this->image = "SoyUnaImagen";
}

void Card::show(){
    qDebug()<<"SHOW-CARD: "<<carta.row<<carta.column<<carta.type<<carta.ganada<<endl;

}
