#include "pagedmatrix.h"
#include <QDebug>

int cont = 0;
int contLeer = 0;

PagedMatrix::PagedMatrix()
{

}


void PagedMatrix::leer_arrayArchivo(){
    Carta temp[3];

    disco.open("cartas2.bin", ios::in | ios::binary);

    disco.read(reinterpret_cast<char*>(temp), 3* sizeof(Carta));

    disco.close();

    for(Carta& c : temp){
        qDebug()<<"Lei"<<disco.tellg()<<c.row<<c.column<<c.type<<c.ganada<<endl;

    }
}

//llenar_array, generar archivo, update array, generar archivo, guardar en nueva struct
void PagedMatrix::generar_archivo(Carta all_cards[3]){


    disco.open("cartas2.bin", ios::binary | ios::out);
    disco.write(reinterpret_cast<char*>(all_cards), 3* sizeof (Carta));
    disco.close();
    qDebug()<<"Archivo generado"<<endl;
}

void PagedMatrix::llenar_array(){
    for(int i = 0; i<3; i++){
        allCards[i] = {i,i,i,false};
        qDebug()<<allCards[i].row<<allCards[i].column<<allCards[i].type<<allCards[i].ganada<<endl;
    }
}

void PagedMatrix::update_array(int whichCard){
    allCards[whichCard] = {whichCard+3,whichCard+3,whichCard+3,false};
}

void PagedMatrix::llamar_matrizDisco(){
    writeFirst_matrizDisco({4,5,1,false});
    append_aDisco({8,8,8,false});
    append_aDisco({6,7,8,false});
    modificar_disco(2);



}

void PagedMatrix::write_in_position(Carta cartaStr, int pos){
    disco.open("cartas.bin", ios::out | ios::in | ios::binary);

    qDebug()<<disco.tellg()<<" First";

    disco.seekp((2) * sizeof(cartaStr), ios::beg);
    qDebug()<<"Hehe"<<disco.tellg()<<endl;

    disco.write(reinterpret_cast<char*>(&cartaStr), sizeof(cartaStr));
    disco.close();
    qDebug()<<"wrote again -------------------------------------"<<endl;


}

void PagedMatrix::modificar_disco(int cualCarta){
    leer2_matriz(1);
    leer2_matriz(2);
    leer2_matriz(3);
    write_in_position({9,9,9,false}, 2);
    leer2_matriz(1);
    leer2_matriz(2);
    leer2_matriz(3);
    leer2_matriz(4);

}



void PagedMatrix::leer2_matriz(int cartaNum){
    Carta c;
    disco.open("cartas.bin", ios::in | ios::binary);
    disco.seekg((cartaNum -1) * sizeof(c), ios::beg);
    disco.read(reinterpret_cast<char*>(&c), sizeof(c));
    disco.close();

    qDebug()<<disco.tellg()<<c.row<<c.column<<c.type<<c.ganada<<endl;

}

void PagedMatrix::leer_matrizDisco(int howManyStructs){
    Carta ccc3;
    Carta ccc4;
    disco.open("cartas.bin", ios::in | ios::binary);

    disco.read(reinterpret_cast<char*>(&ccc3), sizeof(ccc3));
    disco.read(reinterpret_cast<char*>(&ccc4), sizeof(ccc4));
    disco.close();

    qDebug()<<disco.tellg()<<endl;
    qDebug()<<ccc3.row<<ccc3.column<<ccc3.type<<ccc3.ganada<<endl;

    qDebug()<<disco.tellg()<<endl;
    qDebug()<<ccc4.row<<ccc4.column<<ccc4.type<<ccc4.ganada<<endl;
}

void PagedMatrix::append_aDisco(Carta cartaStr){
    //Carta ccc2 = {8,8,8,true};
    disco.open("cartas.bin", ios::app | ios::binary);
    disco.write(reinterpret_cast<char*>(&cartaStr), sizeof(cartaStr));
    disco.close();

}

void PagedMatrix::writeFirst_matrizDisco(Carta cartaStr){
    //Carta ccc = {4,5,1,false};
    disco.open("cartas.bin", ios::out | ios::binary);
    disco.write(reinterpret_cast<char*>(&cartaStr), sizeof(cartaStr));
    disco.close();

}

void PagedMatrix::buscar_cartaEnDisco(){

}
