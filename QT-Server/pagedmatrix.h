#ifndef PAGEDMATRIX_H
#define PAGEDMATRIX_H

#include <fstream>
#include <iostream>
#include "card.h"

using namespace std;
class PagedMatrix
{
public:
    PagedMatrix();
    //opcion 1: hacer structs tarjeta y escribir cada una en el archivo binario
    void llamar_matrizDisco();
    void leer_matrizDisco(int howManyStructs);
    void leer2_matriz(int cartaNum);
    void writeFirst_matrizDisco(Carta cartaStr);
    void append_aDisco(Carta cartStr);
    void buscar_cartaEnDisco();
    void modificar_disco(int cualCarta);
    void write_in_position(Carta cartaStr, int pos);


    //opcion 2: usar un array de structs tarjeta y escribir ese array
    // en el archivo binario cada vez que se modifique
    void generar_archivo(Carta all_cards[3]);
    void llenar_array();
    void update_array(int whichCard);
    void leer_arrayArchivo();


private:
    fstream disco;
    Carta allCards[3];

};

#endif // PAGEDMATRIX_H
