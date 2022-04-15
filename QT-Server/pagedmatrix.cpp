#include "pagedmatrix.h"
#include <QDebug>

PagedMatrix::PagedMatrix()
{
}

void PagedMatrix::crear_disco(){
    disco.open("cartas.bin", ios::out | ios:: binary);

}

void PagedMatrix::leer_disco(){

    int numCopy;

    disco.open("cartas.bin", ios::out | ios:: binary);

    if(disco.is_open()){
        disco.read(reinterpret_cast<char *>(&numCopy), sizeof (numCopy));
        disco.close();
        qDebug()<<"READ!!"<<endl;


    }else{
        qDebug()<<"ERROR reading disco"<<endl;

    }

    qDebug()<<"NumCopy "<<numCopy<<endl;
}

void PagedMatrix::escribir_en_Disco(){

    disco.open("cartas.bin", ios::out | ios:: binary);
    int num = 65;

    if(disco.is_open()){
        disco.write(reinterpret_cast<char *>(&num), sizeof (num));
        disco.close();
        qDebug()<<"Wroteeee"<<endl;
    }else{
        qDebug()<<"ERROR writing disco"<<endl;
    }

}
