#ifndef PAGEDMATRIX_H
#define PAGEDMATRIX_H

#include <ostream>
#include <fstream>
#include <iostream>
using namespace std;
class PagedMatrix
{
public:
    PagedMatrix();
    void crear_disco();
    void leer_disco();
    void escribir_en_Disco();
    void buscar_cartaEnDisco();
    void swap_cartaEnDisco();

private:
    fstream disco;
};

#endif // PAGEDMATRIX_H
