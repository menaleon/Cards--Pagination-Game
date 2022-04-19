#ifndef PAGEDMATRIX_H
#define PAGEDMATRIX_H

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <QDebug>
#include <vector>
#include "card.h"

using namespace std;
class PagedMatrix
{
public:
    PagedMatrix();
    //opcion 1: hacer structs tarjeta y escribir cada una en el archivo binario
    //void llamar_matrizDisco();
    //void leer_matrizDisco(int howManyStructs);
    //void leer2_matriz(int cartaNum);
    //void writeFirst_matrizDisco(Carta cartaStr);
    //void append_aDisco(Carta cartStr);
    //void modificar_disco(int cualCarta);
    //void write_in_position(Carta cartaStr, int pos);


    //-----------opcion 2: usar un array de structs tarjeta y escribir ese array
    // en el archivo binario cada vez que se modifique
    void escribir_archivo(Carta arrayCartas[36]);
    void llenar_array();
    void update_array(int whichCard);
    vector<Card> leer_arrayArchivo(int f1, int c1, int f2, int c2);
    int buscar_CartasCargadas(vector<Card> cargadas, int fila, int col);
    vector<int> random_int(size_t vectorSize);
    void llenar_inMemory();
    size_t verificar_ganadas();


private:
    fstream disco;
    Carta allCards[36];

    vector<Card> onDiscCards;
    vector<Card> inMemoryCards;

};

#endif // PAGEDMATRIX_H
