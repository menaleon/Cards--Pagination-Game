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

    void escribir_archivo(Carta arrayCartas[36]);
    void llenar_array();
    void update_array(int f1, int c1, int f2, int c2);
    vector<Card> leer_arrayArchivo();
    size_t buscar_CartasCargadas(int fila, int col);
    vector<int> random_int(size_t vectorSize);
    void llenar_inMemory();
    size_t verificar_ganadas();
    bool cargar_carta(int fila, int col);


    int pageHits = 0;
    int pageFaults = 0;
    vector<Card> onDiscCards;
    vector<Card> inMemoryCards;

private:
    fstream disco;
    Carta allCards[36];
    int cont_llenadoInMemory = 0;




};

#endif // PAGEDMATRIX_H
