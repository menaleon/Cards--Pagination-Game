#include "pagedmatrix.h"

int cont_llenadoInMemory = 0;

PagedMatrix::PagedMatrix()
{

}

int PagedMatrix::buscar_CartasCargadas(vector<Card> cargadas, int fila, int col){
    for(int i = 0; i<36; i++){
        if(onDiscCards.at(i).carta.row == fila && onDiscCards.at(i).carta.column == col){

            return i;
        }

    }
}


vector<int> PagedMatrix::random_int(size_t vectorSize){
    int num;
    vector<int> randIndex;

    for(size_t cont = 1; cont <=vectorSize; cont++){
        num = 0 + rand()%(36 + 1 - 0);
        randIndex.push_back(num);
    }
    return randIndex;
}

vector<Card> PagedMatrix::leer_arrayArchivo(int f1, int c1, int f2, int c2){
    Carta temp[36];
    disco.open("cartas2.bin", ios::in | ios::binary);
    disco.read(reinterpret_cast<char*>(temp), 36* sizeof(Carta));
    disco.close();

    for(Carta& c : temp){
        Card* temporal = new Card(c.row, c.column, c.type, c.ganada);
        onDiscCards.push_back(*temporal);
        free(temporal);
    }
    //Esto es para lidiar después con saber cuál añado al vector de tarjetas cargadas
    //La idea es hacer un random para que al inicio cargue 12 cartas al otro vector inMemory

    //int ind1 = buscar_CartasCargadas(onDiscCards, f1, c1);
    //int ind2 = buscar_CartasCargadas(onDiscCards, f2, c2);
    //Card* tarjeta1 = new Card(onDiscCards.at(ind1).carta);
    //Card* tarjeta2 = new Card(onDiscCards.at(ind2).carta);

    return onDiscCards;
}

void PagedMatrix::llenar_inMemory(){
    vector<int> randIndexes;
    long ind;


    if(cont_llenadoInMemory == 0){
         randIndexes = random_int(12);

    }else{
        size_t useful_size = verificar_ganadas();
        randIndexes = random_int(useful_size);
    }

    qDebug()<<"Filling inMemory ----------------------------"<<endl;

    for(size_t sizeIndexes= 0; sizeIndexes<randIndexes.size(); sizeIndexes++){
        ind = randIndexes.at(sizeIndexes);
        inMemoryCards.push_back(onDiscCards.at(ind));
        inMemoryCards.at(sizeIndexes).show();
    }
}

size_t PagedMatrix::verificar_ganadas(){
    size_t checkedSize = 36;
    for(size_t cont = 0; cont <onDiscCards.size(); cont++){
        if(onDiscCards.at(cont).carta.ganada == true){
            checkedSize--;
        }
    }
    return checkedSize;
}

void PagedMatrix::escribir_archivo(Carta arrayCartas[36]){
    disco.open("cartas2.bin", ios::binary | ios::out);
    disco.write(reinterpret_cast<char*>(arrayCartas), 36 * sizeof (Carta));
    disco.close();
    qDebug()<<"Archivo binario con 36 cartas generado-------"<<endl;
}

void PagedMatrix::llenar_array(){

    int index = 0;

    for(int col = 0; col<6; col++){

        for(int fila = 0; fila<6; fila++){

            allCards[index] = {fila,col,0,false};
            //qDebug()<<allCards[index].row<<allCards[index].column<<
            //allCards[index].type<<allCards[index].ganada<<endl;
            index++;
        }
    }

    escribir_archivo(allCards);
}

void PagedMatrix::update_array(int whichCard){
    allCards[whichCard] = {whichCard+3,whichCard+3,whichCard+3,false};
}









































//----RELATED TO OPCIÓN 1 para modelar la matriz en disco. No borrar aún, por si acaso.

/**
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

}**/
