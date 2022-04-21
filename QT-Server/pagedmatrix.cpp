#include "pagedmatrix.h"

int cont_llenadoInMemory = 0;


PagedMatrix::PagedMatrix()
{

}


bool PagedMatrix::cargar_carta(int fila, int col){
    qDebug()<<"ondisc size "<<onDiscCards.size()<<endl;

    for(size_t i = 0; i<onDiscCards.size(); i++){ //lo hago en un for porque no siempre serán 12 cartas en inMemory
        if(onDiscCards.at(i).carta.row == fila && onDiscCards.at(i).carta.column == col){
            qDebug()<<"inMemory size "<<inMemoryCards.size()<<endl;
           inMemoryCards.pop_back();
           inMemoryCards.push_back(onDiscCards.at(i)); // carga la tarjeta a inMemory
           pageFaults++;
           return true;
        }
    }

    return false;
}

size_t PagedMatrix::buscar_CartasCargadas(int fila, int col){

    bool isLoaded = false;

    if(fila == -1 && col == -1){ //solo me piden buscar una carta
        qDebug()<<"solo me pidieron una carta"<<endl;
        return 50;
    }


    for(size_t i = 0; i<inMemoryCards.size(); i++){ //lo hago en un for porque no siempre serán 12 cartas en inMemory
        if(inMemoryCards.at(i).carta.row == fila && inMemoryCards.at(i).carta.column == col){
            isLoaded = true; // si encuentra la carta entonces sí estaba cargada en memoria
            pageHits++;
            return i;
        }
    }

    if( !isLoaded){ // si no está cargada, la carga y devuelve su index
        cargar_carta(fila, col);
        for(size_t i = 0; i<inMemoryCards.size(); i++){ //lo hago en un for porque no siempre serán 12 cartas en inMemory
            if(inMemoryCards.at(i).carta.row == fila && inMemoryCards.at(i).carta.column == col){
                return i;
            }
        }
    }
    return 100; // la carta no está cargada
}

vector<int> PagedMatrix::random_int(size_t vectorSize){
    int num;
    vector<int> randIndex;

    randIndex.clear();

    for(size_t cont = 0; cont < vectorSize; cont++){
        num = 0 + rand()%(35 + 1 - 0);
        randIndex.push_back(num);
    }

    return randIndex;
}

vector<Card> PagedMatrix::leer_arrayArchivo(){ //devuelvo el disco leido para mostrarlo en el server
    Carta temp[36];
    disco.open("cartas2.bin", ios::in | ios::binary);
    disco.read(reinterpret_cast<char*>(temp), 36* sizeof(Carta));
    disco.close();

    onDiscCards.clear();
    inMemoryCards.clear();

    for(Carta& c : temp){
        Card* temporal = new Card(c.row, c.column, c.type, c.ganada);
        onDiscCards.push_back(*temporal);
        free(temporal);
    }
    return onDiscCards;
}

void PagedMatrix::llenar_inMemory(){ // al iniciar el juego lleno el vector con 12 cartas random las cuales leo desde el disco
    vector<int> randIndexes;
    int ind;

    randIndexes.clear();
    inMemoryCards.clear();

    if(cont_llenadoInMemory == 0){
         randIndexes = random_int(12);
         cont_llenadoInMemory++;

    }else{
        cont_llenadoInMemory++;
        size_t useful_size = verificar_ganadas();
        randIndexes = random_int((36-useful_size)/3);
    }


    for(size_t cont= 0; cont<randIndexes.size(); cont++){
        ind = randIndexes.at(cont); // me devuelve numeros que fueron creados aleatoriamente
        inMemoryCards.push_back(onDiscCards.at(ind)); //carga esas tarjetas en memoria
    }
}

size_t PagedMatrix::verificar_ganadas(){

    size_t checkedSize = 0;

    for(size_t cont = 0; cont <onDiscCards.size(); cont++){
        if(onDiscCards.at(cont).carta.ganada == true){
            checkedSize++;
        }
    }
    return checkedSize;
}

void PagedMatrix::escribir_archivo(Carta arrayCartas[36]){
    disco.open("cartas2.bin", ios::binary | ios::out);
    disco.write(reinterpret_cast<char*>(arrayCartas), 36 * sizeof (Carta));
    disco.close();
    qDebug()<<"-------El archivo binario se modificó-------"<<endl;
}

void PagedMatrix::llenar_array(){ //esto es para definir las cartas que usaremos. debo lidiar con los types

    int index = 0;

    for(int col = 0; col<6; col++){
        for(int fila = 0; fila<6; fila++){
            allCards[index] = {fila,col,0,false};
            index++;
        }
    }
    escribir_archivo(allCards);
}

void PagedMatrix::update_array(int f1, int c1, int f2, int c2){

    for(int i = 0; i<36; i++){
        if(allCards[i].row == f1 && allCards[i].column == c1){
            allCards[i].ganada = true;
        }

        if(allCards[i].row == f2 && allCards[i].column == c2){
            allCards[i].ganada = true;
        }
    }
    escribir_archivo(allCards);
    leer_arrayArchivo();
    llenar_inMemory();
}









































//----RELATED TO OPCIÓN 1 para modelar la matriz en disco. No borrar aún, por si acaso.
/**
void PagedMatrix::llamar_matrizDisco(){
    writeFirst_matrizDisco({4,5,1,false});
    append_aDisco({8,8,8,false});
    append_aDisco({6,7,8,false});
    modificar_disco(1);



}

void PagedMatrix::write_in_position(Carta cartaStr, int pos){
    disco.open("cartas.bin", ios::out | ios::in | ios::binary);

    qDebug()<<disco.tellg()<<" First";

    unsigned long posi = *reinterpret_cast<long*>(&pos);
    unsigned long one = 1;

    unsigned long index = posi - one;

    disco.seekp((index) * sizeof(cartaStr), ios::beg);
    qDebug()<<"Hehe"<<disco.tellg()<<endl;

    disco.write(reinterpret_cast<char*>(&cartaStr), sizeof(Carta));
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
    disco.read(reinterpret_cast<char*>(&c), sizeof(Carta));
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
    disco.write(reinterpret_cast<char*>(&cartaStr), sizeof(Carta));
    disco.close();

}

void PagedMatrix::writeFirst_matrizDisco(Carta cartaStr){
    //Carta ccc = {4,5,1,false};
    disco.open("cartas.bin", ios::out | ios::binary);
    disco.write(reinterpret_cast<char*>(&cartaStr), sizeof(Carta));
    disco.close();

}**/


